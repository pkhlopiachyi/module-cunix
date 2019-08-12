#include "../include/minishell.h"
#include "../include/hash.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

hashtable_t *ht;

char *builtin_str[] = {
    "echo", 
    "cd",
    "pwd",
    "env",
    "export",
    "exit"
};

int lsh_num_builtins(){
   return sizeof(builtin_str) / sizeof(char *);
}

int lsh_echo(char **args){
    int i=1;
    while(args[i] != NULL){
        printf("%s ", args[i]);
        i++;
    } 
    printf("\n");
    
    return 1;
}

int lsh_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else{
        if(chdir(args[1]) != 0){
            perror("lsh");
        }
    }
    
    return 1;
}

int lsh_pwd(char **args){
    char cwd[100];
    if(args[1]){
        fprintf(stderr, "lsh: too many arguments\n");
    } else{
        getcwd(cwd, FILENAME_MAX);
        printf("%s\n", cwd);
    }
    return 1;
}

int lsh_env(char **args){
    
    hash_print(ht);
    
    return 1;
}

int lsh_export(char **args){
    
    char *key, *data, *tmp, *word;
    int i;

    if(args[1] == NULL){
        return 1;
    }
    word = args[1];
    tmp = strtok(word, "=");
    key = malloc((strlen(tmp)+1)*sizeof(char));
    strcpy(data,tmp);
    printf("\n");

    while(tmp != NULL){
        data = malloc((strlen(tmp)+1)*sizeof(char *));
        strcpy(data, tmp);
        tmp = strtok(NULL, "=");
    }

    if(!strcmp(key, data)){
        return 1;
    }

    hash_set(ht, key, (void*)data);

    return 1;
}

int lsh_exit(char **args){
    
    return 0;
}

int (*builtin_func[]) (char **) = {
    &lsh_echo,
    &lsh_cd,
    &lsh_pwd,
    &lsh_env,
    &lsh_export,
    &lsh_exit
};

int lsh_launch(char **args){
   pid_t pid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }else if(pid <0){
            perror("lsh");
    }else {
        do{
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int lsh_execute(char **args){
    int i;
    int sz;
    if (args[0] == NULL){
        return 1;
    }
    sz = lsh_num_builtins();
    for(i = 0; i < sz; i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }
    
    return lsh_launch(args);
}

char *lsh_read_line(void){
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if(!buffer){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();

        if(c == EOF){
            exit(EXIT_SUCCESS);
        } else if (c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else{
            buffer[position] = c;
        }
        position++;

        if(position >= bufsize){
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if(!buffer){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **lsh_split_line(char *line){
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize*sizeof(char*));
    char *token, **tokens_backup;

    if(!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += LSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize*sizeof(char*));
            if(!tokens){
                free(tokens_backup);
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    
    return tokens;
}

void lsh_loop(void){
    char *line;
    char **args;
    int status;

    do{
        printf("$> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        
        free(line);
        free(args);
    }while(status);
}

int main(int args, char **argv){
    

    ht = hash_create(64);
    lsh_loop();
    hash_destroy(ht);
    
    return EXIT_SUCCESS;
}

#include "../include/minishell.h"
#include "../include/hash.h"
#include "../include/linked_list.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char PWD[1024];
char PATH[1024];

node_t *keyslst;
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

char *without_first_char(char *str)
{
        for (int i = 0; i <= strlen(str); i++)
                str[i] = str[i + 1];
        return str;
}

int lsh_echo(char ** args){
    int i = 1;
    while (1)
    {
        if (args[i] == NULL)
            break;
        if (args[i][0] == '$')
            printf("%s ", hash_get(ht, without_first_char(args[i])));
        else printf("%s ", args[i]);
            i++;
    }
    printf("\n");
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

int lsh_env (char **args){
    node_t *tmp = keyslst;
    while( tmp != NULL)
    {
        printf("%s: %s\n", tmp->data, hash_get(ht, tmp->data));
        tmp = tmp->next;
    }
        
    return 1;
}

int check_export_args(char * arg)
{
    int i = 0;
    int size = strlen(arg);
        
    for (i; i < size; i++)
        if (arg[i] == '=' && i+1 != size)
        {
            i = -1;
            break;
        }
        
    return i == -1 ? 0 : 1;
}

int lsh_export(char **args){
    int i = 1;
        
    while(1){
        if(args[i] == NULL)
            break;
        else if (check_export_args(args[i]))
                return printf("minishell: bad assigment\n");
    
    char *token;
    char s[2] = "=";
    
    token = strtok(args[i], s);
    if (keyslst == NULL && ht == NULL){
        keyslst = list_create(token);
        ht = hash_create(18);
    } else if (list_check(keyslst, token))
                list_push(keyslst, token);
        char *tmp = token;
                  
        token = strtok(NULL, s);
        hash_set(ht, tmp, token);
        i++;
    }

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

char **split_command_line(char *command){
    int position = 0;
    int no_of_tokens = 64;
    char ** tokens = malloc(sizeof(char *) * no_of_tokens);
    char delim[2] = " ";
    char * token = strtok(command, delim);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;

    return tokens;
}

char *read_command_line(void){
    int position = 0;
    int buf_size = 1024;
    char *command = (char *)malloc(sizeof(char) * 1024);
    char symbol;
    symbol = getchar();
    while (symbol != EOF && symbol != '\n')
    {
        command[position] = symbol;
        if (position >= buf_size){
             buf_size += 64;
                command = realloc(command, buf_size);
        }
        position++;
        symbol = getchar();
    }
    command[position] = '\0';
    
    return command;
}

int shell_execute(char **args){
    int i = 0;

    if (args[0] == NULL)
        return 1;

    for (i; i < 6; i++)
        if (strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);

    return printf("minishell: this command doesnt exist");
}

void signalHandler(int sign){
        if (sign == SIGINT){
                puts("");
                write(1, "$_>", 3);
                signal(SIGINT, signalHandler);
        }
}

void shell_loop(void){
    char *command_line;
    char **arguments;
    int     status = 1;
    while (status)
    {
        printf("$_> ");
        signal(SIGINT, signalHandler);
        command_line = read_command_line();
        if (strcmp(command_line, "") == 0 )
            continue;
        arguments = split_command_line(command_line);
        status = shell_execute(arguments);
    }
}

int main(){
    getcwd(PWD, sizeof(PWD));
    strcpy(PATH, PWD);
    strcat(PATH, "/cmds/");
    shell_loop();
    
    return 0;
}


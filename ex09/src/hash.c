#include "../include/hash.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/linked_list.h"

hashtable_t *hash_create(unsigned int size){
  hashtable_t   *node_n;
  if(size > 0){
    node_n = (hashtable_t *) malloc(sizeof(hashtable_t));
    node_n->size = size;
    node_n->table = (void **) malloc(size * sizeof(void *));
    for(unsigned int i = 0; i < size; i++)
      node_n->table[i] = NULL;
    return node_n;
  }
  return NULL;
}

void hash_destroy(hashtable_t *ht, void (*fp)(void *data)){
  node_t *node_n;
  for(unsigned int i = 0; i < ht->size; i++){
    if(ht->table[i] != NULL){
      node_n = ht->table[i];
      list_destroy(&node_n, fp);
    }
  }
  free(ht->table);
  ht->table = NULL;
  free(ht);
}

unsigned int hash_func(char *key){
  if (key == NULL)
    return 0;
  int temp = 0;
  int i = 0;
  while (key[i] != '\0'){
    temp += key[i];
    i++;
  }

  return temp;
}

void hash_set(hashtable_t *ht, char *key, void *ptr, void (*fp)(void *data)){
  int count;
  count = hash_func(key) % ht->size;
  if(ht->table[count] == NULL)
    ht->table[count] = list_create(ptr);
  else
    list_push(ht->table[count], ptr);
  fp(key);
}

void  *hash_get(hashtable_t *ht, char *key){
  int count;
  node_t *node_n;
  count = hash_func(key) % ht->size;
  if(ht->table[count] == NULL)
    return NULL;
  node_n = ht->table[count];
  return (node_n->data);
}

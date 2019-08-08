#include "linked_list.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"



node_t *list_create(void *data){

    node_t *head = NULL;
    head = malloc(sizeof(node_t));
    head -> data = data;
    head -> next = NULL;
    return head;
}


void list_destroy(node_t **head, void (*fp)(void *data)){
    if(head == NULL){
        return;
    }
    node_t *tmp = *head;
    node_t *next;

    while(tmp != NULL){
        fp(tmp -> data);
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    *head = NULL;
}

void list_push(node_t *head, void *data){
    if(head == NULL){
        return;
    }
    while(head -> next)
    {
        head = head->next;
    }
    head -> next = malloc(sizeof(node_t));
    head = head -> next;
    head -> next = NULL;
    head -> data = data;
}


void list_unshift(node_t **head, void *data){
    node_t *new_elem = malloc(sizeof(node_t));
    new_elem -> data = data;
    new_elem -> next = *head;
    *head = new_elem;
}


void list_print(node_t *head){

    node_t *tmp = head;
    while (tmp != NULL){
        printf("%p\n", tmp -> data);
        tmp = tmp -> next;
    }
}

void list_visitor(node_t *head, void (*fp)(void *data)){
    while(head != NULL){
        (*fp)(head -> data);
        head = head -> next;
    }
}

void *list_pop(node_t **head){
    node_t *tmp, *ntmp = *head;
    void *res;
    if(ntmp -> next){
        tmp = ntmp -> next;
    }
    else{
        res = (*head) -> data;
        free((*head)->data);
        free(*head);
        *head = NULL;
        return res;
    }

    while(tmp -> next){
        tmp = tmp -> next;
        ntmp = ntmp -> next;
    }
    res = tmp -> data;
    free(tmp -> data);
    free(tmp);
    ntmp -> next = NULL;
    
    return res;
}

void *list_shift(node_t **head){
    void *res;
    node_t *tmp = *head;
    (*head) = (*head) -> next;
    res = tmp -> data;
    free(tmp -> data);
    free(tmp);
    
    return res;
}

void *list_remove(node_t **head, int pos){
    void *res;
    node_t *tmp = *head, *ntmp;

    if(pos == 0){
        res = tmp -> data;
        (*head) = (*head) -> next;
        free(tmp -> data);
        free(tmp);        
    }
    else{
        ntmp = tmp;
        tmp = tmp -> next;
        while(pos > 1){
            tmp = tmp -> next;
            ntmp = ntmp -> next;
            pos--;
        }
        ntmp -> next = tmp -> next;
        res = tmp -> data;
        free(tmp -> data);
        free(tmp);
    }
    return res;   
}

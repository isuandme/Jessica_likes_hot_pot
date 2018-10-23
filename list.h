#ifndef _LIST_H
#define _LIST_H

#include <sys/types.h>

struct node {
    pid_t child;
    node *next;
} node_t;

void push(node_t * head, int val) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    /* now we can add a new variable */
    current->next = malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}


#endif

#ifndef _HIST_LIST_H
#define _HIST_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

struct cmdNode {
    char *str;
    struct cmdNode *next;
};

typedef struct cmdNode cnode;
cnode *newnode, *ptr, *prev, *temp;
cnode *first = NULL, *last = NULL;
int list_size = 0;

/* Creating Node */
cnode* create_node(char cmd[]) {
    newnode = (cnode *)malloc(sizeof(cnode));
    if (newnode == NULL) {
        printf("\n Memory was not allocated");
        return 0;
    } else {
        newnode->str = cmd;
        newnode->next = NULL;
        return newnode;
    }
}

void delete_index(int index)
{
    /* Makes sure the list is longer than requested index */
    if(list_size < index) {
        printf("List size is smaller than specified index");
        return;
    }
    
    //printf("index: %d\n", index);
    if(index > 0)
        index--;
    
    ptr = first;
    prev = NULL;
    
    /*  */
    if(index == 0) {
        first= first->next;
        free(first);
        return;       // returns 0 upon success
    }
    
    int i;
    for(i = 0; i < index; i++) {
        prev = ptr;
        ptr = ptr->next;
    }
    
    if(ptr == NULL)
        return;
    
    prev->next = ptr->next;
    free(ptr);
    
    list_size--;
}

/* Inserting Node at First */
void insert_node_first(char cmd[])
{
    if(list_size > 9)
        delete_index(10);
    
    newnode = create_node(cmd);
    if (first == last && first == NULL)
    {
        first = last = newnode;
        first->next = NULL;
        last->next = NULL;
    } else {
        temp = first;
        first = newnode;
        first->next = temp;
    }
    list_size++;
}

char *get_index(int index){
    /* Makes sure the list is longer than requested index */
    if(list_size < index) {
        printf("List size is smaller than specified index");
        return NULL;
    }
    
    //printf("index: %d\n", index);
    if(index > 0)
        index--;
    
    ptr = first;
    prev = NULL;
    
    /*  */
    if(index == 0) {
        return first->str;
    }
    
    int i;
    for(i = 0; i < index; i++) {
        ptr = ptr->next;
    }
    
    if(ptr == NULL)
        return NULL;
    
    return ptr->str;
}

/* Displays non-empty List from Beginning to End */
void display_cmdList()
{
    if (first == NULL) {
        printf(":No nodes in the list to display\n");
    } else {
        for (ptr = first; ptr != NULL; ptr = ptr->next) {
            printf("\t%s", ptr->str);
        }
    }
    printf("\n");
}









/*  Delete Node with specified pid in a non-empty list
 parameters: Takes a pid_t value
 returns: 0 on success
 */
//int delete_pidNode(char cmd[])
//{
//
//    ptr = first;
//    prev = NULL;
//
//    /*  */
//    if(ptr != NULL && ptr->str == cmd) {
//        first= first->next;
//        free(first);
//        return 0;       // returns 0 upon success
//    }
//
//    /*  */
//    while(ptr != NULL && ptr->str != cmd) {
//        prev = ptr;
//        ptr = ptr->next;
//    }
//
//    /*  */
//    if(ptr == NULL)
//        return -1;
//
//    prev->next = ptr->next;
//    free(ptr);
//
//    return 0;
//}

#endif

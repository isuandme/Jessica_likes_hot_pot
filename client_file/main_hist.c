#include <stdio.h>
#include <string.h>

#include "historyList.h"

int main(int argv, char * argc[]){
    
    insert_node_first("hello");
    insert_node_first("hi");
    insert_node_first("sup");
    insert_node_first("one");
    insert_node_first("two");
    display_cmdList();
    char *tmp = get_index(4);
    if(tmp == NULL)
        printf("\nasdfa\n");
    else
        printf("\n%s\n", tmp);
    return 0;
}

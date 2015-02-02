#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"

struct row_of_memory* add_to_list (struct row_of_memory* head,
                                   int flag,
				      short unsigned int address, 
				      short unsigned int contents) 
{
	struct row_of_memory *newnode = malloc(sizeof(struct row_of_memory));
    newnode->flag = flag;
    newnode->address = address;
    newnode->label = NULL;
    newnode->contents = contents;
    newnode->assembly = NULL;
    newnode->next = NULL;
    if(head == NULL){
        head = newnode;
    }
    else{
        struct row_of_memory *node = head;
        while(node->next != NULL){
            node = node->next;
        }
        node->next = newnode;
    }
    return head;
}

struct row_of_memory* search_address (struct row_of_memory* head, 
				      short unsigned int address ) 
{
    struct row_of_memory *node = head;
    while(node != NULL){
        if(node->address == address){
            return node;
        }
        node = node->next;
    }
	return NULL ;
}

struct row_of_memory* search_opcode  (struct row_of_memory* head, 
				      short unsigned int opcode, short unsigned int digit )
{
	struct row_of_memory *node = head;
    while(node != NULL){
        short unsigned int contents = node->contents;
        short unsigned int buffer = 0xFFFF - ((1<<digit) - 1);
        if((buffer&contents) == (opcode<<digit)){
            return node;
        }
        node = node->next;
    }
	return NULL ;
}

void print_list (struct row_of_memory* head )
{
	if(head == NULL){
        printf("list is empty");
    }
    else{
        struct row_of_memory *node = head;
        while(node != NULL){
            printf("%s\t%04x\t%04x\t%s\n", node->label, node->address, node->contents, node->assembly);
            node = node->next;
        }
    }
	return ;
}

struct row_of_memory* delete_list    (struct row_of_memory* head )
{
	struct row_of_memory *node = head;
    struct row_of_memory *buffer;
    while(node != NULL){
        free(node->label);
        free(node->assembly);
        buffer = node -> next;
        free(node);
        node = buffer;
    }
	return NULL ;
}

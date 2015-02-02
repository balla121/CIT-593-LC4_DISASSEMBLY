#include <stdio.h>
#include "lc4_memory.h"
#include <string.h>
#include <stdlib.h>

unsigned int readcount(int n1, int n2);
/* declarations of functions that must defined in lc4_loader.c */

FILE* get_file() 
{
	char filename[20];
    printf("Please enter the file name:");
    scanf("%s",filename);
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp){
        return fp;
    }
    return NULL ;
}

struct row_of_memory* parse_file (FILE* fp, struct row_of_memory *memory)
{
	int c;
    unsigned int count;
    short unsigned int address;
    short unsigned int contents;
    while((c = fgetc(fp))!=EOF){
        //code
        if( c == 0xca ){
            if(fgetc(fp) == 0xde){
                address = readcount(fgetc(fp), fgetc(fp));
                count = readcount(fgetc(fp), fgetc(fp));
                for(int i = 0; i<count ; i++){
                    struct row_of_memory *node = search_address(memory, address);
                    contents = readcount(fgetc(fp), fgetc(fp));
                    if(node == NULL){
                        memory = add_to_list(memory, 0, address, contents);
                    }
                    else{
                        node->flag = 0;
                        node->contents = contents;
                    }
                    address++;
                }
            }
        }
        //data
        else if( c == 0xda ){
            if(fgetc(fp) == 0xda){
                address = readcount(fgetc(fp), fgetc(fp));
                count = readcount(fgetc(fp), fgetc(fp));
                for(int i = 0; i<count ; i++){
                    struct row_of_memory *node = search_address(memory, address);
                    contents = readcount(fgetc(fp), fgetc(fp));
                    if(node == NULL){
                        memory = add_to_list(memory, 1, address, contents);
                    }
                    else{
                        node->flag = 1;
                        node->contents = contents;
                    }
                    address++;
                }
            }
        }
        //symbol
        else if( c == 0xc3 ){
            if(fgetc(fp) == 0xb7){
                address = readcount(fgetc(fp), fgetc(fp));
                count = readcount(fgetc(fp), fgetc(fp));
                char buffer[count];
                for(int i = 0; i<count;i++){
                    buffer[i] = fgetc(fp);
                }
                buffer[count] = '\0';
                struct row_of_memory *node = search_address(memory, address);
                if(node == NULL){
                    memory = add_to_list(memory, 0, address, 0);
                }
                node = search_address(memory, address);
                node->label = malloc(sizeof(char)*(count+1));
                strcpy(node->label, buffer);
            }
        }
    }
//    delete_list(memory);
    fclose(fp);
    return memory;
}

unsigned int readcount(int n1, int n2)
{
    n1 = n1 << 8;
    return n1 + n2;
}

//int main(){
//    FILE* fp = get_file();
//    struct row_of_memory *memory;
//    if(fp != NULL){
//        parse_file(fp, memory);
//    }
//    return 0;
//
//}

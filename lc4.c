#include <stdio.h>
#include "lc4_memory.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"

/* program to mimic LC4's program & data memory */

int main () {

	/**
	 * main() holds the linked list &
	 * only calls functions in other files 
	 */

	/* step 1: create head pointer to linked list: memory 	*/
    
	struct row_of_memory* memory = NULL ;
    
    /* step 2: call function: get_file() in lc4_loader.c 	*/
    
    FILE* fp = get_file();
    if(fp == NULL){
        printf("The file does not exist!");
        return 1;
    }
    
    /* step 3: call function: parse_file() in lc4_loader.c 	*/
    
    memory = parse_file(fp, memory);
    
    /* step 4: call function: reverse_assemble() in lc4_disassembler.c */
    
    reverse_assemble(memory);
    
    /* step 5: call function: print_list() in lc4_memory.c 	*/
    
    print_list(memory);
    
    /* step 6: call function: disassembler() in lc4_disasembler.c */
    
    disassembler(memory);
    
    /* step 6: call function: delete_list() in lc4_memory.c */
    
    delete_list(memory);
    
    printf("%lu\n", sizeof(struct row_of_memory));
    
    return 0;
}

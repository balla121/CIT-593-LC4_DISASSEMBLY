#include <stdio.h>
#include "lc4_memory.h"
#include <string.h>
#include "lc4_loader.h"
#include <stdlib.h>

void assemble(struct row_of_memory * node, short unsigned int contents, char* word);
void assembleHelper(struct row_of_memory * node, short unsigned int opcode, short unsigned int digit, char* word);
int reverse_assemble (struct row_of_memory* memory);
int disassembler(struct row_of_memory *memory);

int reverse_assemble (struct row_of_memory* memory) 
{
    struct row_of_memory *node = memory;
    assembleHelper(node, 0, 9, "NOP");
    assembleHelper(node, 4, 9, "BRn");
    assembleHelper(node, 6, 9, "BRnz");
    assembleHelper(node, 5, 9, "BRnp");
    assembleHelper(node, 2, 9, "BRz");
    assembleHelper(node, 3, 9, "BRzp");
    assembleHelper(node, 1, 9, "BRp");
    assembleHelper(node, 7, 9, "BRnzp");
    assembleHelper(node, 1, 12, "add");
    assembleHelper(node, 2, 12, "cmp");
    assembleHelper(node, 9, 11, "JSR");
    assembleHelper(node, 8, 11, "JSRR");
    assembleHelper(node, 5, 12, "and");
    assembleHelper(node, 6, 12, "LDR");
    assembleHelper(node, 7, 12, "STR");
    assembleHelper(node, 8, 12, "RTI");
    assembleHelper(node, 9, 12, "CONST");
    assembleHelper(node, 10, 12, "mv");
    assembleHelper(node, 24, 11, "JMPR");
    assembleHelper(node, 25, 11, "JMP");
    assembleHelper(node, 13, 12, "HICONST");
    assembleHelper(node, 15, 12, "TRAP");
	return 0 ;
}

void assembleHelper(struct row_of_memory * node, short unsigned int opcode, short unsigned int digit, char* word){
    while(node != NULL){
        struct row_of_memory *buffer = search_opcode(node, opcode, digit);
        if(buffer != NULL){
            if(buffer->assembly == NULL){
                if(buffer->flag == 0){
                    short unsigned int contents = buffer->contents;
                    assemble(buffer, contents, word);
                }
                node = buffer->next;
            }
        }
        else{
            return ;
        }
    }
    return ;
}

short unsigned int convert(short unsigned int contents, short unsigned int digit){
    short unsigned int assist = contents & ((1<<digit) - 1);
    short signed int result = assist & (1<<(digit-1)) ? assist | (((~0) >> digit) << digit) : assist;
    return result;
}

void assemble(struct row_of_memory * node, short unsigned int contents, char* word){
    short unsigned int dest, src, rt, uimm7, uimm8, uimm4;
    short signed int imm11, imm9, imm7, imm6, imm5;
    dest = (contents & 3584)>>9;
    src = (contents & 448)>>6;
    rt = contents & 7;
    uimm7 = contents & 127;
    imm11 = convert(contents, 11);
    imm7 = convert(contents, 7);
    imm9 = convert(contents, 9);
    imm5 = convert(contents, 5);
    imm6 = convert(contents, 6);
    uimm8 = contents & 255;
    uimm4 = contents & 15;
    char s[20];
    char opcode[10];
    if(strcmp(word, "NOP") == 0){
        sprintf(s, "%s", "NOP");
    }
    else if(strcmp(word, "BRn") == 0){
        sprintf(s, "%s #%d", "BRn", imm9);
    }
    else if(strcmp(word, "BRnz") == 0){
        sprintf(s, "%s #%d", "BRnz", imm9);
    }
    else if(strcmp(word, "BRnp") == 0){
        sprintf(s, "%s #%d", "BRnp", imm9);
    }
    else if(strcmp(word, "BRz") == 0){
        sprintf(s, "%s #%d", "BRz", imm9);
    }
    else if(strcmp(word, "BRzp") == 0){
        sprintf(s, "%s #%d", "BRzp", imm9);
    }
    else if(strcmp(word, "BRp") == 0){
        sprintf(s, "%s #%d", "BRp", imm9);
    }
    else if(strcmp(word, "BRznp") == 0){
        sprintf(s, "%s #%d", "BRznp", imm9);
    }
    else if(strcmp(word, "add") == 0){
        if(32 & contents){
            // add immediate
            strcpy(opcode, "ADD");
            sprintf (s, "%s R%d, R%d, #%d", opcode, dest, src, imm5);
        }
        else{
            // determine subopcode
            short unsigned int subopcode = contents & 56;
            if(subopcode == 0){
                strcpy(opcode, "ADD");
            }
            else if(subopcode == 8){
                strcpy(opcode, "MUL");
            }
            else if(subopcode == 16){
                strcpy(opcode, "SUB");
            }
            else if(subopcode == 24){
                strcpy(opcode, "DIV");
            }
            rt = contents & 7;
            if(opcode){
                sprintf(s, "%s R%d R%d R%d", opcode, dest, src, rt);
            }
        }
    }
    else if(strcmp(word, "cmp") == 0){
        short unsigned int subopcode = (384 & contents)>>7;
        if(subopcode == 0){
            sprintf(s, "%s R%d R%d", "CMP", dest, rt);
        }
        else if(subopcode == 1){
            sprintf(s, "%s R%d R%d", "CMPU", dest, rt);
        }
        else if(subopcode == 2){
            sprintf(s, "%s R%d #%d", "CMPI", dest, imm7);
        }
        else if(subopcode == 3){
            sprintf(s, "%s R%d #%d", "CMPIU", dest, imm7);
        }
    }
    else if(strcmp(word, "JSR") == 0){
        sprintf(s, "%s #%d", "JSR", imm11);
    }
    else if(strcmp(word, "JSRR") == 0){
        sprintf(s, "%s R%d", "JSRR", src);
    }
    else if(strcmp(word, "and") == 0){
        short unsigned int subopcode = (56 & contents)>>3;
        if(subopcode == 0){
            sprintf(s, "%s R%d R%d R%d", "AND", dest, src, rt);
        }
        else if(subopcode == 1){
            sprintf(s, "%s R%d R%d", "NOT", dest, src);
        }
        else if(subopcode == 2){
            sprintf(s, "%s R%d R%d R%d", "OR", dest, src, rt);
        }
        else if(subopcode == 3){
            sprintf(s, "%s R%d R%d R%d", "XOR", dest, src, rt);
        }
        else{
            sprintf(s, "%s R%d R%d #%d", "AND", dest, src, imm5);
        }
    }
    else if(strcmp(word, "LDR") == 0){
        sprintf(s, "%s R%d R%d #%d", "LDR", dest, src, imm6);
    }
    else if(strcmp(word, "STR") == 0){
        sprintf(s, "%s R%d R%d #%d", "STR", dest, src, imm6);
    }
    else if(strcmp(word, "RTI") == 0){
        sprintf(s, "%s", "RTI");
    }
    else if(strcmp(word, "CONST") == 0){
        sprintf(s, "%s R%d #%d", "CONST", dest, imm9);
    }
    else if(strcmp(word, "mv") == 0){
        short unsigned int subopcode = (48 & contents)>>4;
        if(subopcode == 0){
            sprintf(s, "%s R%d R%d #%d", "SLL", dest, src, uimm4);
        }
        else if(subopcode == 1){
            sprintf(s, "%s R%d R%d #%d", "SRA", dest, src, uimm4);
        }
        else if(subopcode == 2){
            sprintf(s, "%s R%d R%d #%d", "SRL", dest, src, uimm4);
        }
        else{
            sprintf(s, "%s R%d R%d R%d", "AND", dest, src, rt);
        }
    }
    else if(strcmp(word, "JMPR") == 0){
        sprintf(s, "%s R%d", "JMPR", src);
    }
    else if(strcmp(word, "JMP") == 0){
        sprintf(s, "%s #%d", "JMP", imm11);
    }
    else if(strcmp(word, "HICONST") == 0){
        sprintf(s, "%s R%d #%d", "HICONST", dest, uimm8);
    }
    else if(strcmp(word, "TRAP") == 0){
        sprintf(s, "%s #%d", "TRAP", uimm8);
    }
    if(strlen(s)){
        node->assembly = malloc(sizeof(char)*(strlen(s)+1));
        strcpy(node->assembly, s);
    }
    
}

int disassembler(struct row_of_memory *memory){
    FILE *fp;
    char s[20];
    printf("Please enter the output file:");
    scanf("%s", s);
    fp = fopen(s, "wa");
    if(!fp){
        printf("The file does not exist!");
        return 1;
    }
    struct row_of_memory *node = memory;
    short unsigned int address = -1;
    while(node != NULL){
        int count = 0;
        // code
        if(node->flag == 0){
            // continous address
            if(address != (node->address)){
                fprintf(fp, ".CODE\n");
                fprintf(fp, ".ADDR x%04x\n", node->address);
                address = node->address;
            }
            if(node->label != NULL){
                fprintf(fp, "%s\n", node->label);
            }
            fprintf(fp, "\t%s\n", node->assembly);
            address++;
        }
        // data
        else{
            // continous address
            if(address != (node->address)){
                fprintf(fp, ".DATA\n");
                fprintf(fp, ".ADDR x%04x\n", node->address);
                address = node->address;
            }
            if(node->label != NULL){
                fprintf(fp, "%s\n", node->label);
            }
            fprintf(fp, "\t.FILL #%d\n", node->contents);
            address++;
        }
        node = node->next;
    }
    fclose(fp);
    return 0;
}

//int main(){
//    FILE* fp = get_file();
//    struct row_of_memory *memory;
//    if(fp != NULL){
//        memory = parse_file(fp, memory);
//        printf("start\n");
//        reverse_assemble(memory);
//        print_list(memory);
//        disassembler(memory);
//        delete_list(memory);
//    }
//    return 0;
//    
//}


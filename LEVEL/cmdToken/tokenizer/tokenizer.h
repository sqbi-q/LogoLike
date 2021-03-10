//#include "./addChar.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

typedef struct Function{
	char* function;
	char** arguments;
	int arg_size;
	//unsigned char ERRNO;
}Function;

void CMDT(char* str, Function* dest_func);

#endif

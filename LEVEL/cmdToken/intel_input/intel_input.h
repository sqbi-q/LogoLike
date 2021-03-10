#include "../tokenizer/tokenizer.h"
#include "../tokenizer/addChar.h"
#include <stdlib.h>

#ifndef _INTELINPUT_H_
#define _INTELINPUT_H_

typedef struct{
	char* text;
        unsigned int size;
}string;


void getInput(char* output);

#endif

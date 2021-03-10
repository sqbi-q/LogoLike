#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../OBJECTS/objects.h"
#include "../tokenizer/tokenizer.h"
#include "../../../map/src/map.h"

#ifndef _CMDLIST_H_
#define _CMDLIST_H_

typedef struct {
	struct OBJECT_Moveable* objects;
	int objects_size;
	int* indexes;
	int indexes_size;

	//int* win_size;
} target_t;

extern target_t TARGETS;
extern map_str_t variables;
extern char* output;

int Execute(Function* func);

#endif

#include "./tokenizer.h"

void addChar(char* str, char add){
        int length = strlen(str);
        str[length] = add;
        str[length+1] = '\0';
};

void CMDT(char* str, Function* dest_func){
	const unsigned int LENGTH = strlen(str);
	//const unsigned int LENGTH = wcslen(str);
	const unsigned int FUNC_MAX_LENGTH = LENGTH;
	const unsigned int PARAMS_MAX_LENGTH = LENGTH;
	const unsigned int PARAMS_MAX_SIZE = LENGTH/2;

	unsigned int func_real_length = 0;
	unsigned int* param_real_length;
	param_real_length = malloc(LENGTH*sizeof(int));
	int i=0;
	while(i<LENGTH){
		param_real_length[i] = 0;
		i++;
	}

	unsigned int param_real_size = 0;

	i=0;
	int j=0;

	char func[FUNC_MAX_LENGTH];
	char params[PARAMS_MAX_SIZE][PARAMS_MAX_LENGTH];

	dest_func->function = (char*) malloc(FUNC_MAX_LENGTH * sizeof(char));
	dest_func->arguments = (char**) malloc(PARAMS_MAX_SIZE * sizeof(char*));
	while(i<PARAMS_MAX_SIZE){
		dest_func->arguments[i] = (char*) malloc(PARAMS_MAX_LENGTH * sizeof(char));
		strcpy(dest_func->arguments[i], "");
		//wcscpy(dest_func->arguments[i], L"");
		i++;
	}
	strcpy(dest_func->function, "");
	//wcscpy(dest_func->function, L"");

	i=0;
	while(str[i] == ' ' && str[i] != '\0') i++;
	while(str[i] != '\0' && str[i] != ' ' && str[i] != '['){
		addChar(dest_func->function, tolower(str[i]));
		func_real_length++;
		i++;
	}
	while(str[i] != '\0'){
		//check if blank
		if(str[i] == ' ') { i++; continue; }
		//is nested
		else if(str[i] == '[') {
			//NESTED - TODO
			//if brackets 
			int brackets = 0;
			while(str[i] != '\0'){
				if(str[i] == ']') brackets--;
				else if(str[i] == '[') brackets++;

				if(str[i] == ']' && brackets == 0) break;
				
				addChar(dest_func->arguments[j], str[i]);
				param_real_length[j]++;
				i++;
			}
		//normal arguments
		}else{
			while(str[i] != ' ' && str[i] != '\0'){
				addChar(dest_func->arguments[j], str[i]);
				param_real_length[j]++;
				i++;
			}j++;
		}
	}
	dest_func->arg_size = j;

	/*if(str[i] == '['){
		param_real_size++;
		while(str[i] != '\0'){
			addChar(dest_func->arguments[j], str[i]);
			param_real_length[j]++;
			i++;
		}
	}
	if(str[i+1] == '['){
		param_real_size++;
		i++;
		while(str[i] != '\0'){
			addChar(dest_func->arguments[j], str[i]);
			param_real_length[j]++;
			i++;
		}
	}
	else{
		while(str[i] == ' '){
			param_real_size++;
			i++;
			while(str[i] != ' ' && str[i] != '\0'){
				addChar(dest_func->arguments[j], str[i]);
				param_real_length[j]++;
				i++;
			} j++;
		}
	}
	i=0;
	dest_func->arg_size = param_real_size;*/

}

/*
void CMDT(char* str, Function* dest_func){
	const unsigned int LENGTH = strlen(str);
	const unsigned int FUNC_MAX_LENGTH = LENGTH;
	const unsigned int PARAMS_MAX_LENGTH = LENGTH;
	const unsigned int PARAMS_MAX_SIZE = LENGTH/2;

	unsigned int func_real_length = 0;
	unsigned int* param_real_length;
	param_real_length = malloc(LENGTH*sizeof(int));
	int i=0;
	while(i<LENGTH){
		param_real_length[i] = 0;
		i++;
	}

	unsigned int param_real_size = 0;

	i=0;
	int j=0;

	char func[FUNC_MAX_LENGTH];
	char params[PARAMS_MAX_SIZE][PARAMS_MAX_LENGTH];

	dest_func->function = (char*) malloc(FUNC_MAX_LENGTH * sizeof(char));
	dest_func->arguments = (char**) malloc(PARAMS_MAX_SIZE * sizeof(char*));
	while(i<PARAMS_MAX_SIZE){
		dest_func->arguments[i] = (char*) malloc(PARAMS_MAX_LENGTH * sizeof(char));
		strcpy(dest_func->arguments[i], "");
		i++;
	}
	strcpy(dest_func->function, "");

	i=0;
	while(str[i] != '\0' && str[i] != ' '){
		addChar(dest_func->function, tolower(str[i]));
		func_real_length++;
		i++;
	}

	while(str[i] == ' '){
		param_real_size++;
		i++;
		while(str[i] != ' ' && str[i] != '\0'){
			addChar(dest_func->arguments[j], str[i]);
			param_real_length[j]++;
			i++;
		} j++;
	}
	i=0;
	dest_func->arg_size = param_real_size;

}
*/

#include "./execute.h"

/*TODO upgrade for Execute and every function (old -> new)
error codes:
succesful - 0b000
not enough args - 0b001
too much args - 0b010
invalid arg - 0b011
file missing - 0b100
function not found 0b101

respone type:
info - 0b00
warning - 0b01
error - 0b10
*/

char* errorCode[] =
{
	"Succesful (Function: %s)",
	"Not enough arguments (Function: %s | Arguments needed: %d)",
	"Too much arguments (Function: %s | Arguments needed: %d)",
	"Invalid argument (Function: %s | Arguments needed: %d)",
	"File missing (Function: %s)",
	"Function not found (Function: %s)"
};

//    error code		responeType     	print line
//0b  000   			00					0


#define Respone_SilentType 0
#define Respone_InfoType 1
#define Respone_WarningType 2
#define Respone_ErrorType 3

#define Respone_Success 0
//1
//2
//3

#define Respone(errorCode, responeType, printLine) (errorCode<<3)+(responeType<<1)+printLine


extern SDL_Surface* line_surface;
extern SDL_Renderer* line_renderer;
extern int* win_size;

extern SDL_Color* bg_color;
extern Uint32 line_fr_color;
int line_size = 1;
bool draw_line = 1;

extern ext_t EXT;
target_t TARGETS;

//terminal output
char* output;

//MAP FOR VARIABLES
map_str_t variables;


void thickLineColor(SDL_Renderer *renderer, int sx, int sy, int ex, int ey, int size, Uint32 color){
	Uint8 *c = (Uint8 *)&color;
	SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);

	//for(int i=0; i<size; i++){
		SDL_RenderDrawLine(renderer, sx, sy, ex, ey);
	//}
}



Uint32 getColor(char* name){
	char* names[] = {
		"biały",
		"czarny",
		"czerwony",
		"zielony",
		"niebieski",
		"żółty",
		"pomarańczowy"
	};
	const int color_size = sizeof(names)/sizeof(char*);

	const int values[] = {
		//A  B  G  R
		0xFFFFFFFF,
		0xFF000000,
		0xFF0000FF,
		0xFF00FF00,
		0xFFFF0000,
		0xFF00FFFF,
		0xFF00A5FF
	};

	Uint32 ret;

	if(strcmp(name, "random") == 0){
		return values[rand() % color_size];
	}

	for(int i=0; i<color_size; i++){
		if(strcmp(names[i], name) == 0){
			return values[i];
		}
	}
}

//clears strtok
void _fflush_strtok(const char* delim){
	char* _tmp = strtok(NULL, delim);
	while(_tmp != NULL) _tmp = strtok(NULL, delim);
}

//clear string (str) from first (first) and last (last) symbol
void _clear_argument(char* str, char first, char last){
	if(str[strlen(str)-1] == last){
		str[strlen(str)-1] = '\0';
	}
	if(str[0] == first){
		char *ps;
		for (ps = str; *ps != '\0'; ps++)
    		*ps = *(ps+1);
		*ps = '\0';
	}
}

void _func_run(FILE* DEF_FUNC){
	char line[128] = {0};
	
	while (fgets(line, 128, DEF_FUNC)){
		Function* line_func = calloc(1, sizeof(Function));
		
		strtok(line, "\n");
		_fflush_strtok("\n");

		CMDT(line, line_func);

		printf("DEBUG\n");
		printf("Arguments: %d\n", line_func->arg_size);
		printf("Function name: %s\n", line_func->function);

		Execute(line_func);
	}
	fclose(DEF_FUNC);
}

//Replace function name by func->arguments[index]
//and trim previous than index+1 arguments 
void _replace_func(Function* func, int index, int arg_size){
	strcpy(func->function, func->arguments[index]);
	func->arguments += index+1;
	func->arg_size = arg_size;
}

//Execute bracket string (str) as command
void _bracket_multi(char* str){
	_clear_argument(str, '[', ']');

	//func->arguments[0] - nawias
	Function* inside = calloc(1, sizeof(Function));

	//funkcją jest pierwszy argument - kolejne funkcje i argumenty
	//są zapisywane jako argumenty owej funkcji
	CMDT(str, inside);

	int args_left = inside->arg_size /*- 1*/;
	int args_used = 0;

	printf("%d\n", args_left);

	while(args_left > 0){

		args_used = Execute(inside);
		args_left = inside->arg_size - args_used - 1;

		_replace_func(inside, args_used, args_left);
	}
	
}

void move(int steps){
	for(int i=0; i<TARGETS.indexes_size; i++){
		struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[i]]);

		object->position.x = object->position.x + steps * cos(*(double*)object->ext.values[EXT.ROTATION] * M_PI / 180.0);
		object->position.y = object->position.y + steps * sin(*(double*)object->ext.values[EXT.ROTATION] * M_PI / 180.0);
	}
}
void rotate(double degree){
	for(int i=0; i<TARGETS.indexes_size; i++){
		struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[i]]);

		double rotation = (*(double*)(object->ext.values[EXT.ROTATION])) + degree;
		memcpy(object->ext.values[EXT.ROTATION], &rotation, sizeof(double));
	}
}

//przesuń w przód
int np(Function* func){
	if(func->arg_size < 1) return 1;
	//if(func->arg_size > 1) return 2;

	int steps = atoi(func->arguments[0]);
	move(steps);
	return Respone(Respone_Success, Respone_InfoType, 1);
}

//przesuń w tył
int ws(Function* func){
	if(func->arg_size < 1) return 1;
	//if(func->arg_size > 1) return 2;

	int steps = -atoi(func->arguments[0]);
	move(steps);
	return Respone(Respone_Success, Respone_InfoType, 1);
}

//obróć w prawo
int pw(Function* func){
	if(func->arg_size < 1) return 1;
	//if(func->arg_size > 1) return 2;

	double degree = atof(func->arguments[0]);
	//printf("\n\nDEGREE: %s\n", func->arguments[0]);
	rotate(degree);
	return Respone(Respone_Success, Respone_InfoType, 0);
}

//obróć w lewo
int lw(Function* func){
	if(func->arg_size < 1) return 1;
	//if(func->arg_size > 1) return 2;

	double degree = -atof(func->arguments[0]);
	rotate(degree);
	return Respone(Respone_Success, Respone_InfoType, 0);
}

//czyść ekran
int cs(Function* func){
	SDL_SetRenderDrawColor(line_renderer, 1, 0xFF, 1, 0);
	SDL_RenderClear(line_renderer);
	return Respone(Respone_Success, Respone_SilentType, 0);
}

//wywołuje funkcje
int call(Function* func){
	if(func->arg_size < 1) return 1;

	const char* DEF_FUNC_path = "../LEVELS/cmdToken/DEF_FUNC/"; 
	const char* table_path = "../LEVELS/cmdToken/DEF_FUNC/functions.table";

	FILE* table = fopen(table_path, "r");
	if(table == NULL) return -2;

	char line[128] = {0};

	while (fgets(line, 128, table))
    {
		if(strcmp(strtok(line, " "), func->arguments[0]) == 0){
			char* arg_names = calloc(sizeof(char), 128);
			strcpy(arg_names, strtok(NULL, " "));
			char* file_name = calloc(sizeof(char), 128);
			strcpy(file_name, strtok(NULL, " "));


			_fflush_strtok(" ");
			strtok(file_name, "\n");
			_fflush_strtok("\n");


			char file_path[] = "../LEVELS/cmdToken/DEF_FUNC/";
			strcat(file_path, file_name);	

			printf("full path to function: '%s'\n", file_path);	

			FILE* DEF_FUNC = fopen(file_path, "r");
			if(DEF_FUNC == NULL) return -2;

			_clear_argument(arg_names, '[', ']');

			char* arg_name = strtok(arg_names, ",");

			//skipping name of function
			for(int i=1; i<func->arg_size; i++){
				if(arg_name != NULL){
					map_set(&variables, arg_name, func->arguments[i]);
				}
				arg_name = strtok(NULL, ",");
			}

			_func_run(DEF_FUNC);

			break;
		}
    }

	fclose(table);
	return 0;
}

//wyznacza obiekty domyślne. Dodatkowo nadaje obiektowo rozszerzenie. TODO tylko jeden index
int cel(Function* func){
	if(func->arg_size < 1) return 1;

	int *indexes = calloc(func->arg_size, sizeof(int));
	for(int i=0; i<func->arg_size; i++){
		int index = atoi(func->arguments[i]);

		if(index < TARGETS.objects_size && index >= 0){
			indexes[i] = index;
			TARGETS.objects[index].ext.values = calloc(2, sizeof(double*));
			TARGETS.objects[index].ext.size = 2;
			TARGETS.objects[index].ext.values[EXT.VISIBILITY] = (void*)1;

			TARGETS.objects[index].ext.values[EXT.ROTATION] = malloc(sizeof(double));
			double rotation = 270.0F;
			memcpy(TARGETS.objects[index].ext.values[EXT.ROTATION], &rotation, sizeof(double));
		}
		else return 3;
		
	}
	TARGETS.indexes = indexes;
	TARGETS.indexes_size = func->arg_size;

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//powtarza polecenia zapisane w nawiasie
int repeat(Function* func){
	if(func->arg_size < 2) return 1;
	
	//powtórz func->arguments[0] (N) razy
	for(int i=0; i<atoi(func->arguments[0]); i++){ 
		_bracket_multi(func->arguments[1]);
	}

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//wraca obiekt domyślny do swojej pozycji początkowej i kierunku początkowego
int revert(Function* func){

	for(int i=0; i<TARGETS.indexes_size; i++){
		struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[i]]);

		double rotation = 270.0F;
		memcpy( (*object).ext.values[EXT.ROTATION], &rotation, sizeof(double));
		object->position.x = 0;
		object->position.y = 0;
	}
	
	
	return Respone(Respone_Success, Respone_InfoType, 0);
}

//ukrywa obiekt domyślny
int hide(Function* func){
	for(int i=0; i<TARGETS.indexes_size; i++){
		struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[i]]);

		object->ext.values[EXT.VISIBILITY] = 0;
	}

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//pokazuje obiekt domyślny
int show(Function* func){
	for(int i=0; i<TARGETS.indexes_size; i++){
		struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[i]]);

		object->ext.values[EXT.VISIBILITY] = (void*)1;
	}

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//ustala kolor pisaka
int ukp(Function* func){
	if(func->arg_size < 1) return 1;

	char* color_name = func->arguments[0];

	line_fr_color = getColor(color_name);
	//memcpy(line_fr_color, &color, sizeof(SDL_Color));

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//ustala szerokość pisaka
int ugp(Function* func){
	if(func->arg_size < 1) return 1;

	line_size = atoi(func->arguments[0]);

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//podnieś pisak
int pod(Function* func){
	draw_line = 0;
	return Respone(Respone_Success, Respone_InfoType, 0);
}

//opuść pisak
int opu(Function* func){
	draw_line = 1;
	return Respone(Respone_Success, Respone_InfoType, 0);
}

//funkcja pozwala na wykonanie wielu funkcji zapisanych w nawiasach
//jako argument
int multi(Function* func){
	if(func->arg_size < 1) return 1;

	_bracket_multi(func->arguments[0]);

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//funkcja otrzymuje 4 argumenty:
//x, operator, y, polecenie

//x - wartość do porównania
//operator - operator porównujący x oraz y
//y - wartość do porównania
//polecenie - polecenie (zapisane w nawiasach) wykonywane jeżeli wyjście jest równe prawdziwe
int ifcmd(Function* func){
	char* x = func->arguments[0];
	char* oper = func->arguments[1];
	char* y = func->arguments[2];
	char* then = func->arguments[3];

	double x_f = atof(x);
	double y_f = atof(y);

	const bool result[] = {
		x_f<y_f,
		x_f<=y_f,

		x_f>y_f,
		x_f>=y_f,
		
		x_f==y_f,

		x_f!=y_f
	};
	const int result_size = sizeof(result)/sizeof(result[0]);

	const char* operations[] = {
		"<",
		"<=",
		
		">",
		">=",
		
		"==",

		"!="
	};

	for(int i=0; i<result_size; i++){
		if(strcmp(operations[i], oper) == 0){
			if(result[i]){
				_bracket_multi(then);
			}
		}
	}

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//ustawia zmienną globalną
int let(Function* func){
	char* key = func->arguments[0];
	char* value = func->arguments[1];

	map_set(&variables, key, value);
	return Respone(Respone_Success, Respone_InfoType, 0);
}

//funkcja otrzymuje 4 argumenty:
//x, operator, y, zmienna

//x - wartość pierwsza
//operator - operator arytmetyczny
//y - wartość druga
//zmienna - nazwa zmiennej do której ma zostać zapisany wynik działania
int math(Function* func){
	char* x = func->arguments[0];
	char* oper = func->arguments[1];
	char* y = func->arguments[2];
	char* key = func->arguments[3];

	double x_f = atof(x);
	double y_f = atof(y);

	const double result[] = {
		x_f+y_f,
		x_f-y_f,
		x_f*y_f,
		x_f/y_f,
		fmod(x_f, y_f)
	};
	const int result_size = sizeof(result)/sizeof(result[0]);

	const char* operations[] = {
		"+", "-", "*", "/", "%"
	};

	for(int i=0; i<result_size; i++){
		if(strcmp(operations[i], oper) == 0){
			char* value = calloc(32, sizeof(char));
			sprintf(value, "%f", result[i]);
			printf("VALUE: %s\n", value);
			map_set(&variables, key, value);
		}
	}

	return Respone(Respone_Success, Respone_InfoType, 0);
}

//debug tool TODO
int debug(Function* func){
	char* option = func->arguments[0];

	//print all variables in map
	if(strcmp(option, "variables") == 0){
		printf("TEST\n");
		const char *key;
		map_iter_t iter = map_iter(&m);

		key = map_next(&variables, &iter);

		while (key != NULL) {
			printf("%s -> %s", key, *map_get(&variables, key));
			key = map_next(&variables, &iter);
		}
	}	

	return Respone(Respone_Success, Respone_InfoType, 0);
}



int Execute(Function* func){
	char* cmds_names[] = {
		"np",
		"ws",
		"pw",
		"lw",
		"cs",
		"cel",
		"powtórz",
		"wróć",
		"sż",
		"pż",
		"ukp",
		"ugp",
		"pod",
		"opu",
		"multi",
		"wezwij",
		"jeżeli",
		"niech",
		"math",
		"debug"
	};
	const int cmds_size = sizeof(cmds_names)/sizeof(char*);
	int (*cmds_funcs[])() = {
		np,
		ws,
		pw,
		lw,
		cs,
		cel,
		repeat,
		revert,
		hide,
		show,
		ukp,
		ugp,
		pod,
		opu,
		multi,
		call,
		ifcmd,
		let,
		math,
		debug
	};
	unsigned int arguments_size[] = {
		1,
		1,
		1,
		1,
		0,
		1,
		2,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		1,
		1,
		4,
		2,
		4,
		1
	};
	for(int i=0; i<cmds_size; i++){
		if(strcmp(cmds_names[i], func->function) == 0){

			POINT start[TARGETS.indexes_size];
			POINT end;

			int preIndexes = TARGETS.indexes_size;

			for(int j=0; j<TARGETS.indexes_size; j++){
				struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[j]]);
				start[j] = object->position;
			}

			char responeCode;

			//not enough args
			if(func->arg_size < arguments_size[i]){
				responeCode = Respone(1, Respone_ErrorType, 0);	
			}
			else{
				//iterate through arguments, that are used by function
				for(int y=0; y<arguments_size[i]; y++){

					//check if argument is variable
					if(func->arguments[y][0] == '$'){
						
						//get variable from name
						char** val = map_get(&variables, (func->arguments[y]+1) );
						if (val) {
							strcpy(func->arguments[y], *val);
						} else {
							printf("value not found\n");
						}

					}
				}

				responeCode = (*cmds_funcs[i])(func);	
			}

			if(draw_line && (responeCode & 1) ){
				for(int j=0; j<preIndexes; j++){
					struct OBJECT_Moveable* object = &(TARGETS.objects[TARGETS.indexes[j]]);

					end = object->position;

					thickLineColor(line_renderer,
						start[j].x + win_size[0]/2,
						start[j].y + win_size[1]/2, 
						end.x + win_size[0]/2,
						end.y + win_size[1]/2,
						line_size, line_fr_color);
					
				}
			}

			//output
			sprintf(output, errorCode[responeCode >> 3], cmds_names[i], arguments_size[i]);
			
			//return amount of arguments used by function
			return arguments_size[i];
		}
	}
	//command not found
	sprintf(output, errorCode[5], func->function);
	return -1;
}


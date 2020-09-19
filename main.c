#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include "./LEVEL/loadProject.h"
#include "./jsmn/jsmn.h"

//global variables
SDL_Window* m_window = NULL;
SDL_Renderer* m_renderer = NULL;
int* win_size = NULL;

SDL_Surface* line_surface = NULL;
SDL_Renderer* line_renderer = NULL;
Uint32 line_fr_color;

int close_window;
//

//draw line surface
void Line_RenderCopy(){
	SDL_Texture* tmp = SDL_CreateTextureFromSurface(m_renderer, line_surface);
	SDL_RenderCopy(m_renderer, tmp, NULL, NULL);
	SDL_DestroyTexture(tmp);
}
//

static void addChar(char* str, char add){
        int length = strlen(str);
        str[length] = add;
        str[length+1] = '\0';
};

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void parseJson(char* json_str, jsmn_parser *json_parser, jsmntok_t* tokens, int* n_tokens){

	(*n_tokens) = jsmn_parse(json_parser, json_str, strlen(json_str), tokens, 256);
	// n will now contain number of parsed tokens or a negative
	// error value
}

void getJsonValue(char* json_str, jsmntok_t *tokens, int n, char* key, char* ret){
	for(int i=1; i<n; i++){
		//ret as string
		if(jsoneq(json_str, &tokens[i], key) == 0){
			sprintf(ret, "%.*s", tokens[i + 1].end - tokens[i + 1].start, json_str + tokens[i + 1].start);
		}
	}
}

void loadFileLine(char* filepath, char* ret, int size){
    FILE *fptr;
    if ((fptr = fopen(filepath, "r")) == NULL) {
        printf("Error with opening file!\n");
        exit(1);
    }

	fgets(ret, size, fptr);

    fclose(fptr);
}



int main(int argc, char *argv[]){

	//PRE JSON
	char* w_title = calloc(128, sizeof(char));
	
	SDL_Color* bg_color = malloc(sizeof(SDL_Color));
	bg_color->r = 0;
	bg_color->g = 0;
	bg_color->b = 0;
	
	//

	//load settings
	char* json_setting_filepath = argv[1];

	char* setting_json_line = calloc(1024, sizeof(char));
	loadFileLine(json_setting_filepath, setting_json_line, 1024);

	jsmntok_t tokens[256];
	
	jsmn_parser json_parser;
	jsmn_init(&json_parser);

	int n;

	parseJson(setting_json_line, &json_parser, tokens, &n);


	for(int i=1; i<n; i++){
		if(tokens[i].type != JSMN_STRING) continue;

		if(jsoneq(setting_json_line, &tokens[i], "project-name") == 0){
			//get window title
			sprintf(w_title, "%.*s", tokens[i + 1].end - tokens[i + 1].start, setting_json_line + tokens[i + 1].start);
		}
		else if(jsoneq(setting_json_line, &tokens[i], "background-color") == 0){
			char r[4];
			char g[4];
			char b[4];

			sprintf(r, "%.*s", tokens[i + 2].end - tokens[i + 2].start, setting_json_line + tokens[i + 2].start);
			sprintf(g, "%.*s", tokens[i + 3].end - tokens[i + 3].start, setting_json_line + tokens[i + 3].start);
			sprintf(b, "%.*s", tokens[i + 4].end - tokens[i + 4].start, setting_json_line + tokens[i + 4].start);

			bg_color->r = atoi(r);
			bg_color->g = atoi(g);
			bg_color->b = atoi(b);
		}
		/*else if(jsoneq(setting_json_line, &tokens[i], "objects") == 0){
			printf("===========\n");
			//printf("%.*s\n", tokens[i + 1].end - tokens[i + 1].start, setting_json_line + tokens[i + 1].start);
		
			int j = 2;
			while(tokens[i + j].start < tokens[i + 1].end){
				if(tokens[i+j].type == JSMN_STRING){
					printf("%.*s\n", tokens[i + j].end - tokens[i + j].start, setting_json_line + tokens[i + j].start);
					if (jsoneq(setting_json_line, &tokens[i+j], "origin-position") == 0 ){

					}
					else if(jsoneq(setting_json_line, &tokens[i+j], "sprite") == 0){

					}
					else if(jsoneq(setting_json_line, &tokens[i+j], "sprite-size") == 0){

					}
				}
				

				j++;
			}
		}*/
	}
	//
	

	//init graphics
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	//

	//random seed
	srand(time(NULL));

	//declare window size
	win_size = calloc(2, sizeof(int));
	win_size[0] = 1600;
	win_size[1] = 900;
	//

	m_window = SDL_CreateWindow(w_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_size[0], win_size[1], SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	//declare color, surface and renderer for line
	line_surface = SDL_CreateRGBSurface(0, win_size[0], win_size[1], 32, 0, 0, 0, 0);
	//make color green (1, 0xFF, 1) as transparent
    SDL_SetColorKey(line_surface, SDL_TRUE, SDL_MapRGB(line_surface->format, 1, 0xFF, 1));

	line_renderer = SDL_CreateSoftwareRenderer(line_surface);

	SDL_SetRenderDrawColor(line_renderer, 1, 0xFF, 1, 0);
	SDL_RenderClear(line_renderer);

	line_fr_color = 255 << 24 | 255 << 16 | 255 << 8 | 255;
	//

	//check renderer name
	SDL_RendererInfo* m_renderer_info = (SDL_RendererInfo*)malloc(sizeof(SDL_RendererInfo));
	SDL_GetRendererInfo(m_renderer, m_renderer_info);

	printf("%s\n", m_renderer_info->name);
	//


	SDL_Event event;
	
	//check for any SDL errors
    printf("%s\n", SDL_GetError());	

	//Setup project
	SetupProject(&event);
	//

	close_window = 0;

    const int fps = 60;
    const int frame_delay = 1000 / fps;

	while(close_window == 0){

		int fStart = (int)SDL_GetTicks();

		SDL_SetRenderDrawColor(m_renderer, bg_color->r, bg_color->g, bg_color->b, 0);
		SDL_RenderClear(m_renderer);

        RunProject();

		Line_RenderCopy();
		
		SDL_RenderPresent(m_renderer);

        int fTime = SDL_GetTicks() - fStart;
        if(frame_delay > fTime){
        	SDL_Delay(frame_delay - fTime);
        }
	}

	//clean up
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyRenderer(line_renderer);

	SDL_Quit();
	TTF_Quit();
	return 0;
}












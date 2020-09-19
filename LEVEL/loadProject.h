#include "../OBJECTS/objects.h"
#include "SDL2/SDL_ttf.h"
#include "./cmdToken/tokenizer/tokenizer.h"
#include "./cmdToken/execute/execute.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifndef _NRML_H_
#define _NRML_H_

void SetupProject(SDL_Event *event_ptr);
void RunProject();

typedef struct{

    struct {
        char command[256];
        struct OBJECT_Text command_line_text;
        struct OBJECT_Text output_text;
        SDL_Window *term_window;
        SDL_Renderer *term_renderer;
        bool enter_command;
    } TERMINAL;

    struct {
        int size_x;
        int size_y;
    } CANVAS;

    struct {
        float zoom;
        float distance;
        bool **field;
    } FIELD;

    struct {
        int size;
        struct OBJECT_Moveable *objects;
    } OBJECTS;

} example_t;

extern example_t EXAMPLE;

#endif

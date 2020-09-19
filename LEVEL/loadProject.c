#include "./loadProject.h"

//local pointer to global variable event 
SDL_Event* event;

//global variables
extern SDL_Window *m_window;
extern SDL_Renderer *m_renderer;
extern int close_window;
extern int *win_size;
//

example_t EXAMPLE;


//functions
void zoom(float value){
    EXAMPLE.FIELD.zoom *= value;
}

/*void drawGrid(){
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    int linie_x = Application.win_size[1]/EXAMPLE.FIELD.zoom*EXAMPLE.FIELD.distance;

    for(int y=0; y<linie_x+1; y++){
        SDL_RenderDrawLine(m_renderer, 
            0, EXAMPLE.FIELD.distance*EXAMPLE.FIELD.zoom*y,
            Application.win_size[0], EXAMPLE.FIELD.distance*EXAMPLE.FIELD.zoom*y);
    }

    int linie_y = Application.win_size[0]/EXAMPLE.FIELD.zoom*EXAMPLE.FIELD.distance;

    for(int x=0; x<linie_y+1; x++){
        SDL_RenderDrawLine(m_renderer, 
            EXAMPLE.FIELD.distance*EXAMPLE.FIELD.zoom*x, 0,
            EXAMPLE.FIELD.distance*EXAMPLE.FIELD.zoom*x, Application.win_size[1]);
    }
}*/

//

//TERMINAL

//setup terminal
void SetupTerminal(){
    EXAMPLE.TERMINAL.term_window = SDL_CreateWindow("Terminal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 360, SDL_WINDOW_SHOWN);
    EXAMPLE.TERMINAL.term_renderer = SDL_CreateRenderer(EXAMPLE.TERMINAL.term_window, -1, SDL_RENDERER_ACCELERATED);

    SET_OBJECT_Text(&EXAMPLE.TERMINAL.command_line_text, "../SPRITES/fonts/AverBold.ttf", 16,
    (SDL_Color){0, 200, 0}, "COMMAND_LINE", (POINT){0, 0}, EXAMPLE.TERMINAL.term_renderer, false);
    
    int w, h;
    SDL_GetWindowSize(EXAMPLE.TERMINAL.term_window, &w, &h);

    SET_OBJECT_Text(&EXAMPLE.TERMINAL.output_text, "../SPRITES/fonts/Amethysta-Regular.ttf", 16,
    (SDL_Color){0, 255, 100}, "OUTPUT_LINE", (POINT){0, h-20}, EXAMPLE.TERMINAL.term_renderer, false);

    EXAMPLE.TERMINAL.command_line_text.text = calloc(256, sizeof(wchar_t));
    //EXAMPLE.TERMINAL.output_text.text = calloc(256, sizeof(char));
}
//

//sent command
void CommandTerminal(){
    if(EXAMPLE.TERMINAL.enter_command){
        EXAMPLE.TERMINAL.enter_command = false;
        char commandline[256];
        strcpy(commandline, EXAMPLE.TERMINAL.command); 
        //swprintf(commandline, 256, L"%hs", EXAMPLE.TERMINAL.command);
        //sprintf(commandline, "%s", EXAMPLE.TERMINAL.command);
        memset(EXAMPLE.TERMINAL.command, 0, sizeof(EXAMPLE.TERMINAL.command));

        //printf("%ls\n", commandline);

        Function *function = calloc(1, sizeof(Function));

        CMDT(commandline, function);
        printf("%s\n", function->function);
        for(int i=0; i<function->arg_size; i++){
            printf("%d. %s\n", i+1, function->arguments[i]);
        }
        
        TARGETS.objects = EXAMPLE.OBJECTS.objects;
        TARGETS.objects_size = EXAMPLE.OBJECTS.size;
        //TARGETS.win_size = Application.win_size;

        Execute(function);

        //view output
        EXAMPLE.TERMINAL.output_text.text = output;
        UPDATE_OBJECT_Text_UTF8(&EXAMPLE.TERMINAL.output_text);
    }
}
//

//looped calling of this function
void RunTerminal(){
    CommandTerminal();
    strcpy(EXAMPLE.TERMINAL.command_line_text.text, EXAMPLE.TERMINAL.command);

    UPDATE_OBJECT_Text_UTF8(&EXAMPLE.TERMINAL.command_line_text);

    int w, h;
    SDL_GetWindowSize(EXAMPLE.TERMINAL.term_window, &w, &h);

    /*UPDATE_OBJECT_Text(&EXAMPLE.TERMINAL.output_text, output, EXAMPLE.TERMINAL.term_renderer,
    (POINT){0, h-20}, "../SPRITES/fonts/Amethysta-Regular.ttf", 16, false);*/

    SDL_SetRenderDrawColor(EXAMPLE.TERMINAL.term_renderer, 0, 0, 0, 0);
	SDL_RenderClear(EXAMPLE.TERMINAL.term_renderer);

    DisplayObj(EXAMPLE.TERMINAL.term_renderer, 
        EXAMPLE.TERMINAL.command_line_text.surface, &EXAMPLE.TERMINAL.command_line_text.dstrect);

    DisplayObj(EXAMPLE.TERMINAL.term_renderer, 
        EXAMPLE.TERMINAL.output_text.surface, &EXAMPLE.TERMINAL.output_text.dstrect);

    SDL_RenderPresent(EXAMPLE.TERMINAL.term_renderer);
}
//

//




//SETUP

void SetupProject(SDL_Event *event_ptr){

    //for polish accent
    setlocale(LC_ALL, "pl_PL");

    //set pointers
    event = event_ptr;
    //

    //set objects LOAD FROM JSON
    EXAMPLE.OBJECTS.size = 1;
    
    EXAMPLE.OBJECTS.objects = (struct OBJECT_Moveable*) malloc(EXAMPLE.OBJECTS.size*sizeof(struct OBJECT_Moveable));

    SET_OBJECT_Moveable(&EXAMPLE.OBJECTS.objects[0], (POINT){0, 0}, (POINT){0, 0}, 
    1, 270, SDL_LoadBMP("../SPRITES/krab.bmp"), 
    RET_Rect(0, 0, 48, 48, 0), 0);
    //

    EXAMPLE.FIELD.distance = 48;
    EXAMPLE.FIELD.zoom = 1;

    map_init(&variables);

    output = calloc(128, sizeof(char));

    SetupTerminal();
    SDL_StartTextInput();
    
}

//RUN
void RunProject(){
    while(SDL_PollEvent(event)){
        switch(event->type){
            case SDL_KEYDOWN:
                switch(event->key.keysym.sym){
                    case SDLK_1:
                        //SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
                        break;
                    case SDLK_0:
                        //SDL_SetWindowFullscreen(m_window, 0);
                        break;
                    case SDLK_MINUS:
                        //zoom(0.5);
                        break;
                    case SDLK_EQUALS:
                        //zoom(2);
                        break;
                    case SDLK_BACKSPACE:
                        if(strlen(EXAMPLE.TERMINAL.command) > 0){
                            int fill2 = EXAMPLE.TERMINAL.command[strlen(EXAMPLE.TERMINAL.command)-2];
                            int fill1 = EXAMPLE.TERMINAL.command[strlen(EXAMPLE.TERMINAL.command)-1];
                            int remove = 1;
                            if(fill2 < 0 && fill1 < 0) remove = 2;

                            EXAMPLE.TERMINAL.command[strlen(EXAMPLE.TERMINAL.command)-remove] = '\0';
                        }
                        break;
                    case SDLK_RETURN:
                        if(!EXAMPLE.TERMINAL.enter_command) EXAMPLE.TERMINAL.enter_command = true;
                        break;
                }
                break;
            case SDL_QUIT:
                close_window = 1;
                break;
            case SDL_WINDOWEVENT:
                switch(event->window.event){
                    case SDL_WINDOWEVENT_CLOSE:
                        if(event->window.windowID == SDL_GetWindowID(EXAMPLE.TERMINAL.term_window)){
                            SDL_DestroyWindow(EXAMPLE.TERMINAL.term_window);
                        }
                        if(event->window.windowID == SDL_GetWindowID(m_window))
                            close_window = 1;
                        break;
                }            
                break;

            //KEYBOARD
            case SDL_TEXTINPUT:
                if(strlen(EXAMPLE.TERMINAL.command) < 256){ 
                    if(
                        strcmp(event->text.text, "ą") == 0 ||
                        strcmp(event->text.text, "ć") == 0 ||
                        strcmp(event->text.text, "ę") == 0 ||
                        strcmp(event->text.text, "ł") == 0 ||
                        strcmp(event->text.text, "ń") == 0 ||
                        strcmp(event->text.text, "ó") == 0 ||
                        strcmp(event->text.text, "ś") == 0 ||
                        strcmp(event->text.text, "ź") == 0 ||
                        strcmp(event->text.text, "ż") == 0 ||
                        (event->text.text[0] >= 0 &&
                        event->text.text[0] <= 127)
                    ){
                        strcat(EXAMPLE.TERMINAL.command, event->text.text);
                    }
                    
                }
                break;
        }

    }

    //drawGrid();

    for(int i=0; i<EXAMPLE.OBJECTS.size; i++){
        if(EXAMPLE.OBJECTS.objects[i].ext.size){
            if(EXAMPLE.OBJECTS.objects[i].ext.values[EXT.VISIBILITY] == 0) continue;
        }
        SDL_Rect tmpDstrect = EXAMPLE.OBJECTS.objects[i].dstrect;
        tmpDstrect.w *= EXAMPLE.FIELD.zoom;
        tmpDstrect.h *= EXAMPLE.FIELD.zoom;
        tmpDstrect.x = EXAMPLE.OBJECTS.objects[i].position.x + win_size[0]/2 - tmpDstrect.w/2;
        tmpDstrect.y = EXAMPLE.OBJECTS.objects[i].position.y + win_size[1]/2 - tmpDstrect.h/2;
        DisplayObj(m_renderer, EXAMPLE.OBJECTS.objects[i].sprite, &tmpDstrect);
    }

    if(SDL_GetWindowID(EXAMPLE.TERMINAL.term_window))
        RunTerminal();
}
//
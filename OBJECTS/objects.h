#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"

#ifndef _OBJ_H_
#define _OBJ_H_

typedef struct{
    const int ROTATION;
    const int VISIBILITY;
} ext_t;

extern ext_t EXT;

void DisplayObj(SDL_Renderer* renderer, SDL_Surface* surf, SDL_Rect* dstrect);
void DisplayObjs(SDL_Renderer* renderer, int size, ...);

typedef struct struct_point {
    int x;
    int y;
} POINT;
POINT RET_Point(int x, int y);
SDL_Rect RET_Rect(int x, int y, int w, int h, bool center);

struct OBJECT_Static {
    POINT position;
    POINT size;
    bool breakable;
    bool blocking;
    SDL_Surface *sprite;
    SDL_Rect srcrect;
    SDL_Rect dstrect;
    struct{
        void** values;
        int size;
    }ext;
};

void SET_OBJECT_Static(struct OBJECT_Static *obj, POINT position, POINT size,
                        bool breakable, bool blocking, SDL_Surface *sprite, SDL_Rect dstrect,
                        bool specify_srcrect, ...);



struct OBJECT_Moveable{
    POINT position;
    POINT size;
    bool movement[2];
    unsigned int speed;
    unsigned int gravity;
    SDL_Surface *sprite;
    SDL_Rect srcrect;
    SDL_Rect dstrect;
    struct{
        void** values;
        int size;
    }ext;
};
void SET_OBJECT_Moveable(struct OBJECT_Moveable *obj, POINT position, POINT size,
                        unsigned int speed, unsigned int gravity, SDL_Surface *sprite, SDL_Rect dstrect,
                        bool specify_srcrect, ...);

struct OBJECT_Text{
    char* fontPath;
    unsigned int fontSize;
    char* text;
    bool onCenter;
    //TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color color;
    SDL_Rect dstrect;
    SDL_Surface* surface;
    struct{
        void** values;
        int size;
    }ext;
};
void SET_OBJECT_Text(struct OBJECT_Text *obj, char* fontPath, unsigned int fontSize,
                    SDL_Color fontColor, char* text, POINT dst_pos,
                    SDL_Renderer* renderer, bool center);
void UPDATE_OBJECT_Text(struct OBJECT_Text *obj);
void UPDATE_OBJECT_Text_UTF8(struct OBJECT_Text *obj);
//void SET_GROUP(int elements, ...);

#endif
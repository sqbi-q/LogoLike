#include "./objects.h"

ext_t EXT = {0, 1};

void DisplayObj(SDL_Renderer* renderer, SDL_Surface* surf, SDL_Rect* dstrect){
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    //SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture, NULL, dstrect);
    SDL_DestroyTexture(texture);
}
void DisplayObjs(SDL_Renderer* renderer, int size, ...){

    va_list varg;
    SDL_Rect* dstrects = calloc(size, sizeof(SDL_Rect));

    va_start(varg, size);

    for(int i=0; i<size; i++){
        dstrects[i] = va_arg(varg, SDL_Rect);
    }

    for(int i=0; i<size; i++){
        SDL_Surface* surf = va_arg(varg, SDL_Surface*);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
        
        SDL_RenderCopy(renderer, texture, NULL, &dstrects[i]);

        SDL_DestroyTexture(texture);
    }

    va_end(varg);


}

POINT RET_Point(int x, int y){
    POINT tmp = {x, y};
    return tmp; 
}
SDL_Rect RET_Rect(int x, int y, int w, int h, bool center){
    SDL_Rect tmp;
    tmp = (center) ? (SDL_Rect){x-w/2, y-h/2, w, h} : (SDL_Rect){x, y, w, h};
    return tmp;
}


void SET_OBJECT_Static(struct OBJECT_Static *obj, POINT position, POINT size,
    bool breakable, bool blocking, SDL_Surface *sprite, SDL_Rect dstrect, bool specify_srcrect,
     ...){
        obj->position = position;
        obj->size = size;
        obj->breakable = breakable;
        obj->blocking = blocking;
        obj->sprite = sprite;

        
        obj->dstrect = dstrect;
        
        if(specify_srcrect){
            va_list varg;
            va_start(varg, (int)specify_srcrect);
            obj->srcrect = va_arg(varg, SDL_Rect);
            va_end(varg);
        }
}   

void SET_OBJECT_Moveable(struct OBJECT_Moveable *obj, POINT position, POINT size,
    unsigned int speed, unsigned gravity, SDL_Surface *sprite, SDL_Rect dstrect, bool specify_srcrect,
     ...){
        obj->position = position;
        obj->size = size;
        obj->movement[0] = 0;
        obj->movement[1] = 0;
        obj->speed = speed;
        obj->gravity = gravity;
        obj->sprite = sprite;

        
        obj->dstrect = dstrect;
        
        if(specify_srcrect){
            va_list varg;
            va_start(varg, (int)specify_srcrect);
            obj->srcrect = va_arg(varg, SDL_Rect);
            va_end(varg);
        }

        obj->ext.values = 0;
        obj->ext.size = 0;
}       

void _setup_text(struct OBJECT_Text *obj, char* fontPath, unsigned int fontSize,
                    SDL_Color fontColor, char* text, POINT dst_pos,
                    SDL_Renderer* renderer, bool center){

    obj->fontPath = malloc(strlen(fontPath)*sizeof(char));
    strcpy(obj->fontPath, fontPath);
    obj->fontSize = fontSize;
    obj->color = fontColor;
    obj->renderer = renderer;
    obj->onCenter = center;

    SDL_Rect dstrect = {0,0,0,0};

    SDL_QueryTexture(SDL_CreateTextureFromSurface(obj->renderer, obj->surface),
                    NULL, NULL, &dstrect.w, &dstrect.h);
    obj->dstrect.w = dstrect.w;
    obj->dstrect.h = dstrect.h;
    obj->dstrect.x = center ? dst_pos.x-dstrect.w/2 : dst_pos.x;
    obj->dstrect.y = center ? dst_pos.y-dstrect.h/2 : dst_pos.y;

}

void _create_surf_text(struct OBJECT_Text *obj){
    TTF_Font *font = TTF_OpenFont(obj->fontPath, obj->fontSize);

    obj->surface = 
    TTF_RenderText_Blended(font, obj->text, obj->color);

    TTF_CloseFont(font);

}

void _create_utf_8__surf_text(struct OBJECT_Text *obj){
    TTF_Font *font = TTF_OpenFont(obj->fontPath, obj->fontSize);

    //obj->surface = 
    //TTF_RenderUNICODE_Solid(font, (Uint16*)obj->text, obj->color);
    obj->surface =
    TTF_RenderUTF8_Blended(font, obj->text, obj->color);

    TTF_CloseFont(font);

}

void _update_dstrect_text(struct OBJECT_Text *obj){
    SDL_Rect dstrect = {0,0,0,0};

    SDL_QueryTexture(SDL_CreateTextureFromSurface(obj->renderer, obj->surface),
                    NULL, NULL, &dstrect.w, &dstrect.h);
    obj->dstrect.w = dstrect.w;
    obj->dstrect.h = dstrect.h;
    obj->dstrect.x = obj->onCenter ? obj->dstrect.x-dstrect.w/2 : obj->dstrect.x;
    obj->dstrect.y = obj->onCenter ? obj->dstrect.y-dstrect.h/2 : obj->dstrect.y;
}

void SET_OBJECT_Text(struct OBJECT_Text *obj, char* fontPath, unsigned int fontSize,
                    SDL_Color fontColor, char* text, POINT dst_pos,
                    SDL_Renderer* renderer, bool center){

    _setup_text(obj, fontPath, fontSize, fontColor, text, dst_pos, renderer, center);
    //_create_surf_text(obj);
    /*obj->color = fontColor;
    obj->font = TTF_OpenFont(fontName, fontSize);

    
    obj->text = fontText;

    obj->messageSurface = 
    TTF_RenderText_Blended(obj->font, obj->text, obj->color);

    TTF_CloseFont(obj->font);


    SDL_Rect dstrect = {0,0,0,0};

    SDL_QueryTexture(SDL_CreateTextureFromSurface(renderer, obj->messageSurface),
                    NULL, NULL, &dstrect.w, &dstrect.h);
    obj->dstrect.w = dstrect.w;
    obj->dstrect.h = dstrect.h;
    obj->dstrect.x = center ? dst_pos.x-dstrect.w/2 : dst_pos.x;
    obj->dstrect.y = center ? dst_pos.y-dstrect.h/2 : dst_pos.y;*/

}
void UPDATE_OBJECT_Text(struct OBJECT_Text *obj){
    _create_surf_text(obj);
    _update_dstrect_text(obj);
}
void UPDATE_OBJECT_Text_UTF8(struct OBJECT_Text *obj){
    _create_utf_8__surf_text(obj);
    _update_dstrect_text(obj);
}




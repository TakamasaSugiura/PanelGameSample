#pragma once
#include <SDL.h>
#include <stdlib.h>
#include "sdl_extensions.h"

typedef struct
{
    SDL_Rect* rect;
    void (*func)(SDL_MouseButtonEvent*, void*);
    void* parameter;
} Button;

int do_button_action(SDL_MouseButtonEvent* event);
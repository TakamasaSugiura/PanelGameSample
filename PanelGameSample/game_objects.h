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

int RegisterScreenButton(SDL_Rect* src_rect, void (*func)(SDL_MouseButtonEvent*, void*), void* parameter);
int UnregisterScreenButton(int index);
int UnregisterAllScreenButtons();

int DoButtonAction(SDL_MouseButtonEvent* event);
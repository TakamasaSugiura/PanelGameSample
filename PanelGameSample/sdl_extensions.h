#pragma once
#include <SDL.h>

typedef struct
{
    int w;
    int h;
} SDL_Size;

typedef struct
{
    SDL_Surface** surface;
    int length;
} SDL_SurfaceCollection;

SDL_SurfaceCollection* SDL_CreateSurfaceCollection(int length);

void SDL_DeleteSurfaceCollection(SDL_SurfaceCollection* collection);

SDL_Surface* SDL_CreateRGBSurfaceLite(int w, int h);

int SDL_FillAll(SDL_Surface* surface, Uint32 color);

int SDL_PutSurface(SDL_Surface* src, SDL_Point* point, SDL_Surface* dst);

#include "sdl_extensions.h"

SDL_SurfaceCollection* SDL_CreateSurfaceCollection(int length)
{
    SDL_SurfaceCollection* collection = (SDL_SurfaceCollection*)malloc(sizeof(SDL_SurfaceCollection));
    if (collection)
    {
        collection->surface = (SDL_Surface**)calloc(length, sizeof(SDL_Surface*));
        collection->texture = (SDL_Texture**)calloc(length, sizeof(SDL_Texture*));
        collection->length = length;
        if (collection->surface)
        {
            for (int i = 0; i < length; i++)
            {
                collection->surface[i] = NULL;
            }
        }
        if (collection->texture)
        {
            for (int i = 0; i < length; i++)
            {
                collection->texture[i] = NULL;
            }
        }
    }
    return collection;
}

void SDL_DeleteSurfaceCollection(SDL_SurfaceCollection* collection)
{
    for (int i = 0; i < collection->length; i++)
    {
        if (collection->surface[i])
        {
            SDL_FreeSurface(collection->surface[i]);
            collection->surface[i] = NULL;
        }
        if (collection->texture[i])
        {
            SDL_DestroyTexture(collection->texture[i]);
            collection->texture[i] = NULL;
        }
    }
    free(collection->surface);
    free(collection->texture);
    free(collection);
}

SDL_Surface* SDL_CreateRGBSurfaceLite(int w, int h)
{
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface* fgSurface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
    return fgSurface;
}

int SDL_FillAll(SDL_Surface* surface, Uint32 color)
{
    return SDL_FillRect(surface, &surface->clip_rect, color);
}

int SDL_PutSurface(SDL_Surface* src, SDL_Point* point, SDL_Surface* dst)
{
    SDL_Rect dst_rect = { .x = point->x, .y = point->y, .w = 0, .h = 0 };
    SDL_Rect src_rect = { .x = 0, .y = 0, .w = src->w, .h = src->h };
    return SDL_BlitSurface(src, &src_rect, dst, &dst_rect);
}

void SDL_CopyRect(SDL_Rect* src, SDL_Rect* dst)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->w = src->w;
    dst->h = src->h;
}
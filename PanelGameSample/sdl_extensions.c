#include "sdl_extensions.h"

SDL_SurfaceCollection* SDL_CreateSurfaceCollection(int length)
{
    SDL_SurfaceCollection* collection = (SDL_SurfaceCollection*)malloc(sizeof(SDL_SurfaceCollection));
    if (collection)
    {
        collection->surface = (SDL_Surface**)calloc(length, sizeof(SDL_Surface*));
        collection->length = length;
        if (collection->surface)
        {
            for (int i = 0; i < length; i++)
            {
                collection->surface[i] = NULL;
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
    }
    free(collection->surface);
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
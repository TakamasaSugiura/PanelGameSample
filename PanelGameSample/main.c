#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;
static const int BORDER_WIDTH = 5;
static const int BORDER_HEIGHT = 6;

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

typedef enum
{
    BACKGROUND = 0,
    FOREGROUND
} SurfaceIndex;

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

void DrawSurfaceCollection(SDL_SurfaceCollection* collection, SDL_Window* window)
{
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    for (int i = 0; i < collection->length; i++)
    {
        SDL_Surface* srcSurface = collection->surface[i];
        if (srcSurface != NULL)
        {
            SDL_BlitSurface(srcSurface, &srcSurface->clip_rect, windowSurface, &windowSurface->clip_rect);
        }
    }
    SDL_UpdateWindowSurface(window);
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

SDL_Surface* CreateBackgroundSurface(SDL_Rect* windowRect, SDL_Size* areaSize)
{
    SDL_Surface* bgSurface = SDL_CreateRGBSurfaceLite(windowRect->w, windowRect->h);
    //SDL_FillRect(bgSurface, &bgSurface->clip_rect, SDL_MapRGB(bgSurface->format, 0x00, 0x00, 0x00));
    SDL_Surface* imageSurface = IMG_Load("bgimage.png");
    SDL_BlitSurface(imageSurface, &imageSurface->clip_rect, bgSurface, &bgSurface->clip_rect);
    SDL_FreeSurface(imageSurface);
    SDL_Rect horizontalBorderRect = { .x = 0, .y = areaSize->h, .w = bgSurface->clip_rect.w, .h = BORDER_HEIGHT };
    Uint32 borderColor = SDL_MapRGB(bgSurface->format, 0xFF, 0xFF, 0xFF);
    SDL_FillRect(bgSurface, &horizontalBorderRect, borderColor);
    horizontalBorderRect.y += areaSize->h + BORDER_HEIGHT;
    SDL_FillRect(bgSurface, &horizontalBorderRect, borderColor);
    SDL_Rect verticalBorderRect = { .x = areaSize->w, .y = 0, .w = BORDER_WIDTH, .h = bgSurface->clip_rect.w };
    SDL_FillRect(bgSurface, &verticalBorderRect, borderColor);
    verticalBorderRect.x += areaSize->w + BORDER_WIDTH;
    SDL_FillRect(bgSurface, &verticalBorderRect, borderColor);
    return bgSurface;
}

SDL_Surface* CreateForegroundSurface(SDL_Rect* windowRect)
{
    SDL_Surface* fgSurface = SDL_CreateRGBSurfaceLite(windowRect->w, windowRect->h);
    SDL_FillRect(fgSurface, &fgSurface->clip_rect, SDL_MapRGBA(fgSurface->format, 0, 0, 0, 0));
    return fgSurface;
}

void PanelClickedEvent(SDL_bool map[3][3], int xIndex, int yIndex)
{
    map[yIndex][xIndex] = !map[yIndex][xIndex];
    if (yIndex - 1 >= 0) map[yIndex - 1][xIndex] = !map[yIndex - 1][xIndex];
    if (yIndex + 1 <= 2) map[yIndex + 1][xIndex] = !map[yIndex + 1][xIndex];
    if (xIndex - 1 >= 0) map[yIndex][xIndex - 1] = !map[yIndex][xIndex - 1];
    if (xIndex + 1 <= 2) map[yIndex][xIndex + 1] = !map[yIndex][xIndex + 1];
}


void UpdateWindow(SDL_bool map[3][3], SDL_Size* areaSize, SDL_SurfaceCollection* collection, SDL_Window* window)
{
    int count = 0;
    SDL_FreeSurface(collection->surface[FOREGROUND]);
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    collection->surface[FOREGROUND] = SDL_CreateRGBSurfaceLite(windowSurface->clip_rect.w, windowSurface->clip_rect.h);
    SDL_Surface* fgSurface = collection->surface[FOREGROUND];
    for (int loopY = 0; loopY < 3; loopY++)
    {
        for (int loopX = 0; loopX < 3; loopX++)
        {
            SDL_Rect rect;
            rect.x = loopX * (areaSize->w + BORDER_WIDTH);
            rect.y = loopY * (areaSize->h + BORDER_HEIGHT);
            rect.w = areaSize->w;
            rect.h = areaSize->h;
            if (map[loopY][loopX])
            {
                SDL_FillRect(fgSurface, &rect, SDL_MapRGBA(fgSurface->format, 0x00, 0x66, 0x00, 0x66));
            }
        }
    }
    DrawSurfaceCollection(collection, window);
}

void MouseButtonDownEvent(SDL_bool map[3][3], SDL_Event* event, SDL_Size* areaSize, SDL_SurfaceCollection* collection, SDL_Window* window, SDL_bool* endFlag)
{
    int pointedX = event->button.x / (areaSize->w + BORDER_WIDTH / 2);
    int pointedY = event->button.y / (areaSize->h + BORDER_HEIGHT / 2);

    PanelClickedEvent(map, pointedX, pointedY);

    UpdateWindow(map, areaSize, collection, window);

    int count = 0;
    for (int loopY = 0; loopY < 3; loopY++)
    {
        for (int loopX = 0; loopX < 3; loopX++)
        {
            if (map[loopY][loopX])
            {
                count++;
            }
        }
    }

    if (count == 9)
    {
        *endFlag = SDL_TRUE;
    }
}

void EventLoop(SDL_bool map[3][3], SDL_Size* areaSize, SDL_SurfaceCollection* collection, SDL_Window* window)
{
    SDL_bool endFlag = SDL_FALSE;

    while (!endFlag)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                endFlag = SDL_TRUE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                MouseButtonDownEvent(map, &event, areaSize, collection, window, &endFlag);
                break;
            default:
                break;
            }
        }
        SDL_Delay(10);
    }
}

int SDL_main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    SDL_SurfaceCollection* collection = SDL_CreateSurfaceCollection(2);

    SDL_Size areaSize = {
        .w = (WINDOW_WIDTH - (BORDER_WIDTH * 2)) / 3,
        .h = (WINDOW_HEIGHT - (BORDER_HEIGHT * 2)) / 3
    };

    SDL_bool map[3][3] =
    {
        { SDL_FALSE, SDL_FALSE, SDL_FALSE },
        { SDL_FALSE, SDL_FALSE, SDL_FALSE },
        { SDL_FALSE, SDL_FALSE, SDL_FALSE }
    };

    // Window
    SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // Window Surface
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    collection->surface[BACKGROUND] = CreateBackgroundSurface(&windowSurface->clip_rect, &areaSize);
    collection->surface[FOREGROUND] = CreateForegroundSurface(&windowSurface->clip_rect);

    srand(time(NULL));
    for (int i = 0; i < 4; i++)
    {
        int x = rand() % 3;
        int y = rand() % 3;
        PanelClickedEvent(map, x, y);
    }

    UpdateWindow(map, &areaSize, collection, window);

    EventLoop(map, &areaSize, collection, window);

    SDL_DeleteSurfaceCollection(collection);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
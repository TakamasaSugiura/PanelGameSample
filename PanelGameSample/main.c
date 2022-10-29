#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include "sdl_extensions.h"

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

typedef enum
{
    BACKGROUND = 0,
    FOREGROUND
} SurfaceIndex;


static SDL_bool m_map[7][7];
static int m_cell_count = 5; // 3, 4, 5のいずれか
static SDL_Size m_cell_size;
static int m_border_width = 0;
static int m_border_height = 0;


void InitMap()
{
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            m_map[y][x] = SDL_FALSE;
        }
    }
    switch (m_cell_count) 
    {
    case 3:
        m_border_width = 5;
        m_border_height = 6;
        break;
    case 4:
        m_border_width = 4;
        m_border_height = 4;
        break;
    case 5:
        m_border_width = 5;
        m_border_height = 5;
        break;
    default:
        // 
        break;
    }
}

void InitAll()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    InitMap();
}

void QuitAll()
{
    IMG_Quit();
    SDL_Quit();
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

SDL_Surface* CreateBackgroundSurface(SDL_Rect* windowRect)
{
    SDL_Surface* bgSurface = SDL_CreateRGBSurfaceLite(windowRect->w, windowRect->h);
    SDL_Surface* imageSurface = IMG_Load("bgimage.png");
    SDL_BlitSurface(imageSurface, &imageSurface->clip_rect, bgSurface, &bgSurface->clip_rect);
    SDL_FreeSurface(imageSurface);
    Uint32 borderColor = SDL_MapRGB(bgSurface->format, 0xFF, 0xFF, 0xFF);

    // Draw horizontal borders.
    SDL_Rect horizontalBorderRect = { .x = 0, .y = m_cell_size.h, .w = bgSurface->clip_rect.w, .h = m_border_height };
    for (int i = 0; i < m_cell_count; i++)
    {
        SDL_FillRect(bgSurface, &horizontalBorderRect, borderColor);
        horizontalBorderRect.y += m_cell_size.h + m_border_height;
    }

    // Draw vertical borders.
    SDL_Rect verticalBorderRect = { .x = m_cell_size.w, .y = 0, .w = m_border_width, .h = bgSurface->clip_rect.w };
    for (int i = 0; i < m_cell_count; i++)
    {
        SDL_FillRect(bgSurface, &verticalBorderRect, borderColor);
        verticalBorderRect.x += m_cell_size.w + m_border_height;
    }
    return bgSurface;
}

SDL_Surface* CreateForegroundSurface(SDL_Rect* windowRect)
{
    SDL_Surface* fgSurface = SDL_CreateRGBSurfaceLite(windowRect->w, windowRect->h);
    SDL_FillRect(fgSurface, &fgSurface->clip_rect, SDL_MapRGBA(fgSurface->format, 0, 0, 0, 0));
    return fgSurface;
}

void PanelClickedEvent(int xIndex, int yIndex)
{
    m_map[yIndex][xIndex] = !m_map[yIndex][xIndex];
    if (yIndex - 1 >= 0) m_map[yIndex - 1][xIndex] = !m_map[yIndex - 1][xIndex];
    if (yIndex + 1 <= m_cell_count - 1) m_map[yIndex + 1][xIndex] = !m_map[yIndex + 1][xIndex];
    if (xIndex - 1 >= 0) m_map[yIndex][xIndex - 1] = !m_map[yIndex][xIndex - 1];
    if (xIndex + 1 <= m_cell_count - 1) m_map[yIndex][xIndex + 1] = !m_map[yIndex][xIndex + 1];
}


void UpdateWindow(SDL_SurfaceCollection* collection, SDL_Window* window)
{
    int count = 0;
    SDL_FreeSurface(collection->surface[FOREGROUND]);
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    collection->surface[FOREGROUND] = SDL_CreateRGBSurfaceLite(windowSurface->clip_rect.w, windowSurface->clip_rect.h);
    SDL_Surface* fgSurface = collection->surface[FOREGROUND];
    for (int loopY = 0; loopY < m_cell_count; loopY++)
    {
        for (int loopX = 0; loopX < m_cell_count; loopX++)
        {
            SDL_Rect rect;
            rect.x = loopX * (m_cell_size.w + m_border_width);
            rect.y = loopY * (m_cell_size.h + m_border_height);
            rect.w = m_cell_size.w;
            rect.h = m_cell_size.h;
            if (m_map[loopY][loopX])
            {
                SDL_FillRect(fgSurface, &rect, SDL_MapRGBA(fgSurface->format, 0x00, 0x66, 0x00, 0x66));
            }
        }
    }
    DrawSurfaceCollection(collection, window);
}

void MouseButtonDownEvent(SDL_Event* event, SDL_SurfaceCollection* collection, SDL_Window* window, SDL_bool* endFlag)
{
    int pointedX = event->button.x / (m_cell_size.w + m_border_width / 2);
    int pointedY = event->button.y / (m_cell_size.h + m_border_height / 2);

    PanelClickedEvent(pointedX, pointedY);

    UpdateWindow(collection, window);

    int count = 0;
    for (int loopY = 0; loopY < m_cell_count; loopY++)
    {
        for (int loopX = 0; loopX < m_cell_count; loopX++)
        {
            if (m_map[loopY][loopX])
            {
                count++;
            }
        }
    }

    if (count == m_cell_count * m_cell_count)
    {
        *endFlag = SDL_TRUE;
    }
}

void EventLoop(SDL_SurfaceCollection* collection, SDL_Window* window)
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
                MouseButtonDownEvent(&event, collection, window, &endFlag);
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
    InitAll();
    SDL_SurfaceCollection* collection = SDL_CreateSurfaceCollection(2);
    m_cell_size.w = (WINDOW_WIDTH - (m_border_width * 2)) / m_cell_count;
    m_cell_size.h = (WINDOW_HEIGHT - (m_border_height * 2)) / m_cell_count;


    // Window
    SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // Window Surface
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    collection->surface[BACKGROUND] = CreateBackgroundSurface(&windowSurface->clip_rect);
    collection->surface[FOREGROUND] = CreateForegroundSurface(&windowSurface->clip_rect);

    srand(time(NULL));
    for (int i = 0; i < 4; i++)
    {
        int x = rand() % m_cell_count;
        int y = rand() % m_cell_count;
        PanelClickedEvent(x, y);
    }

    UpdateWindow(collection, window);

    EventLoop(collection, window);

    SDL_DeleteSurfaceCollection(collection);
    SDL_DestroyWindow(window);
    QuitAll();
    return 0;
}
#include "game_objects.h"
#define MAX_BUTTON_COUNT 100

Button* _buttons[MAX_BUTTON_COUNT];

void InitGameObjects()
{
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        _buttons[i] = NULL;
    }
}

static Button* CreateButton(SDL_Rect* src_rect, void (*func)(SDL_MouseButtonEvent*, void*), void* parameter)
{
    Button* button = (Button*)malloc(sizeof(Button));
    if (button)
    {
        SDL_Rect *rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
        SDL_CopyRect(src_rect, rect);
        button->rect = rect;
        button->func = func;
        button->parameter = parameter;
    }
    return button;
}

int RegisterScreenButton(SDL_Rect* src_rect, void (*func)(SDL_MouseButtonEvent*, void*), void* parameter)
{
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        if (_buttons[i] == NULL)
        {
            _buttons[i] = CreateButton(src_rect, func, parameter);
            return i;
        }
    }
    return -1;
}

int UnregisterScreenButton(int index)
{
    if (_buttons[index] != NULL)
    {
        free(_buttons[index]->rect);
        free(_buttons[index]);
        _buttons[index] = NULL;
    }
    return 0;
}

int DoButtonAction(SDL_MouseButtonEvent* event)
{
    SDL_Point point = { .x = event->x, .y = event->y };
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        if (_buttons[i] == NULL)
        {
            continue;
        }
        Button* button = _buttons[i];
        if (SDL_PointInRect(&point, button->rect))
        {
            button->func(event, button->parameter);
        }
    }
    return 0;
}

int UnregisterAllScreenButtons()
{
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        UnregisterScreenButton(i);
    }
    return 0;
}
#include "game_objects.h"
#define MAX_BUTTON_COUNT 100

Button* buttons[MAX_BUTTON_COUNT];

void init_game_objects()
{
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        buttons[i] = NULL;
    }
}

static Button* create_button(SDL_Rect* rect, void (*func)(SDL_MouseButtonEvent*, void*), void* parameter)
{
    Button* button = (Button*)malloc(sizeof(Button));
    if (button)
    {
        button->rect = rect;
        button->func = func;
        button->parameter = parameter;
    }
    return button;
}

int register_screen_button(SDL_Rect* rect, void (*func)(SDL_MouseButtonEvent*, void*), void* parameter)
{
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        if (buttons[i] == NULL)
        {
            buttons[i] = create_button(rect, func, parameter);
            return i;
        }
    }
    return -1;
}

int unregister_screen_button(int index)
{
    if (buttons[index] != NULL)
    {
        free(buttons[index]);
        buttons[index] = NULL;
    }
    return 0;
}

int do_button_action(SDL_MouseButtonEvent* event)
{
    SDL_Point point = { .x = event->x, .y = event->y };
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        if (buttons[i] == NULL)
        {
            continue;
        }
        Button* button = buttons[i];
        if (SDL_PointInRect(&point, button->rect))
        {
            button->func(event, button->parameter);
        }
    }
    return 0;
}

int unregister_all_screen_buttons()
{
    for (int i = 0; i < MAX_BUTTON_COUNT; i++)
    {
        unregister_screen_button(i);
    }
    return 0;
}
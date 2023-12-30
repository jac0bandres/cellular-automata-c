#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>

typedef struct View
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int dimensions[2];
    int cell_size;
    bool state;

} View;

void *poll_input(void *t_view)
{
    View *view = (void *) t_view;
    SDL_Event e;
    if (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            view->state = false;
        }
    }
}

void init_window(int dimensions[2], View *view)
{
    view->state = true;
    view->window = NULL;
    view->renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL");
        exit(0);
    }
    
    view->cell_size = 10;

    while((dimensions[1] * view->cell_size > 1000))
    {
        view->cell_size --;
    }

    view->window = SDL_CreateWindow("cellular automata", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dimensions[0] * view->cell_size, dimensions[1] * view->cell_size, SDL_RENDERER_ACCELERATED);
    view->renderer = SDL_CreateRenderer(view->window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (view->window == NULL || view->renderer == NULL)
    {
        printf("Window or renderer failure.");
        exit(0);
    } 

    pthread_t thread;
    pthread_create(&thread, NULL, poll_input, (void *)view);
}


void draw(int size, int **map, View *view)
{
    SDL_SetRenderDrawColor(view->renderer, 0, 0, 0, 255);
    SDL_RenderClear(view->renderer);
    SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, 255);
    for (int i = 0; i < size; i++)
    {
        int *cell = map[i];
        int x = cell[0];
        int y = cell[1];
        SDL_Rect rect;
        rect.w = view->cell_size - 1;
        rect.h = view->cell_size - 1;
        rect.x = x * view->cell_size;
        rect.y = y * view->cell_size;
        SDL_RenderFillRect(view->renderer, &rect);
    }

    SDL_RenderPresent(view->renderer);
}

#include <assert.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

//#include "sac.h"

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Mutex *mutex = NULL;

// typedef struct SDLdisplay {
//     SDL_Window *window;
//     SDL_Renderer *renderer;
//     SDL_Surface *surface;
//     SDL_Mutex *mutex;
// } SDLdisplay;

void initDisplay(int w, int h)
{
    assert(window == NULL);
    assert(surface == NULL);
    assert(mutex == NULL);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }

    printf("a\n");
    window = SDL_CreateWindow("SDL3", w, h, 0);
    if(window == NULL)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
    }

    printf("b\n");

    assert(window != NULL);
    surface = SDL_GetWindowSurface(window);
    if(surface == NULL)
    {
        printf("SDL_GetWindowSurface failed: %s\n", SDL_GetError());
    }

    SDL_ClearSurface(surface, 0., 0., 120., 255.);
    SDL_UpdateWindowSurface(window);

    printf("c\n");



    SDL_Delay(2000);

    mutex = SDL_CreateMutex();
    if(mutex == NULL)
    {
        printf("SDL_CreateMutex failed: %s\n", SDL_GetError());
    }

    SDL_Delay(2000);
}

void drawPixel(int x, int y, int r, int g, int b)
{
    printf("drawing pixel at %d %d\n", x, y);

    SDL_LockMutex(mutex);

    if (SDL_MUSTLOCK(surface)) {
        if (!SDL_LockSurface(surface)) {
            printf("SDL_LockSurface failed: %s\n", SDL_GetError());
        }
    }

    int yoffset = x * (surface->pitch / 4);
    int xoffset = y;

    Uint32 *bptr = (Uint32 *) (surface->pixels) + xoffset + yoffset;
    *bptr = r << 16 | g << 8 | b;

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    SDL_UpdateWindowSurface(window);

    SDL_UnlockMutex(mutex);
}


void closeDisplay(void)
{
    printf("SDL_Quit\n");
    SDL_Quit();
}

#if 0
int main()
{
    initDisplay(640, 480);
    printf("Created window\n");
    drawPixel(50,50, 255,255,255);
    drawPixel(60,60, 0,0,0);
    closeDisplay();
    return 0;
}
#endif

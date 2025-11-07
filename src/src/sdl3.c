#include <SDL3/SDL.h>

#include "sac.h"

typedef struct SDLdisplay {
    SDL_Window *window;
    SDL_Surface *surf;
} SDLdisplay;

SDLdisplay initDisplay(int w, int h)
{
    SDLdisplay disp;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SAC_RuntimeError("SDL_Init failed: %s", SDL_GetError());
    }

    SDL_Delay(2000);

    disp.window = SDL_CreateWindow("SDL3", w, h, 0);
    if(disp.window == NULL)
    {
        SAC_RuntimeError("SDL_CreateWindow failed: %s", SDL_GetError());
    }

    SDL_Delay(2000);

    disp.surf = SDL_GetWindowSurface(disp.window);

    SDL_Delay(2000);

    return disp;
}

void closeDisplay(SDLdisplay disp)
{
    SDL_DestroySurface(disp.surf);
    SDL_DestroyWindow(disp.window);
    SDL_Quit();
}

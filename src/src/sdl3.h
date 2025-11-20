#ifndef _SACSDL3_H_
#define _SACSDL3_H_

#include <stdint.h>
#include <SDL3/SDL.h>

#include "sac.h"
#include "sacinterface.h"

typedef struct SDLcontext {
    bool running;
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Thread *eventHandler;
} SDLcontext;

extern SDLcontext *SAC_InitDisplay(int height, int width);
extern void SAC_DrawPixels(SDLcontext *ctx, SACarg *sa_pixels);
extern int SAC_CloseDisplay(SDLcontext *ctx);
extern bool SAC_IsRunning(SDLcontext *ctx);

#endif /* _SACSDL3_H_ */

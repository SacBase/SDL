#ifndef _SACSDL3_H_
#define _SACSDL3_H_

#include <stdint.h>
#include <SDL3/SDL.h>

#include "sac.h"
#include "sacinterface.h"

typedef enum {
    SEL_none,
    SEL_from,
    SEL_to,
} selmode_t;

typedef struct SDLselection {
    SDL_Semaphore *isSelecting;
    selmode_t mode;
    int coords[4];
} SDLselection;

typedef struct SDLcontext {
    bool running;
    size_t width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Thread *eventHandler;
    SDLselection selection;
} SDLcontext;

extern SDLcontext *SAC_InitDisplay(int height, int width);
extern void SAC_DrawPixelsOffset(SDLcontext *ctx, SACarg *sacPixels, int xOffset, int yOffset);
extern void SAC_DrawPixels(SDLcontext *ctx, SACarg *sacPixels);
extern SACarg *SAC_GetSelection(SDLcontext *ctx);
extern int SAC_CloseDisplay(SDLcontext *ctx);
extern bool SAC_IsRunning(SDLcontext *ctx);

#endif /* _SACSDL3_H_ */

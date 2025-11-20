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

typedef struct SDLcontext {
    bool running;
    size_t width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Thread *eventHandler;
    SDL_Semaphore *waitForSelection;
    int selectionCoords[4];
    selmode_t selectionMode;
} SDLcontext;

extern SDLcontext *SAC_InitDisplay(int height, int width);
extern void SAC_DrawPixels(SDLcontext *ctx, SACarg *sa_pixels);
extern SACarg *SAC_GetSelection(SDLcontext *ctx);
extern int SAC_CloseDisplay(SDLcontext *ctx);
extern bool SAC_IsRunning(SDLcontext *ctx);

#endif /* _SACSDL3_H_ */

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <X11/Xlib.h>

#include "sdl3.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static int SAC_EventHandler(SDLcontext *ctx)
{
    while (ctx->running) {
        SDL_Event event;
        if (!SDL_WaitEvent(&event)) {
            SAC_RuntimeError("SDL_WaitEvent failed: %s", SDL_GetError());
        }

        switch (event.type) {
            case SDL_EVENT_QUIT:
                ctx->running = false;
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT && ctx->selection.mode == SEL_from) {
                    ctx->selection.coords[0] = event.button.x;
                    ctx->selection.coords[1] = event.button.y;
                    ctx->selection.mode = SEL_to;
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT && ctx->selection.mode == SEL_to) {
                    ctx->selection.coords[2] = event.button.x;
                    ctx->selection.coords[3] = event.button.y;
                    ctx->selection.mode = SEL_none;
                    SDL_SignalSemaphore(ctx->selection.isSelecting);
                }
                // Undo selection if the right mouse button was pressed
                if (event.button.button == SDL_BUTTON_RIGHT && ctx->selection.mode != SEL_none) {
                    ctx->selection.coords[0] = -1;
                    ctx->selection.coords[1] = -1;
                    ctx->selection.coords[2] = -1;
                    ctx->selection.coords[3] = -1;
                    ctx->selection.mode = SEL_none;
                    SDL_SignalSemaphore(ctx->selection.isSelecting);
                }
                break;

            default:
                break;
        }
    }

    SDL_Quit();
    exit(0);
}

SDLcontext *SAC_InitDisplay(int height, int width)
{
    SDLcontext *ctx = (SDLcontext *)malloc(sizeof(SDLcontext));
    ctx->width = width;
    ctx->height = height;
    ctx->running = true;

    // Fix for using SDL with X-forwarding over SSH
    XInitThreads();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SAC_RuntimeError("SDL_Init failed: %s", SDL_GetError());
    }

    if(!SDL_CreateWindowAndRenderer("SaC SDL3", width, height, 0, &ctx->window, &ctx->renderer)) {
        SAC_RuntimeError("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
    }

    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(ctx->texture == NULL) {
        SAC_RuntimeError("SDL_CreateTexture failed: %s", SDL_GetError());
    }

    ctx->eventHandler = SDL_CreateThread(SAC_EventHandler, "SAC_EventHandler", ctx);
    if (ctx->eventHandler == NULL) {
        SAC_RuntimeError("SDL_CreateThread failed: %s", SDL_GetError());
    }

    ctx->selection = (SDLselection){
        .isSelecting = SDL_CreateSemaphore(0),
        .mode = SEL_none,
        .coords = {-1, -1, -1, -1},
    };

    if (ctx->selection.isSelecting == NULL) {
        SAC_RuntimeError("SDL_CreateSemaphore failed: %s", SDL_GetError());
    }

    SDL_RenderClear(ctx->renderer);
    SDL_RenderPresent(ctx->renderer);
    return ctx;
}

void SAC_DrawPixelsOffset(SDLcontext *ctx, SACarg *sacPixels, int xOffset, int yOffset)
{
    assert(SACARGgetDim(sacPixels) == 3);
    assert(SACARGgetShape(sacPixels, 2) == 3);

    const int srcHeight = SACARGgetShape(sacPixels, 0);
    const int srcWidth = SACARGgetShape(sacPixels, 1);
    const int *srcPixels = SACARGgetSharedData(SACTYPE__MAIN__int, sacPixels);

    uint8_t *dstPixels;
    int pitch;
    if (!SDL_LockTexture(ctx->texture, NULL, (void **)&dstPixels, &pitch)) {
        SAC_RuntimeError("SDL_LockTexture failed: %s", SDL_GetError());
    }

    for (size_t y = 0; y < MIN(srcHeight, ctx->height - yOffset); y++) {
        const int *srcRow = srcPixels + y * srcWidth * 3;
        uint8_t *dstRow = dstPixels + (yOffset + y) * pitch;

        for (size_t x = 0; x < 3 * MIN(srcWidth, ctx->width - xOffset); x += 3) {
            dstRow[xOffset + x + 0] = (uint8_t)(srcRow[x + 0]);
            dstRow[xOffset + x + 1] = (uint8_t)(srcRow[x + 1]);
            dstRow[xOffset + x + 2] = (uint8_t)(srcRow[x + 2]);
        }
    }

    SDL_UnlockTexture(ctx->texture);
    SDL_RenderTexture(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
}

void SAC_DrawPixels(SDLcontext *ctx, SACarg *sacPixels)
{
    SAC_DrawPixelsOffset(ctx, sacPixels, 0, 0);
}

SACarg *SAC_GetSelection(SDLcontext *ctx)
{
    ctx->selection.mode = SEL_from;
    SDL_WaitSemaphore(ctx->selection.isSelecting);
    assert(ctx->selection.mode == SEL_none);

    int *res = malloc(4 * sizeof(int));
    // Ensure coordinates are [topleft, bottomright] and each of the form [y,x]
    if (ctx->selection.coords[0] <= ctx->selection.coords[2]) {
        res[1] = ctx->selection.coords[0];
        res[3] = ctx->selection.coords[2];
    } else {
        res[1] = ctx->selection.coords[2];
        res[3] = ctx->selection.coords[0];
    }
    if (ctx->selection.coords[1] <= ctx->selection.coords[3]) {
        res[0] = ctx->selection.coords[1];
        res[2] = ctx->selection.coords[3];
    } else {
        res[0] = ctx->selection.coords[3];
        res[2] = ctx->selection.coords[1];
    }

    return SACARGcreateFromPointer(SACTYPE__MAIN__int, (void *)res, 2, 2, 2);
}

int SAC_CloseDisplay(SDLcontext *ctx)
{
    if (ctx->running) {
        SDL_Event quitEvent;
        quitEvent.type = SDL_EVENT_QUIT;
        if (!SDL_PushEvent(&quitEvent)) {
            SAC_RuntimeError("SDL_PushEvent failed: %s", SDL_GetError());
        }
    }

    int exitStatus;
    SDL_WaitThread(ctx->eventHandler, &exitStatus);
    SDL_Quit();

    return exitStatus;
}

bool SAC_IsRunning(SDLcontext *ctx)
{
    return (ctx)->running;
}

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <X11/Xlib.h>

#include "sdl3.h"

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
                if (event.button.button == 1 && ctx->selectionMode == SEL_from) {
                    ctx->selectionCoords[0] = event.button.x;
                    ctx->selectionCoords[1] = event.button.y;
                    ctx->selectionMode = SEL_to;
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == 1 && ctx->selectionMode == SEL_to) {
                    ctx->selectionCoords[2] = event.button.x;
                    ctx->selectionCoords[3] = event.button.y;
                    ctx->selectionMode = SEL_none;
                    SDL_SignalSemaphore(ctx->waitForSelection);
                }
                break;

            default:
                break;
        }
    }

    SDL_DestroyTexture(ctx->texture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
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

    ctx->waitForSelection = SDL_CreateSemaphore(0);
    if (ctx->waitForSelection == NULL) {
        SAC_RuntimeError("SDL_CreateSemaphore failed: %s", SDL_GetError());
    }

    SDL_RenderClear(ctx->renderer);
    SDL_RenderPresent(ctx->renderer);
    return ctx;
}

void SAC_DrawPixels(SDLcontext *ctx, SACarg *sa_pixels)
{
    assert(SACARGgetDim(sa_pixels) == 3);
    assert(SACARGgetShape(sa_pixels, 0) == ctx->height);
    assert(SACARGgetShape(sa_pixels, 1) == ctx->width);
    assert(SACARGgetShape(sa_pixels, 2) == 3);

    const int *src_colors = SACARGgetSharedData(SACTYPE__MAIN__int, sa_pixels);

    uint8_t *dst_pixels;
    int pitch;
    if (!SDL_LockTexture(ctx->texture, NULL, (void **)&dst_pixels, &pitch)) {
        SAC_RuntimeError("SDL_LockTexture failed: %s", SDL_GetError());
    }

    for (int y = 0; y < ctx->height; y++) {
        const int *src_row = src_colors + y * ctx->width * 3;
        uint8_t *dst_row = dst_pixels + y * pitch;

        for (int x = 0; x < 3 * ctx->width; x += 3) {
            dst_row[x + 0] = (uint8_t)(src_row[x + 0]);
            dst_row[x + 1] = (uint8_t)(src_row[x + 1]);
            dst_row[x + 2] = (uint8_t)(src_row[x + 2]);
        }
    }

    SDL_UnlockTexture(ctx->texture);
    SDL_RenderTexture(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
}

SACarg *SAC_GetSelection(SDLcontext *ctx)
{
    ctx->selectionMode = SEL_from;

    SDL_WaitSemaphore(ctx->waitForSelection);
    assert(ctx->selectionMode == SEL_none);

    int *res = malloc(4 * sizeof(int));
    // Ensure coordinates are [topleft, bottomright] and each of the form [y,x]
    if (ctx->selectionCoords[0] <= ctx->selectionCoords[2]) {
        res[1] = ctx->selectionCoords[0];
        res[3] = ctx->selectionCoords[2];
    } else {
        res[1] = ctx->selectionCoords[2];
        res[3] = ctx->selectionCoords[0];
    }
    if (ctx->selectionCoords[1] <= ctx->selectionCoords[3]) {
        res[0] = ctx->selectionCoords[1];
        res[2] = ctx->selectionCoords[3];
    } else {
        res[0] = ctx->selectionCoords[3];
        res[2] = ctx->selectionCoords[1];
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

    SDL_DestroyTexture(ctx->texture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();

    return exitStatus;
}

bool SAC_IsRunning(SDLcontext *ctx)
{
    return (ctx)->running;
}

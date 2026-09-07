#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL.h>

#ifdef HAVE_X11
#include <X11/Xlib.h>
#endif

#include "sdl3.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static int SAC_EventHandler(void *data)
{
    SDLcontext *ctx = (SDLcontext *)data;

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

    return 0;
}

SDLcontext *SAC_InitDisplay(sac_int height, sac_int width)
{
    SDLcontext *ctx = (SDLcontext *)malloc(sizeof(SDLcontext));
    ctx->width = width;
    ctx->height = height;
    ctx->running = true;

#ifdef HAVE_X11
    // Fix for using SDL with X-forwarding over SSH
    XInitThreads();
#endif

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SAC_RuntimeError("SDL_Init failed: %s", SDL_GetError());
    }

    if(!SDL_CreateWindowAndRenderer("SaC SDL3", (int)width, (int)height, 0, &ctx->window, &ctx->renderer)) {
        SAC_RuntimeError("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
    }

    // printf("Creating a texture of size [%"PRIisac",%"PRIisac"]\n", width, height);
    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, (int)width, (int)height);
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

    if (!SDL_RenderClear(ctx->renderer)) {
        SAC_RuntimeError("SDL_RenderClear: %s", SDL_GetError());
    }

    if (!SDL_RenderPresent(ctx->renderer)) {
        SAC_RuntimeError("SDL_RenderPresent: %s", SDL_GetError());
    }

    return ctx;
}

void SAC_DrawPixelsOffset(SDLcontext *ctx, SACarg *sacPixels, sac_int xOffset, sac_int yOffset)
{
    assert(SACARGgetDim(sacPixels) == 3);
    assert(SACARGgetShape(sacPixels, 2) == 3);

    const sac_int srcHeight = SACARGgetShape(sacPixels, 0);
    const sac_int srcWidth = SACARGgetShape(sacPixels, 1);
    const sac_int *srcPixels = SACARGgetSharedData(SACTYPE__MAIN__int, sacPixels);

    assert(srcPixels != NULL);
    assert(xOffset >= 0 && xOffset < (sac_int)ctx->width);
    assert(yOffset >= 0 && yOffset < (sac_int)ctx->height);

    // printf("Drawing pixels with offset [%"PRIisac",%"PRIisac"] and shape [%"PRIisac",%"PRIisac"]\n",
    //        xOffset, yOffset, srcWidth, srcHeight);

    uint8_t *dstPixels;
    int pitch; // The pitch of the locked pixels; the pitch is the length of one row in bytes.
    if (!SDL_LockTexture(ctx->texture, NULL, (void **)&dstPixels, &pitch)) {
        SAC_RuntimeError("SDL_LockTexture failed: %s", SDL_GetError());
    }

    assert(pitch > 0);
    assert(dstPixels != NULL);
    // printf("pitch=%d bytes=%zu\n", pitch, (size_t)pitch * ctx->height);

    sac_int copyWidth = MIN(srcWidth, (sac_int)ctx->width - xOffset);
    sac_int copyHeight = MIN(srcHeight, (sac_int)ctx->height - yOffset);
    // printf("Copying a total of [%"PRIisac",%"PRIisac"] pixels\n", copyWidth, copyHeight);

    for (sac_int y = 0; y < copyHeight; y++) {
        assert(3 * y * srcWidth < srcWidth * srcHeight * 3);
        assert((y + yOffset) * (sac_int)pitch < (sac_int)ctx->height * (sac_int)pitch);

        const sac_int *srcRow = srcPixels + (3 * y * srcWidth);
        uint8_t *dstRow = dstPixels + ((y + yOffset) * (sac_int)pitch);

        assert(srcRow != NULL);
        assert(dstRow != NULL);

        for (sac_int x = 0; x < copyWidth; x++) {
            assert(3 * x + 2 < srcWidth * 3);
            assert(3 * (xOffset + x) + 2 < (sac_int)pitch);

            dstRow[3 * (xOffset + x) + 0] = (uint8_t)(srcRow[3 * x + 0]);
            dstRow[3 * (xOffset + x) + 1] = (uint8_t)(srcRow[3 * x + 1]);
            dstRow[3 * (xOffset + x) + 2] = (uint8_t)(srcRow[3 * x + 2]);
        }
    }

    SDL_UnlockTexture(ctx->texture);
    if (!SDL_RenderTexture(ctx->renderer, ctx->texture, NULL, NULL)) {
        SAC_RuntimeError("SDL_RenderTexture: %s", SDL_GetError());
    }

    if (!SDL_RenderPresent(ctx->renderer)) {
        SAC_RuntimeError("SDL_RenderPresent: %s", SDL_GetError());
    }
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

    sac_int *res = malloc(4 * sizeof(sac_int));
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

    sac_int shp[] = { 2, 2 };
    return SACARGcreateFromPointer(SACTYPE__MAIN__int, (void *)res, 2, shp);
}

sac_int SAC_CloseDisplay(SDLcontext *ctx)
{
    if (ctx->running) {
        SDL_Event quitEvent;
        SDL_zero(quitEvent); // Initialize the event
        quitEvent.type = SDL_EVENT_QUIT;
        if (!SDL_PushEvent(&quitEvent)) {
            SAC_RuntimeError("SDL_PushEvent failed: %s", SDL_GetError());
        }
    }

    int exitStatus;
    SDL_WaitThread(ctx->eventHandler, &exitStatus);
    return (sac_int)exitStatus;
}

bool SAC_IsRunning(SDLcontext *ctx)
{
    return (ctx)->running;
}

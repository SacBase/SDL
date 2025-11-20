#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <X11/Xlib.h>

#include "sac.h"
#include "sacinterface.h"

typedef struct SDLcontext {
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} SDLcontext;

SDLcontext *SAC_InitDisplay(int height, int width)
{
    SDLcontext *ctx = (SDLcontext *)malloc(sizeof(SDLcontext));
    ctx->width = width;
    ctx->height = height;

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

int SAC_CloseDisplay(SDLcontext *ctx)
{
    SDL_DestroyTexture(ctx->texture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
    return 0;
}

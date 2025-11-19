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
    SDL_Mutex *mutex;
} SDLcontext;

SDLcontext *initDisplay(int width, int height)
{
    SDLcontext *ctx = (SDLcontext *)malloc(sizeof(SDLcontext));
    ctx->width = width;
    ctx->height = height;

    // Fix for using SDL with X-forwarding over SSH
    XInitThreads();

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SAC_RuntimeError("SDL_Init failed: %s", SDL_GetError());
    }

    if(!SDL_CreateWindowAndRenderer("SaC SDL3", width, height, 0, &ctx->window, &ctx->renderer))
    {
        SAC_RuntimeError("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
    }

    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(ctx->texture == NULL)
    {
        SAC_RuntimeError("SDL_CreateTexture failed: %s", SDL_GetError());
    }

    uint32_t *pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = (x % 2) * 255;
            uint8_t g = (y % 2) * 255;
            uint8_t b = ((x + y) % 2) * 255;
            uint8_t a = 255;
            pixels[x + y * width] = (r << 0) | (g << 8) | (b << 16) | (a << 24);
        }
    }

    SDL_UpdateTexture(ctx->texture, NULL, pixels, width * sizeof(uint32_t));
    //free(pixels);

    SDL_RenderClear(ctx->renderer);
    SDL_RenderTexture(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);

    ctx->mutex = SDL_CreateMutex();
    if(ctx->mutex == NULL)
    {
        printf("SDL_CreateMutex failed: %s\n", SDL_GetError());
    }

    SAC_Print ("Created %dx%d SDL3 display\n", width, height);
    return ctx;
}

void drawScreen(SDLcontext *ctx, SACarg *sa_pixels)
{
    assert(SACARGgetDim(sa_pixels) == 3);
    assert(SACARGgetShape(sa_pixels, 0) == ctx->width);
    assert(SACARGgetShape(sa_pixels, 1) == ctx->height);
    assert(SACARGgetShape(sa_pixels, 2) == 3);

    SDL_LockMutex(ctx->mutex);

    const int *sa_pixel_data = SACARGgetSharedData(SACTYPE__MAIN__int, sa_pixels);

    uint32_t *pixels;
    int pitch;
    if (!SDL_LockTexture(ctx->texture, NULL, (void **)&pixels, &pitch)) {
        SAC_RuntimeError("SDL_LockTexture failed: %s", SDL_GetError());
    }
    assert(pitch == ctx->width * sizeof(uint32_t));

    for (int i = 0, y = 0; y < ctx->height; y++) {
        for (int x = 0; x < ctx->width; x++, i++) {
            uint8_t r = sa_pixel_data[3 * i + 0];
            uint8_t g = sa_pixel_data[3 * i + 1];
            uint8_t b = sa_pixel_data[3 * i + 2];
            uint8_t a = 255;
            pixels[i] = (r << 0) | (g << 8) | (b << 16) | (a << 24);
        }
    }

    SDL_UnlockTexture(ctx->texture);

    SDL_RenderClear(ctx->renderer);
    SDL_RenderTexture(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);

    SDL_Delay(5000);

    SDL_UnlockMutex(ctx->mutex);
}

void closeDisplay(SDLcontext *ctx)
{
    printf("Closing %dx%d SDL3 display\n", ctx->width, ctx->height);
    SDL_DestroyMutex(ctx->mutex);
    SDL_DestroyTexture(ctx->texture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
}

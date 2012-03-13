#include "SDL2sac.h"

void SAC_SDL2_pixel( SDL2* disp, SAC_ND_PARAM_in( shp_nt, int), SAC_ND_PARAM_in( color_nt, int))
{
  const int           x = SAC_ND_A_FIELD( shp_nt)[1];
  const int           y = SAC_ND_A_FIELD( shp_nt)[0];
  const int*          src = SAC_ND_A_FIELD( color_nt);
  const unsigned char r = src[0];
  const unsigned char g = src[1];
  const unsigned char b = src[2];
  int                 locking;
  int                 offset;
  Uint32*             bptr;

  if (SDL2_DEBUG( disp) >= 3) {
    printf("%sSAC_SDL_pixel [%d,%d] %d,%d,%d\n", When( disp), x, y, r, g, b);
  }

  SAC_SDL2_check( disp);
  locking = SDL2_LOCKED( disp);
  if (locking) {
    SAC_SDL2_lock( disp);
  }

  if ( ((unsigned) x >= SDL2_WIDTH( disp)) ||
       ((unsigned) y >= SDL2_HEIGHT( disp)) ) {
    SAC_RuntimeError( "SAC_SDL_pixel: Pixel [%d,%d] exceeds display [%d,%d]\n",
                      x, y, SDL2_WIDTH( disp), SDL2_HEIGHT( disp));
  } else {
    offset = x + y * (SDL2_PITCH( disp) / 4);
    bptr = (Uint32 *) SDL2_PIXELS(disp) + offset;
    *bptr = ((Uint32) r << 16) | ((Uint32) g << 8) | ((Uint32) b);
  }

  // Updating a single pixel is too slow, besides, SDL seems
  // to simply drop user events when too many arrive:
  // SAC_SDL2_update_pixel( disp, x, y);

  if (locking) {
    SAC_SDL2_unlock( disp);
  }

  SAC_ND_DEC_RC_FREE( shp_nt, 1, )
  SAC_ND_DEC_RC_FREE( color_nt, 1, )
}


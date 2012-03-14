#include "SDL2sac.h"

void SAC_SDL2_draw_a_pixel( SDL2* disp, int x, int y, Uint32 rgb)
{
  const int pitch = SDL2_PITCH( disp) / BYTES_PER_PIXEL;
  Uint32*   videomem = (Uint32 *) SDL2_PIXELS( disp);

  videomem[x + y * pitch] = rgb;
}

void SAC_SDL2_pixel( SDL2* disp, SAC_ND_PARAM_in( shp_nt, int), SAC_ND_PARAM_in( color_nt, int))
{
  const int           pitch = SDL2_PITCH( disp) / BYTES_PER_PIXEL;
  Uint32*             videomem = (Uint32 *) SDL2_PIXELS( disp);
  const int           x = SAC_ND_A_FIELD( shp_nt)[1];
  const int           y = SAC_ND_A_FIELD( shp_nt)[0];
  const int*          color = SAC_ND_A_FIELD( color_nt);
  const unsigned char r = color[0];
  const unsigned char g = color[1];
  const unsigned char b = color[2];

  if (SDL2_DEBUG( disp) >= 3) {
    printf("%sSAC_SDL_pixel [%d,%d] %d,%d,%d\n", When( disp), x, y, r, g, b);
  }

  SAC_SDL2_check( disp);

  if ( ((unsigned) x >= SDL2_DISP_WIDTH( disp)) ||
       ((unsigned) y >= SDL2_DISP_HEIGHT( disp)) ) {
    SAC_RuntimeError( "SAC_SDL_pixel: Pixel [%d,%d] exceeds display [%d,%d]\n",
                      x, y, SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else {
    const int xoffset = SDL2_DISP_X( disp) + x;
    const int yoffset = SDL2_DISP_Y( disp) + y;
    videomem[xoffset + yoffset * pitch] = RGB(r,g,b);
  }

  SAC_ND_DEC_RC_FREE( shp_nt, 1, )
  SAC_ND_DEC_RC_FREE( color_nt, 1, )
}


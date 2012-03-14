#include "SDL2sac.h"

void SAC_SDL2_line( SDL2* disp, const int P1[2], const int P2[2], int async)
{
  const int     width = SDL2_DISP_WIDTH( disp);
  const int     height = SDL2_DISP_HEIGHT( disp);
  int           x1 = P1[1];
  int           y1 = P1[0];
  int           x2 = P2[1];
  int           y2 = P2[0];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_line: [%d,%d] to [%d,%d] on [%d,%d]\n",
            When( disp), x1, y1, x2, y2,
            width, height);
  }

  SAC_SDL2_lock( disp);

  if ( x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 ||
       x1 >= width || x2 >= width || y1 >= height || y2 >= height)
  {
    SAC_RuntimeError( "SAC_SDL2_line: [%d,%d] to [%d,%d] exceeds [%d,%d]\n",
                      x1, y1, x2, y2, width, height);
  }
  else {
    SAC_SDL2_draw_a_line( disp,
                          SDL2_DISP_X( disp) + x1,
                          SDL2_DISP_Y( disp) + y1,
                          SDL2_DISP_X( disp) + x2,
                          SDL2_DISP_Y( disp) + y2,
                          SAC_SDL2_foreground_rgb,
                          TRUE,
                          async);
  }

  SAC_SDL2_unlock( disp);
}

void SAC_SDL2_draw_a_line( SDL2* disp, int x1, int y1, int x2, int y2,
                           Uint32 rgb, int update, int async)
{
  const int            pitch = SDL2_PITCH( disp) / BYTES_PER_PIXEL;
  Uint32*              videomem = (Uint32 *) SDL2_PIXELS( disp);
  
  int                  x;
  int                  y;
  int                  xinc;
  int                  yinc;
  int                  deltax;
  int                  deltay;
  int                  denom;
  int                  numer;

  deltax = DELTA( x1, x2);
  deltay = DELTA( y1, y2);

  if (deltax >= deltay)
  {
    if (x1 > x2) {
      SWAP2( int, x1, x2);
      SWAP2( int, y1, y2);
    }

    if (deltay == 0) {
      for (x = x1; x <= x2; ++x)
      {
        videomem[x + y1 * pitch] = rgb;
      }
    }
    else {
      denom = deltax;
      numer = deltax / 2;
      y = y1;
      yinc = (y1 <= y2) ? 1 : -1;

      for (x = x1; x <= x2; ++x)
      {
        videomem[x + y * pitch] = rgb;
        numer += deltay;
        if (numer >= denom)
        {
          numer -= denom;
          y += yinc;
        }
      }
    }
  }
  else
  {
    if (y1 > y2) {
      SWAP2( int, x1, x2);
      SWAP2( int, y1, y2);
    }

    if (deltax == 0) {
      for (y = y1; y <= y2; ++y)
      {
        videomem[x1 + y * pitch] = rgb;
      }
    }
    else {
      denom = deltay;
      numer = deltay / 2;
      x = x1;
      xinc = (x1 <= x2) ? 1 : -1;

      for (y = y1; y <= y2; ++y)
      {
        videomem[x + y * pitch] = rgb;
        numer += deltax;
        if (numer >= denom)
        {
          numer -= denom;
          x += xinc;
        }
      }
    }
  }

  if (update) {
    SAC_SDL2_update_rect( disp,
                          MIN( x1, x2), MIN( y1, y2),
                          deltax + 1, deltay + 1,
                          async);
  }
}


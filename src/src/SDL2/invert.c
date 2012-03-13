#include "SDL2sac.h"

void SAC_SDL2_invert_rect( SDL2* disp, int *pos, int update)
{
  int     y, x;
  int     x1, x2, y1, y2;
  int     screenoffset;
  Uint32* bptr;

  x1 = (pos[0] < pos[2]) ? pos[0] : pos[2];
  x2 = (pos[0] < pos[2]) ? pos[2] : pos[0];
  y1 = (pos[1] < pos[3]) ? pos[1] : pos[3];
  y2 = (pos[1] < pos[3]) ? pos[3] : pos[1];

  for (y = y1; y <= y2; ++y) {
    screenoffset = y * SDL2_PITCH( disp) / 4;
    bptr = (Uint32 *) SDL2_PIXELS( disp) + screenoffset + x1;
    for (x = x1; x <= x2 ; ++x, ++bptr) {
      *bptr = (0xFFFFFF & ~(*bptr));
    }
  }

  if (update) {
    SDL_UpdateRect( SDL2_SURF( disp), x1, y1, x2 - x1 + 1, y2 - y1 + 1);
  }
}

void SAC_SDL2_invert2( SDL2* disp, int offsets[2], int sizes[2])
{
  const int width = sizes[1];
  const int height = sizes[0];
  const int xoffset = offsets[1];
  const int yoffset = offsets[0];
  const int dont_update = 0;

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_invert2: [%d,%d]:[%d,%d] on [%d,%d]\n",
            When( disp), xoffset, yoffset, width, height,
            SDL2_WIDTH( disp), SDL2_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       xoffset + width  > SDL2_WIDTH(disp) ||
       yoffset + height > SDL2_HEIGHT(disp))
  {
    SAC_RuntimeError( "SAC_SDL2_invert2: Offset [%d,%d] + array [%d,%d] "
                      "exceeds display [%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_WIDTH(disp), SDL2_HEIGHT(disp));
  }
  else {
    int pos[4];
    pos[0] = xoffset;
    pos[1] = yoffset;
    pos[2] = xoffset + width - 1;
    pos[3] = yoffset + height - 1;
    SAC_SDL2_invert_rect( disp, pos, dont_update);
    SAC_SDL2_update_rect( disp, xoffset, yoffset, width, height, FALSE);
  }

  SAC_SDL2_unlock( disp);
}


#include "SDL2sac.h"

void SAC_SDL2_invert_rect( SDL2* disp, const int *pos, const int update)
{
  const int pitch = SDL2_PITCH( disp) / BYTES_PER_PIXEL;
  Uint32*   videomem = (Uint32 *) SDL2_PIXELS( disp);
  int       y, x;
  int       x1, x2, y1, y2;
  Uint32*   bptr;

  x1 = MIN(pos[0], pos[2]);
  x2 = MAX(pos[0], pos[2]);
  y1 = MIN(pos[1], pos[3]);
  y2 = MAX(pos[1], pos[3]);

  for (y = y1; y <= y2; ++y) {
    bptr = videomem + x1 + y * pitch;
    for (x = x1; x <= x2 ; ++x, ++bptr) {
      *bptr = (0xFFFFFF & ~(*bptr));
    }
  }

  if (update) {
    SDL_UpdateRect( SDL2_SURF( disp), x1, y1, x2 - x1 + 1, y2 - y1 + 1);
  }
}

void SAC_SDL2_invert2( SDL2* disp, const int offsets[2], const int sizes[2])
{
  const int width = sizes[1];
  const int height = sizes[0];
  const int xoffset = offsets[1];
  const int yoffset = offsets[0];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_invert2: [%d,%d]:[%d,%d] on [%d,%d]\n",
            When( disp), xoffset, yoffset, width, height,
            SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       width < 0 || height < 0 ||
       xoffset + width  > SDL2_DISP_WIDTH( disp) ||
       yoffset + height > SDL2_DISP_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_invert2: Invalid offset [%d,%d] + size [%d,%d] "
                      "on display [%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else {
    int pos[4];
    pos[0] = xoffset + SDL2_DISP_X( disp);
    pos[1] = yoffset + SDL2_DISP_Y( disp);
    pos[2] = pos[0] + width - 1;
    pos[3] = pos[1] + height - 1;
    SAC_SDL2_invert_rect( disp, pos, false);
    SAC_SDL2_update_rect( disp, pos[0], pos[1], width, height, false);
  }

  SAC_SDL2_unlock( disp);
}


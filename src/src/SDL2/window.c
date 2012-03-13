#include "SDL2sac.h"

SDL2* SAC_SDL2_window( SDL2* disp, int offsets[2], int sizes[2])
{
  const int width = sizes[1];
  const int height = sizes[0];
  const int xoffset = offsets[1];
  const int yoffset = offsets[0];
  SDL2*     window = NULL;

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_window: [%d,%d]:[%d,%d] on [%d,%d]\n",
            When( disp),
            xoffset, yoffset,
            width, height,
            SDL2_DISP_WIDTH( disp),
            SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       xoffset + width  > SDL2_DISP_WIDTH( disp) ||
       yoffset + height > SDL2_DISP_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_window: Offset [%d,%d] + array [%d,%d] "
                      "exceeds display [%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else {
    window = SAC_SDL2_copy_disp( disp, xoffset, yoffset, width, height);
  }

  SAC_SDL2_unlock( disp);

  return window;
}


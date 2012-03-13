#include "SDL2sac.h"

static void copy_rect( SDL2* disp, const int* color,
                       const int xoffset, const int yoffset,
                       const int width, const int height)
{
  const int pitch = SDL2_PITCH(disp) / 4;
  Uint32*   videomem = (Uint32 *) SDL2_PIXELS(disp);
  int       x, y;

  for (y = 0; y < height; ++y) {
    const int  offset = xoffset + ((y + yoffset) * pitch);
    Uint32*    dest   = videomem + offset;
    const int* src    = color + y * width * 3;

    for (x = 0; x < width; ++x) {
      const unsigned char r = *src++;
      const unsigned char g = *src++;
      const unsigned char b = *src++;
      *dest++ = ((Uint32) r << 16) | ((Uint32) g << 8) | ((Uint32) b);
    }
  }
}

void SAC_SDL2_draw( SDL2* disp,
                    SAC_ND_PARAM_in( ar_nt, int))
{
  const int width  = SAC_ND_A_DESC_SHAPE( ar_nt, 1);
  const int height = SAC_ND_A_DESC_SHAPE( ar_nt, 0);

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_draw %d,%d\n", When( disp), width, height);
  }

  SAC_SDL2_lock( disp);

  if ( width  != SDL2_WIDTH(disp) ||
       height != SDL2_HEIGHT(disp))
  {
    SAC_RuntimeError( "SAC_SDL2_draw: Array [%d,%d] != display [%d,%d]\n",
                      width, height, SDL2_WIDTH(disp), SDL2_HEIGHT(disp));
  }
  else {
    copy_rect( disp, SAC_ND_A_FIELD( ar_nt), 0, 0, width, height);
    SAC_SDL2_update_rect( disp, 0, 0, width, height, FALSE);
  }

  SAC_SDL2_unlock( disp);

  SAC_ND_DEC_RC_FREE( ar_nt, 1, )
}

void SAC_SDL2_draw2( SDL2* disp, SAC_ND_PARAM_in( ar_nt, int), int offsets[2])
{
  const int width = SAC_ND_A_DESC_SHAPE( ar_nt, 1);
  const int height = SAC_ND_A_DESC_SHAPE( ar_nt, 0);
  const int xoffset = offsets[1];
  const int yoffset = offsets[0];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_draw2 [%d,%d]:[%d,%d] on [%d,%d]\n",
            When( disp), xoffset, yoffset, width, height,
            SDL2_WIDTH( disp), SDL2_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       xoffset + width  > SDL2_WIDTH(disp) ||
       yoffset + height > SDL2_HEIGHT(disp))
  {
    SAC_RuntimeError( "SAC_SDL2_draw2: Offset [%d,%d] + array [%d,%d] "
                      "exceeds display [%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_WIDTH(disp), SDL2_HEIGHT(disp));
  }
  else {
    copy_rect( disp, SAC_ND_A_FIELD( ar_nt), xoffset, yoffset, width, height);
    SAC_SDL2_update_rect( disp, xoffset, yoffset, width, height, FALSE);
  }

  SAC_SDL2_unlock( disp);

  SAC_ND_DEC_RC_FREE( ar_nt, 1, )
}


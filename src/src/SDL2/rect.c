#include "SDL2sac.h"

void SAC_SDL2_draw_rect( SDL2* disp, const int offsets[2], const int sizes[2],
                         int async)
{
  const int    width = sizes[1];
  const int    height = sizes[0];
  const int    xoffset = offsets[1];
  const int    yoffset = offsets[0];
  const Uint32 rgb = SAC_SDL2_foreground_rgb;

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_draw_rect: [%d,%d]:[%d,%d] on [%d,%d]\n",
            When( disp), xoffset, yoffset, width, height,
            SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       width < 0 || height < 0 ||
       xoffset + width  > SDL2_DISP_WIDTH( disp) ||
       yoffset + height > SDL2_DISP_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_draw_rect: Invalid offset [%d,%d] + "
                      "size [%d,%d] on display [%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else if (width > 1) {
    if (height > 1) {
      const int x = SDL2_DISP_X( disp) + xoffset;
      const int y = SDL2_DISP_Y( disp) + yoffset;
      SAC_SDL2_draw_a_line( disp, x, y, x + width - 1, y, rgb, TRUE, TRUE);
      SAC_SDL2_draw_a_line( disp, x, y + 1, x, y + height - 1, rgb, TRUE, TRUE);
      if (width > 2) {
        SAC_SDL2_draw_a_line( disp,
                              x + 1, y + height - 1,
                              x + width - 1, y + height - 1,
                              rgb, TRUE, TRUE);
      }
      SAC_SDL2_draw_a_line( disp,
                            x + width - 1, y + 1,
                            x + width - 1, y + height - 1,
                            rgb, TRUE, FALSE);
    }
    else if (height == 1) {
      SAC_SDL2_draw_a_line( disp,
                            SDL2_DISP_X( disp) + xoffset,
                            SDL2_DISP_Y( disp) + yoffset,
                            SDL2_DISP_X( disp) + xoffset + width - 1,
                            SDL2_DISP_Y( disp) + yoffset,
                            rgb, TRUE, async);
    }
  }
  else if (width == 1) {
    if (height > 1) {
      SAC_SDL2_draw_a_line( disp,
                            SDL2_DISP_X( disp) + xoffset,
                            SDL2_DISP_Y( disp) + yoffset,
                            SDL2_DISP_X( disp) + xoffset,
                            SDL2_DISP_Y( disp) + yoffset + height - 1,
                            rgb, TRUE, async);
    }
    else if (height == 1) {
      SAC_SDL2_draw_a_pixel( disp,
                             SDL2_DISP_X( disp) + xoffset,
                             SDL2_DISP_Y( disp) + yoffset,
                             SAC_SDL2_foreground_rgb);
      SAC_SDL2_update_rect( disp, 
                            SDL2_DISP_X( disp) + xoffset,
                            SDL2_DISP_Y( disp) + yoffset,
                            1, 1, TRUE);
    }
  }

  SAC_SDL2_unlock( disp);
}

static void fill_a_rect( SDL2* disp, int xoff, int yoff, int wid, int hei, int async)
{
  const Uint32 rgb = SAC_SDL2_foreground_rgb;
  const int    pitch = SDL2_PITCH( disp) / BYTES_PER_PIXEL;
  Uint32*      videomem = (Uint32 *) SDL2_PIXELS( disp);
  Uint32*      bptr;
  int          x, y;

  if (SDL2_DEBUG( disp)) {
    printf("fill_a_rect: [%d,%d]:[%d,%d], %d, 0x%x\n",
            xoff, yoff, wid, hei, async, rgb);
  }

  for (y = 0; y < hei; ++y) {
    bptr = &videomem[xoff + ((y + yoff) * pitch)];
    for (x = 0; x < wid; ++x, ++bptr) {
      *bptr = rgb;
    }
  }

  SAC_SDL2_update_rect( disp, xoff, yoff, wid, hei, async);
}

void SAC_SDL2_fill_rect( SDL2* disp, const int offsets[2], const int sizes[2],
                         int async)
{
  const int    width = sizes[1];
  const int    height = sizes[0];
  const int    xoffset = offsets[1];
  const int    yoffset = offsets[0];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_fill_rect: [%d,%d]:[%d,%d] on display [%d,%d]:[%d,%d]\n",
            When( disp), xoffset, yoffset, width, height,
            SDL2_DISP_X( disp), SDL2_DISP_Y( disp),
            SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       width < 0 || height < 0 ||
       xoffset + width  > SDL2_DISP_WIDTH( disp) ||
       yoffset + height > SDL2_DISP_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_fill_rect: Invalid offset [%d,%d] + "
                      "size [%d,%d] on display [%d,%d]:[%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_DISP_X( disp), SDL2_DISP_Y( disp),
                      SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else {
    fill_a_rect( disp,
                 SDL2_DISP_X( disp) + xoffset,
                 SDL2_DISP_Y( disp) + yoffset,
                 width, height, async);
  }
  SAC_SDL2_unlock( disp);
}

void SAC_SDL2_fill_disp( SDL2* disp, int async)
{
  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_fill_disp: [%d,%d]:[%d,%d]\n",
            When( disp), 
            SDL2_DISP_X( disp), SDL2_DISP_Y( disp),
            SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  fill_a_rect( disp,
               SDL2_DISP_X( disp),
               SDL2_DISP_Y( disp),
               SDL2_DISP_WIDTH( disp),
               SDL2_DISP_HEIGHT( disp),
               async);

  SAC_SDL2_unlock( disp);
}


#include "SDL2sac.h"

void SAC_SDL2_resize( SDL2* disp,
                      SAC_ND_PARAM_in( shp_nt, int))
{
  const int width  = SAC_ND_A_FIELD( shp_nt)[1];
  const int height = SAC_ND_A_FIELD( shp_nt)[0];
  SDL_Event event;
  SDL_Rect  rect;

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_resize: w=%d, h=%d\n", When( disp), width, height);
  }

  SAC_SDL2_lock( disp);

  if ( width < 0 || width > SHRT_MAX || height < 0 || height > SHRT_MAX)
  {
    SAC_RuntimeError( "SAC_SDL2_resize: invalid dimensions: [%d,%d]\n",
                      width, height);
  }
  else if (SDL2_ISROOT( disp))
  {
    event.type = SDL2_CREATE_EVENT;
    rect.w = width;
    rect.h = height;
    event.user.data1 = &rect;
    event.user.data2 = disp;
    SDL_PushEvent( &event);
    SAC_SDL2_wait( disp);
  }
  else if (width < 1 || height < 1 ||
           SDL2_DISP_X( disp) + width >
           SDL2_PARENT_X( disp) + SDL2_PARENT_WIDTH( disp) ||
           SDL2_DISP_Y( disp) + height >
           SDL2_PARENT_Y( disp) + SDL2_PARENT_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_resize: Invalid resize [%d,%d] at [%d,%d] "
                      "on parent space [%d,%d]\n",
                      width, height,
                      SDL2_DISP_X( disp) - SDL2_PARENT_X( disp),
                      SDL2_DISP_Y( disp) - SDL2_PARENT_Y( disp),
                      SDL2_PARENT_WIDTH( disp),
                      SDL2_PARENT_HEIGHT( disp));
  }
  else {
    SDL2_DISP_WIDTH( disp) = width;
    SDL2_DISP_HEIGHT( disp) = height;
  }

  SAC_SDL2_unlock( disp);

  SAC_ND_DEC_RC_FREE( shp_nt, 1, )
}


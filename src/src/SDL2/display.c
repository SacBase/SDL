#include "SDL2sac.h"
#include <limits.h>

static Uint32    SAC_SDL2_fullscreen_flag;
static Uint32    SAC_SDL2_noframe_flag;

void SAC_SDL2_enable_fullscreen( int enable)
{
  SAC_SDL2_fullscreen_flag = enable ? SDL_FULLSCREEN : 0;
}

void SAC_SDL2_enable_noframe( int enable)
{
  SAC_SDL2_noframe_flag = enable ? SDL_NOFRAME : 0;
}

void SAC_SDL2_create_display_event( SDL_Event* event)
{
  SDL_Rect* rect = (SDL_Rect *) event->user.data1;
  SDL2*     disp = (SDL2 *) event->user.data2;
  Uint32    flags = DEFAULT_VID_MOD | SAC_SDL2_fullscreen_flag |
                    SAC_SDL2_noframe_flag;

  SDL2_SURF( disp) = SDL_SetVideoMode( rect->w, rect->h, BITS_PER_PIXEL, flags);
  if ( SDL2_SURF( disp) == NULL) {
    SAC_RuntimeError( "SAC_SDL2_display: failed SDL_SetVideoMode: %s",
                      SDL_GetError());
  }
  else {
    SAC_SDL2_default_heading();
    SAC_SDL2_cursor( 0);
  }

  SAC_SDL2_post( disp);
}

void SAC_SDL2_display( SDL2** disp_ptr,
                       SAC_ND_PARAM_in( shp_nt, int))
{
  const int width  = SAC_ND_A_FIELD( shp_nt)[1];
  const int height = SAC_ND_A_FIELD( shp_nt)[0];
  SDL_Event event;
  SDL_Rect  rect;
  SDL2*     disp = NULL;

  if (getenv("SDL2_DEBUG")) {
    printf("%sSAC_SDL2_display: w=%d, h=%d\n", When( disp), width, height);
  }

  if ( width < 0 || width > SHRT_MAX || height < 0 || height > SHRT_MAX)
  {
    SAC_RuntimeError( "SAC_SDL2_display: invalid dimensions: [%d,%d]\n",
                      width, height);
  }
  else {
    disp = SAC_SDL2_alloc_disp( width, height);
    if ( SAC_SDL2_init( disp) == 0) {
      event.type = SDL2_CREATE_EVENT;
      rect.w = width;
      rect.h = height;
      event.user.data1 = &rect;
      event.user.data2 = disp;
      SDL_PushEvent( &event);
      SAC_SDL2_wait( disp);
      SDL2_DISP_WIDTH( disp) = SDL2_SURF_WIDTH( disp);
      SDL2_DISP_HEIGHT( disp) = SDL2_SURF_HEIGHT( disp);
      SAC_SDL2_unlock( disp);
    }
    else {
      SAC_SDL2_free_disp( disp);
      disp = NULL;
    }
  }

  *disp_ptr = disp;

  SAC_ND_DEC_RC_FREE( shp_nt, 1, )
}


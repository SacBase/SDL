#include "SDL2sac.h"

void SAC_SDL2_update_display_event( SDL_Event* event)
{
  SDL_Rect* rect = event->user.data1;
  SDL2*     disp = event->user.data2;

  SDL_UpdateRect( SDL2_SURF( disp), rect->x, rect->y, rect->w, rect->h);

  if (event->type == SDL2_UPDATE_ASYNC_EVENT) {
    free( rect);
  }
  else {
    SAC_SDL2_post( disp);
  }
}

/*
 * Receive coordinates in true surface area.
 */
void SAC_SDL2_update_rect( SDL2* disp, int x, int y, int w, int h, int async)
{
  SDL_Event event;
  SDL_Rect  rect;
  SDL_Rect* rect_ptr;

  if (async) {
    event.type = SDL2_UPDATE_ASYNC_EVENT;
    rect_ptr = (SDL_Rect *) SAC_MALLOC( sizeof( SDL_Rect));
    rect_ptr->x = x;
    rect_ptr->y = y;
    rect_ptr->w = w;
    rect_ptr->h = h;
    event.user.data1 = rect_ptr;
    event.user.data2 = disp;
    SDL_PushEvent( &event);
  }
  else {
    event.type = SDL2_UPDATE_EVENT;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    event.user.data1 = &rect;
    event.user.data2 = disp;
    SDL_PushEvent( &event);
    SAC_SDL2_wait( disp);
  }
}

void SAC_SDL2_update( SDL2* disp, int async)
{
  int offsets[2];
  int sizes[2];
  
  SAC_SDL2_check( disp);
  offsets[1] = 0;
  offsets[0] = 0;
  sizes[1] = SDL2_DISP_WIDTH( disp);
  sizes[0] = SDL2_DISP_HEIGHT( disp);
  SAC_SDL2_update2( disp, offsets, sizes, async);
}

void SAC_SDL2_update2( SDL2* disp, int offsets[2], int sizes[2], int async)
{
  const int width = sizes[1];
  const int height = sizes[0];
  const int xoffset = offsets[1];
  const int yoffset = offsets[0];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_update2: [%d,%d]:[%d,%d] on [%d,%d]\n",
            When( disp), xoffset, yoffset, width, height,
            SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       xoffset + width  > SDL2_DISP_WIDTH( disp) ||
       yoffset + height > SDL2_DISP_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_update2: Offset [%d,%d] + array [%d,%d] "
                      "exceeds display [%d,%d]\n",
                      xoffset, yoffset, width, height,
                      SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else {
    SAC_SDL2_update_rect( disp,
                          SDL2_DISP_X( disp) + xoffset,
                          SDL2_DISP_Y( disp) + yoffset,
                          width, height, async);
  }

  SAC_SDL2_unlock( disp);
}


#include "SDL2sac.h"

static int do_event( SDL_Event* event, SDL2* disp)
{
  int done = 0;

  if (event->type != SDL_MOUSEMOTION &&
      event->type != SDL_ACTIVEEVENT)
  {
    if (SDL2_DEBUG( disp)) {
      printf("%sSDL2 got event: %s\n", When( NULL),
             SAC_SDL2_event_name( event->type));
    }
  }
  switch (event->type) {
    case SDL_ACTIVEEVENT:
      if (SDL2_DEBUG( disp)) {
        printf("%sSDL2 active event: %s, %s\n", When( NULL),
              event->active.gain ? "gain" : "loss",
              event->active.state == SDL_APPMOUSEFOCUS ? "mouse " :
              event->active.state == SDL_APPINPUTFOCUS ? "input " :
              event->active.state == SDL_APPACTIVE     ? "active" :
              "unknown");
      }
      break;

    case SDL_QUIT:
      SDL_Quit();
      exit(0);
      break;

    case SDL_KEYDOWN:
      break;

    case SDL_KEYUP:
      break;

    case SDL_VIDEORESIZE:
      break;

    case SDL_MOUSEBUTTONDOWN:
      SAC_SDL2_mouse_down_event( event);
      break;

    case SDL_MOUSEBUTTONUP:
      SAC_SDL2_mouse_up_event( event);
      break;

    case SDL_MOUSEMOTION:
      SAC_SDL2_mouse_motion_event( event);
      break;

    case SDL_VIDEOEXPOSE:
      break;

    case SDL2_CREATE_EVENT:
      SAC_SDL2_create_display_event( event);
      break;

    case SDL2_TEARDOWN_EVENT:
      SAC_SDL2_teardown_event( event);
      done = 1;
      break;

    case SDL2_UPDATE_EVENT:
    case SDL2_UPDATE_ASYNC_EVENT:
      SAC_SDL2_update_display_event( event);
      break;

    default:
      SAC_RuntimeError( "SDL2_event_loop: unknown event no %d\n", event->type);
  }

  return done;
}

void SAC_SDL2_event_loop( SDL2* disp)
{
  SDL_Event event;
  int done = 0;

  while ( ! done) {
    if (SDL_WaitEvent( &event) == 1) {
      done = do_event( &event, disp);
    }
  }
}


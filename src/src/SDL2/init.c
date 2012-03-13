#include "SDL2sac.h"

enum SDL2_init_states {
  NotInited,
  Started,
  Completed,
};

static volatile int inited;

void SAC_SDL2_check( SDL2* disp)
{
  if (inited != Completed) {
    SAC_RuntimeError( "SAC_SDL2_check: SDL2 module is not initialized");
  }
  if ( ! disp || ! SDL2_CHECK( disp)) {
    SAC_RuntimeError( "SAC_SDL2_check: invalid SDL2 display structure");
  }
}

int SAC_SDL2_init( SDL2* disp)
{
  int r = 0;

  if (inited == NotInited && ++inited == Started) {
    if (SAC_SDL2_setup( disp)) {
      SAC_RuntimeError( "SAC_SDL2_init: failed to init SDL: %s\n",
                        SDL_GetError());
      r = -1;
      inited = NotInited;
    } else {
      inited = Completed;
    }
  }
  else {
    SAC_RuntimeError( "SAC_SDL2_init: called twice (%d)\n", inited + 1);
  }

  return r;
}

void SAC_SDL2_stop( SDL2* disp)
{
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_stop()\n", When( disp));
  }

  if (inited == Completed) {
    SAC_SDL2_teardown( disp);
    inited = NotInited;
  }
}


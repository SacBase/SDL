#include "SDL2sac.h"

/*
 * Close a display or a sub-window.
 */
void SAC_SDL2_close( SDL2* disp)
{
  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_close: %p\n", When( disp), (void *)disp);
  }

  if (SDL2_ISROOT( disp)) {
    SAC_SDL2_lock( disp);
    SAC_SDL2_stop( disp);
    SAC_SDL2_free_disp( disp);
  }
  else {
    SDL2* parent = SDL2_PARENT( disp);
    SAC_SDL2_lock( parent);
    SAC_SDL2_free_disp( disp);
    SAC_SDL2_unlock( parent);
  }
}


#include "SDL2sac.h"

void SAC_SDL2_close( SDL2* disp)
{
  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_close: %p\n", When( disp), disp);
  }

  SAC_SDL2_lock( disp);

  if (SDL2_ISROOT( disp)) {
    SAC_SDL2_stop( disp);
  }
  else {
    SAC_SDL2_unlock( disp);
  }

  SAC_SDL2_free_disp( disp);
}


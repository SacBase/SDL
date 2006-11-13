#include "SDLsac.h"
#include <stdlib.h>
#include <unistd.h>

void destroyDisplay( SAC_ND_PARAM_in_nodesc( disp_nt, Display))
{
  if (SDLsac_eventhandler != NULL) {
    SDL_KillThread( SDLsac_eventhandler);
  }

  if (SDLsac_mutex != NULL) {
    SDL_DestroyMutex( SDLsac_mutex);
  }

  if (SDLsac_timer != NULL) {
    SDL_RemoveTimer( SDLsac_timer);
  }
}

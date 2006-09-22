#include "SDLsac.h"
#include <stdlib.h>
#include <unistd.h>

void destroyDisplay( SAC_ND_PARAM_out( scalres_nt, int),
                     SAC_ND_PARAM_in_nodesc( disp_nt, Display))
{
  SAC_ND_DECL__DATA( scalar_nt, int, );
  SAC_ND_DECL__DESC( scalar_nt, );

  SAC_ND_CREATE__SCALAR__DATA( scalar_nt, 0);

  if (SDLsac_eventhandler != NULL) {
    SDL_KillThread( SDLsac_eventhandler);
  }

  if (SDLsac_mutex != NULL) {
    SDL_DestroyMutex( SDLsac_mutex);
  }

  if (SDLsac_timer != NULL) {
    SDL_RemoveTimer( SDLsac_timer);
  }

  SAC_ND_RET_out( scalres_nt, scalar_nt);
}

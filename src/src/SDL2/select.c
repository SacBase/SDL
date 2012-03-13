#include "SDL2sac.h"

void SAC_SDL2_select( SAC_ND_PARAM_out_nodesc( aks_out_nt, int), SDL2* disp)
{
  SAC_ND_DECL__DATA( aks_nt, int, );
  int sel[4];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL_select\n", When( disp));
  }

  SAC_SDL2_lock( disp);

  SAC_SDL2_select_heading();
  SAC_SDL2_cursor( 1);

  SAC_SDL2_start_selection( disp);
  SAC_SDL2_wait( disp);
  
  SAC_SDL2_default_heading();
  SAC_SDL2_cursor( 0);

  SAC_SDL2_get_selection( sel);

  /*
   * write back result, ensure that we have [topleft, bottomright] and
   * [Y,X] coordinates
   */
  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 4);
  SAC_ND_A_FIELD( aks_nt)[1] = MIN(sel[0], sel[2]);
  SAC_ND_A_FIELD( aks_nt)[3] = MAX(sel[0], sel[2]);
  SAC_ND_A_FIELD( aks_nt)[0] = MIN(sel[1], sel[3]);
  SAC_ND_A_FIELD( aks_nt)[2] = MAX(sel[1], sel[3]);

  SAC_SDL2_unlock( disp);

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}

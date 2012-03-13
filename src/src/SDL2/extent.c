#include "SDL2sac.h"

void SAC_SDL2_extent( SAC_ND_PARAM_out_nodesc( aks_out_nt, int), SDL2* disp)
{
  SAC_ND_DECL__DATA( aks_nt, int, );

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_extent: %p\n", When( disp), disp);
  }

  /*
   * create result and write back with [Y,X] as result
   */
  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 2);
  SAC_ND_A_FIELD( aks_nt)[1] = SDL2_DISP_WIDTH( disp);
  SAC_ND_A_FIELD( aks_nt)[0] = SDL2_DISP_HEIGHT( disp);

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}



#include "SDLsac.h"

void SAC_SDL_getExtent( SAC_ND_PARAM_out_nodesc( aks_out_nt, int),
                        SAC_ND_PARAM_inout_nodesc_bx( disp_nt, SDLdisplay))
{
  SAC_ND_DECL__DATA( aks_nt, int, );
  
  /*
   * create result and write back with [Y,X] as result
   */
  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 2);
  SAC_ND_A_FIELD( aks_nt)[0] = NT_NAME( disp_nt)->h;
  SAC_ND_A_FIELD( aks_nt)[1] = NT_NAME( disp_nt)->w;

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}

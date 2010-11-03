
#include "SDLsac.h"

void SAC_SDL_getSelection( SAC_ND_PARAM_out_nodesc( aks_out_nt, int),
                           SAC_ND_PARAM_inout_nodesc_bx( disp_nt, Display))
{
  SAC_ND_DECL__DATA( aks_nt, int, );

  /*
   * update window title and cursor to inform user of select mode 
   */
  SDL_WM_SetCaption( SDL_SAC_SELECT_HEADING, NULL);
  SDL_ShowCursor( SDL_ENABLE);

  /*
   * turn on select mode
   */
  SDLsac_selmode = SEL_top;

  /*
   * wait for result
   */
  SDL_SemWait( SDLsac_selectsem);
  
  /*
   * resume normal cursor and title
   */
  SDL_WM_SetCaption( SDL_SAC_DEFAULT_HEADING, NULL);
  SDL_ShowCursor( SDL_DISABLE);
  
  /*
   * write back result, ensure that we have [topleft, bottomrigth] and
   * [Y,X] coordinates
   */
  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 4);
  if (SDLsac_selection[0] <= SDLsac_selection[2]) {
    SAC_ND_A_FIELD( aks_nt)[1] = SDLsac_selection[0];
    SAC_ND_A_FIELD( aks_nt)[3] = SDLsac_selection[2];
  } else {
    SAC_ND_A_FIELD( aks_nt)[1] = SDLsac_selection[2];
    SAC_ND_A_FIELD( aks_nt)[3] = SDLsac_selection[0];
  }
  if (SDLsac_selection[1] <= SDLsac_selection[3]) {
    SAC_ND_A_FIELD( aks_nt)[0] = SDLsac_selection[1];
    SAC_ND_A_FIELD( aks_nt)[2] = SDLsac_selection[3];
  } else {
    SAC_ND_A_FIELD( aks_nt)[0] = SDLsac_selection[3];
    SAC_ND_A_FIELD( aks_nt)[2] = SDLsac_selection[1];
  }

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}

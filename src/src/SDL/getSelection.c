
#include "SDLsac.h"

void SAC_SDL_getSelection( SAC_ND_PARAM_out_nodesc( select_out_nt, int),
                           SAC_ND_PARAM_inout_nodesc_bx( disp_nt, Display))
{
  SAC_ND_DECL__DATA( select_nt, int, );
  int cnt;

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
   * write back result
   */
  SAC_ND_A_FIELD( select_nt) = SAC_MALLOC( sizeof( int) * 4);
  for (cnt = 0; cnt < 4; cnt++) {
    SAC_ND_A_FIELD( select_nt)[cnt] = SDLsac_selection[cnt];
  }

  SAC_ND_RET_out__NODESC( select_out_nt, select_nt);
}

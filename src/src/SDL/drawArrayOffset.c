#include "SDLsac.h"

#include <unistd.h>

void SAC_SDL_drawArrayOffset( 
       SAC_ND_PARAM_inout_nodesc_bx( disp_nt, SDLdisplay),
       SAC_ND_PARAM_in( ar_nt, int),
       int pic_offset[2]
     )
{
  int xaxis, yaxis, aroffset, screenoffset, xoffset, yoffset;

  /*
   * accessing the display needs to be mutually exclusive
   */
  if (SDL_mutexP( SDLsac_mutex)==-1){
    SAC_RuntimeError( "Failed to lock the access mutex");
  }

  xoffset = pic_offset[1];
  yoffset = pic_offset[0];

  /*
   * check bounds
   */


  if ( xoffset < 0 || yoffset < 0 ||
       (SAC_ND_A_DESC_SHAPE( ar_nt, 1)+xoffset > NT_NAME( disp_nt)->w) ||
       (SAC_ND_A_DESC_SHAPE( ar_nt, 0)+yoffset > NT_NAME( disp_nt)->h) ) {
    SAC_RuntimeError( "Cannot draw array of shape [ %d, %d] starting \n"
                      "***at [%d,%d] on display of size [ %d, %d] ! \n",
                      SAC_ND_A_DESC_SHAPE( ar_nt, 0),
                      SAC_ND_A_DESC_SHAPE( ar_nt, 1),
                      xoffset,
                      yoffset,
                      NT_NAME( disp_nt)->w,
                      NT_NAME( disp_nt)->h);
  }


  /*
   * lock the screen for drawing
   */
  if (SDL_MUSTLOCK( NT_NAME( disp_nt))) {
    if (SDL_LockSurface( NT_NAME( disp_nt)) < 0) {
      SAC_RuntimeError( "Failed to lock the SDL Display");
    }
  }

  /*
   * draw
   */
  aroffset = 0;
  for (yaxis = yoffset; 
       yaxis < SAC_ND_A_DESC_SHAPE( ar_nt, 0)+yoffset; yaxis ++) {
    
    screenoffset = yaxis * NT_NAME( disp_nt)->pitch / 4;

    for (xaxis = xoffset; 
         xaxis < SAC_ND_A_DESC_SHAPE( ar_nt, 1)+xoffset; xaxis ++) {
      Uint32 *bptr = (Uint32 *) NT_NAME( disp_nt)->pixels 
        + screenoffset + xaxis;

      *bptr = ( ((Uint32)SAC_ND_A_FIELD( ar_nt)[aroffset]) << 16
                | ((Uint32)SAC_ND_A_FIELD( ar_nt)[aroffset+1]) << 8
                | ((Uint32)SAC_ND_A_FIELD( ar_nt)[aroffset+2]) 
              );
      aroffset+=3;
    }
  }

  /*
   * unlock it
   */
  if (SDL_MUSTLOCK( NT_NAME( disp_nt))) {
    SDL_UnlockSurface( NT_NAME( disp_nt));
  }

  if( !SDLsac_isasync) {
    SDL_UpdateRect( NT_NAME( disp_nt), xoffset, yoffset, 
        SAC_ND_A_DESC_SHAPE( ar_nt, 1),
        SAC_ND_A_DESC_SHAPE( ar_nt, 0));
  }

  /*
   * accessing the display needs to be mutually exclusive
   */
  if (SDL_mutexV( SDLsac_mutex)==-1){
    SAC_RuntimeError( "Failed to unlock the access mutex");
  }

  SAC_ND_DEC_RC_FREE( ar_nt, 1, )
}
           

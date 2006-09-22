#include "SDLsac.h"

void drawPixel( SAC_ND_PARAM_out_nodesc( dispout_nt, Display),
                SAC_ND_PARAM_in_nodesc( dispin_nt, Display),
                SAC_ND_PARAM_in( shp_nt, int),
                SAC_ND_PARAM_in( color_nt, int))
{
  int xoffset, yoffset;
  Uint32 *bptr;

  /*
   * accessing the display needs to be mutually exclusive
   */
  if (SDL_mutexP( SDLsac_mutex)==-1){
    SAC_RuntimeError( "Failed to lock the access mutex");
  }

  /*
   * check bounds
   */
  if ( (SAC_ND_A_FIELD( shp_nt)[1] < 0) ||
       (SAC_ND_A_FIELD( shp_nt)[1] >= NT_NAME( dispin_nt)->w) ||
       (SAC_ND_A_FIELD( shp_nt)[0] < 0) ||
       (SAC_ND_A_FIELD( shp_nt)[0] >= NT_NAME( dispin_nt)->h) ) {
    SAC_RuntimeError( "Cannot draw pixel at position [%d/%d]:\n"
                      "*** pixel out of bounds !\n", 
                      SAC_ND_A_FIELD( shp_nt)[1],
                      SAC_ND_A_FIELD( shp_nt)[0]);
  }

  /*
   * lock the screen for drawing
   */
  if (SDL_MUSTLOCK( NT_NAME( dispin_nt))) {
    if (SDL_LockSurface( NT_NAME( dispin_nt)) < 0) {
      SAC_RuntimeError( "Failed to lock the SDL Display");
    }
  }

  /*
   * draw
   */
  yoffset = SAC_ND_A_FIELD( shp_nt)[0] * (NT_NAME( dispin_nt)->pitch / 4);
  xoffset = SAC_ND_A_FIELD( shp_nt)[1];

  bptr = (Uint32 *) (NT_NAME( dispin_nt)->pixels) + xoffset + yoffset;
  *bptr = ( ((Uint32)SAC_ND_A_FIELD( color_nt)[0])   << 16
            | ((Uint32)SAC_ND_A_FIELD( color_nt)[1]) << 8
            | ((Uint32)SAC_ND_A_FIELD( color_nt)[2])
            );

  /*
   * unlock it
   */
  if (SDL_MUSTLOCK( NT_NAME( dispin_nt))) {
    SDL_UnlockSurface( NT_NAME( dispin_nt));
  }

  /*
   * accessing the display needs to be mutually exclusive
   */
  if (SDL_mutexV( SDLsac_mutex)==-1){
    SAC_RuntimeError( "Failed to unlock the access mutex");
  }

  /*
   * return the Display
   */
  * SAC_NAMEP( SAC_ND_A_FIELD( dispout_nt)) = SAC_ND_A_FIELD( dispin_nt);
  
  SAC_ND_DEC_RC_FREE( shp_nt, 1, )
  SAC_ND_DEC_RC_FREE( color_nt, 1, )
}
           

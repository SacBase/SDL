#include "SDLsac.h"
#include <stdlib.h>
#include <unistd.h>

#define ADAPTIVE_MODE
#define MIN_UPDATE_INTERVAL 50
#define START_UPDATE_INTERVAL 50

SDL_Thread *SDLsac_eventhandler = NULL;
SDL_mutex *SDLsac_mutex = NULL;
SDL_TimerID SDLsac_timer = NULL;
SDL_sem *SDLsac_updatesem = NULL;

bool SDLsac_isasync;

static
void updateScreen( SDL_Surface  *surface)
{
  /*
   * accessing the display needs to be mutually exclusive
   */
  if (SDL_mutexP( SDLsac_mutex)==-1){
    SAC_RuntimeError( "Failed to lock the access mutex");
  }

  SDL_UpdateRect( surface, 0, 0, 0, 0);

  /*
   * accessing the display needs to be mutually exclusive
   */
  if (SDL_mutexV( SDLsac_mutex)==-1){
    SAC_RuntimeError( "Failed to unlock the access mutex");
  }
}

static 
int EventHandler( void *data)
{
  SDL_Event event;
  int done = 0;

  while ( !done) {
    if (SDL_WaitEvent( &event) == 1) {
      switch (event.type) {
        case SDL_QUIT:
          /* someone clicked the close button on the window */
          SDL_Quit();
          exit(10);
          break;

        case SDL_USEREVENT_DRAW:
          updateScreen( (SDL_Surface *) event.user.data1);
          break;

        case SDL_USEREVENT_QUIT:
          done = 1;
          break;

        default:
          break;
      }
    }
  }

  return 0;
}

static
Uint32 TimerHandler(Uint32 interval, void *param) {
  SDL_Event event;
#ifdef ADAPTIVE_MODE
  int eventqueue;
#endif

#ifdef ADAPTIVE_MODE
  eventqueue = SDL_PeepEvents( &event, 
                               1, 
                               SDL_PEEKEVENT, 
                               SDL_EVENTMASK( SDL_USEREVENT_DRAW));

  if (eventqueue == 1) {
    /* an event overtook us, so we are too fast */
    interval *= 2;
  } else if (interval > MIN_UPDATE_INTERVAL + 10) {
    /* nothing waiting, we can speed up :-) */
    interval -= 10;
  }
#endif

  event.type = SDL_USEREVENT_DRAW;
  event.user.code = 0;
  event.user.data1 = param;
  event.user.data2 = NULL;

  SDL_PushEvent(&event);

  return(interval);
}


void initDisplay( SAC_ND_PARAM_out_nodesc( disp_nt, Display),
                  SAC_ND_PARAM_in( shp_nt, int),
                  SAC_ND_PARAM_in( async_nt, bool))
{
  SAC_ND_DECL__DATA( disp_nt, Display, )

  if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    SAC_RuntimeError( "Failed to init SDL System: %s", SDL_GetError());
  }

  SAC_ND_A_FIELD( disp_nt) = 
    SDL_SetVideoMode( SAC_ND_A_FIELD( shp_nt)[1], 
                      SAC_ND_A_FIELD( shp_nt)[0], 
                      32, SDL_HWSURFACE | SDL_ASYNCBLIT );

  if (SAC_ND_A_FIELD( disp_nt) == NULL) {
    SAC_RuntimeError( "Failed to init SDL Display: %s", SDL_GetError());
  }

  SDL_WM_SetCaption( "SaC SDL Output", NULL);

  SDLsac_isasync = SAC_ND_A_FIELD( async_nt);

  /*
   * a shiny mutex 
   */
  SDLsac_mutex = SDL_CreateMutex();

  if( SDLsac_isasync) {
    /*
     * register an event handler 
     */ 
    SDLsac_eventhandler = SDL_CreateThread( EventHandler, NULL);
  
    /*
     * start a display update timer 
     */
    SDLsac_timer = SDL_AddTimer( START_UPDATE_INTERVAL, 
                                 TimerHandler, 
                                 SAC_ND_A_FIELD( disp_nt));
    if ( SDLsac_timer == NULL) {
      SAC_RuntimeError( "Failed to init update timer");
    }
  } 

  * SAC_NAMEP( SAC_ND_A_FIELD( disp_nt)) = SAC_ND_A_FIELD( disp_nt);
  
  SAC_ND_DEC_RC_FREE( shp_nt, 1, )
}

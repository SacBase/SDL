#include "SDLsac.h"
#include <stdlib.h>
#include <unistd.h>

#define MIN_UPDATE_INTERVAL 50
#define START_UPDATE_INTERVAL 200

SDL_Thread *SDLsac_eventhandler = NULL;
SDL_mutex *SDLsac_mutex = NULL;
SDL_TimerID SDLsac_timer = NULL;

#ifdef UPDATE_VIA_SEMAPHORE
SDL_sem *SDLsac_updatesem = NULL;
SDL_Thread *SDLsac_updater = NULL;
#endif /* UPDATE_VIA_SEMAPHORE */

bool SDLsac_isasync;

static
int UpdateScreen( void *surface)
{
#ifdef UPDATE_VIA_SEMAPHORE
  while (1) {
    /*
     * wait for update event 
     */
    if (SDL_SemWait( SDLsac_updatesem) == 0) {
      
#endif /* UPDATE_VIA_SEMAPHORE */
      /*
       * accessing the display needs to be mutually exclusive
       */
      if (SDL_mutexP( SDLsac_mutex)==-1){
        SAC_RuntimeError( "Failed to lock the access mutex");
      }

      SDL_Flip( (SDL_Surface *) surface);

      /*
       * accessing the display needs to be mutually exclusive
       */
      if (SDL_mutexV( SDLsac_mutex)==-1){
        SAC_RuntimeError( "Failed to unlock the access mutex");
      }
#ifdef UPDATE_VIA_SEMAPHORE
    }
  }
#endif /* UPDATE_VIA_SEMAPHORE */

  return 0;
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
#ifdef UPDATE_VIA_SEMAPHORE
          SDL_SemPost( SDLsac_updatesem);
#else /* UPDATE_VIA_SEMAPHORE */
          UpdateScreen( (SDL_Surface *) event.user.data1);
#endif /* UPDATE_VIA_SEMAPHORE */
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
#ifdef UPDATE_VIA_SEMAPHORE
  eventqueue = SDL_SemValue( SDLsac_updatesem);
#else /* UPDATE_VIA_SEMAPHORE */
  eventqueue = SDL_PeepEvents( &event, 
                               1, 
                               SDL_PEEKEVENT, 
                               SDL_EVENTMASK( SDL_USEREVENT_DRAW));
#endif /* UPDATE_VIA_SEMAPHORE */

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
                      32, SDL_SWSURFACE);

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
#ifdef UPDATE_VIA_SEMAPHORE
    /*
     * create a semaphore
     */
    SDLsac_updatesem = SDL_CreateSemaphore( 0);
    if (SDLsac_updatesem == NULL) {
      SAC_RuntimeError( "Failed to init update semaphore");
    }

    /*
     * create the update thread
     */
    SDLsac_updater = SDL_CreateThread( UpdateScreen, SAC_ND_A_FIELD( disp_nt));
#endif /* UPDATE_VIA_SEMAPHORE */
  
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

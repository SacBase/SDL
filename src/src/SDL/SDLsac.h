#ifndef _SDLSAC_H_
#define _SDLSAC_H_

#include "sac.h"
#include "SDL.h"
#include "SDL_thread.h"

#undef UPDATE_VIA_SEMAPHORE
#define ADAPTIVE_MODE

typedef SDL_Surface* Display;

extern SDL_Thread *SDLsac_eventhandler;
extern SDL_mutex *SDLsac_mutex;
extern SDL_TimerID SDLsac_timer;

#ifdef UPDATE_VIA_SEMAPHORE
extern SDL_sem *SDLsac_updatesem;
extern SDL_Thread *SDLsac_updater;
#endif /* UPDATE_VIA_SEMAPHORE */

extern bool SDLsac_isasync;

#define SDL_USEREVENT_DRAW (SDL_USEREVENT + 1)
#define SDL_USEREVENT_QUIT (SDL_USEREVENT + 2)

#define disp_nt (disp, T_OLD((SCL, (HID, (NUQ,)))))
#define disp_nt (disp, T_OLD((SCL, (HID, (NUQ,)))))
#define shp_nt  (shp, T_OLD((AKD, (NHD, (NUQ,)))))
#define ar_nt   (ar, T_OLD((AKD, (NHD, (NUQ,)))))
#define color_nt (col, T_OLD((AKS, (NHD, (NUQ,)))))
#define scalar_nt (scl, T_OLD((SCL, (NHD, (NUQ,)))))
#define scalres_nt (sclres, T_OLD((SCL, (NHD, (NUQ,)))))
#define async_nt (async, T_OLD((SCL, (NHD, (NUQ,)))))

#endif

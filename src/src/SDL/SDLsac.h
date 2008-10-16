#ifndef _SDLSAC_H_
#define _SDLSAC_H_

#include "sac.h"
#include "SDL.h"
#include "SDL_thread.h"

typedef SDL_Surface* Display;

extern SDL_Thread *SDLsac_eventhandler;
extern SDL_mutex *SDLsac_mutex;
extern SDL_TimerID SDLsac_timer;

extern bool SDLsac_isasync;

#define disp_nt (disp, (SCL, (HID, (NUQ,))))
#define disp_nt (disp, (SCL, (HID, (NUQ,))))
#define shp_nt  (shp, (AKD, (NHD, (NUQ,))))
#define ar_nt   (ar, (AKD, (NHD, (NUQ,))))
#define color_nt (col, (AKS, (NHD, (NUQ,))))
#define scalar_nt (scl, (SCL, (NHD, (NUQ,))))
#define scalres_nt (sclres, (SCL, (NHD, (NUQ,))))
#define async_nt (async, (SCL, (NHD, (NUQ,))))

#endif

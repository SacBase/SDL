#ifndef _SDL2SAC_H_
#define _SDL2SAC_H_

#include "sac.h"
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL2data.h"

#define disp_nt       (disp, T_OLD((SCL, (HID, (NUQ,)))))
#define shp_nt        (shp, T_OLD((AKS, (NHD, (NUQ,)))))
#define ar_nt         (ar, T_OLD((AKD, (NHD, (NUQ,)))))
#define color_nt      (col, T_OLD((AKS, (NHD, (NUQ,)))))
#define async_nt      (async, T_OLD((SCL, (NHD, (NUQ,)))))
#define aks_out_nt    (aks_out, T_OLD((AKS, (NHD, (NUQ,)))))
#define aks_nt        (aks, T_OLD((AKS, (NHD, (NUQ,)))))

#include "SDL2proto.h"

#endif

#ifndef SDL2DATA_H
#define SDL2DATA_H

/* Macros for min/max.  */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

/* semaphores */

#include <semaphore.h>

#define SEM_SHARE_THREAD       0
#define SEM_SHARE_PROC         1
#define SEM_INIT_LOCKED        0
#define SEM_INIT_UNLOCKED      1

/* Simple DirectMedia Layer */

#define DEFAULT_BPP     32
#define DEFAULT_VID_MOD (SDL_SWSURFACE)

/* SDL gives USEREVENT+0 to USEREVENT+7 for users to define: */
#define SDL2_CREATE_EVENT       (SDL_USEREVENT + 1)
#define SDL2_TEARDOWN_EVENT     (SDL_USEREVENT + 2)
#define SDL2_UPDATE_EVENT       (SDL_USEREVENT + 3)
#define SDL2_UPDATE_ASYNC_EVENT (SDL_USEREVENT + 4)

/* SDL2 structure */

#define SDL2_MAGIC_NUMBER 0x31415926

typedef struct SDL2 SDL2;
struct SDL2 {
  unsigned int magic;
  unsigned int debug;
  unsigned int locked;
  sem_t*       lock;
  sem_t*       sem;
  SDL_Surface* surf;
};

#define SDL2_CHECK(s)     (SDL2_MAGIC(s) == SDL2_MAGIC_NUMBER)
#define SDL2_MAGIC(s)     ((s)->magic)
#define SDL2_DEBUG(s)     ((s)->debug)
#define SDL2_LOCK(s)      ((s)->lock)
#define SDL2_LOCKED(s)    ((s)->locked)
#define SDL2_SEM(s)       ((s)->sem)
#define SDL2_SURF(s)      ((s)->surf)
#define SDL2_WIDTH(s)     ((s)->surf->w)
#define SDL2_HEIGHT(s)    ((s)->surf->h)
#define SDL2_PITCH(s)     ((s)->surf->pitch)
#define SDL2_PIXELS(s)    ((s)->surf->pixels)

/* SDL2 user events */

enum SDL2_user_code {
  SDL2_CREATE_DISPLAY = 1,
  SDL2_UPDATE_DISPLAY,
  SDL2_TEARDOWN,
};

/* Window manager frame title */

#define SAC_SDL2_DEFAULT_HEADING "SaC SDL Display"
#define SAC_SDL2_SELECT_HEADING "Click and drag to select an area, button-two click to cancel..."

#endif

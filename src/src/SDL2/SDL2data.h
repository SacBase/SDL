#ifndef SDL2DATA_H
#define SDL2DATA_H

/* Boolean values. */
#define TRUE    1
#define FALSE   0

/* Macros for min/max from <sys/param.h>.  */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Absolute difference between two scalars. */
#define DELTA(a,b)      (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

/* Restrict scalar 'a' to a range with lower boundary 'b' and upper 'c'. */
#define LIMIT(a,b,c)    ((a) = ((a) < (b)) ? (b) : ((a) > (c)) ? (c) : (a))

/* Swap two values. */
#define SWAP2(type,v1,v2)      do{type _ = (v1); (v1) = (v2); (v2) = _;}while(0)

/* semaphores */

#include <semaphore.h>

#define SEM_SHARE_THREAD       0
#define SEM_SHARE_PROC         1
#define SEM_INIT_LOCKED        0
#define SEM_INIT_UNLOCKED      1

/* Simple DirectMedia Layer */

#define BITS_PER_PIXEL         32
#define BYTES_PER_PIXEL        4
#define DEFAULT_VID_MOD        (SDL_SWSURFACE)
#define RGB(r,g,b)             ((Uint32)(r)<<16)|((Uint32)(g)<<8)|((Uint32)(b))

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
  sem_t*       lock;
  sem_t*       sem;
  SDL_Surface* surf;
  SDL2*        parent;
  SDL2*        root;
  int          x;
  int          y;
  int          width;
  int          height;
};

#define SDL2_MAGIC(s)         ((s)->magic)
#define SDL2_DEBUG(s)         ((s)->debug)
#define SDL2_LOCK(s)          ((s)->lock)
#define SDL2_SEM(s)           ((s)->sem)
#define SDL2_SURF(s)          ((s)->surf)
#define SDL2_SURF_WIDTH(s)    ((s)->surf->w)
#define SDL2_SURF_HEIGHT(s)   ((s)->surf->h)
#define SDL2_PITCH(s)         ((s)->surf->pitch)
#define SDL2_PIXELS(s)        ((s)->surf->pixels)
#define SDL2_PARENT(s)        ((s)->parent)
#define SDL2_ROOT(s)          ((s)->root)
#define SDL2_DISP_X(s)        ((s)->x)
#define SDL2_DISP_Y(s)        ((s)->y)
#define SDL2_DISP_WIDTH(s)    ((s)->width)
#define SDL2_DISP_HEIGHT(s)   ((s)->height)
#define SDL2_PARENT_X(s)      SDL2_DISP_X(SDL2_PARENT(s))
#define SDL2_PARENT_Y(s)      SDL2_DISP_Y(SDL2_PARENT(s))
#define SDL2_PARENT_WIDTH(s)  SDL2_DISP_WIDTH(SDL2_PARENT(s))
#define SDL2_PARENT_HEIGHT(s) SDL2_DISP_HEIGHT(SDL2_PARENT(s))

#define SDL2_CHECK(s)        (SDL2_MAGIC(s) == SDL2_MAGIC_NUMBER)
#define SDL2_ISROOT(s)       (SDL2_PARENT(s) == NULL)

/* Window manager frame titles */

#define SAC_SDL2_DEFAULT_HEADING "SaC SDL Display"
#define SAC_SDL2_SELECT_HEADING  "Click and drag to select an area, " \
                                 "button-two click to cancel..."

/* Colors */
extern Uint32 SAC_SDL2_background_rgb;
extern Uint32 SAC_SDL2_foreground_rgb;

#endif

#include "SDL2sac.h"

static int get_debug_level(void)
{
  const char* p = getenv("SDL2_DEBUG");
  unsigned    level = 0;

  if (p != NULL && sscanf(p, " %u", &level) <= 0) {
    level = 1;
  }
  return (int) level;
}

SDL2* SAC_SDL2_alloc_disp( int width, int height)
{
  SDL2* disp;
  
  disp = (SDL2 *) SAC_MALLOC( sizeof( *disp));
  SDL2_DEBUG( disp)        = get_debug_level();
  SDL2_SURF( disp)         = NULL;
  SDL2_LOCK( disp)         = SAC_SDL2_sem_alloc();
  SDL2_SEM( disp)          = SAC_SDL2_sem_alloc();
  SDL2_PARENT( disp)       = NULL;
  SDL2_ROOT( disp)         = disp;
  SDL2_DISP_X( disp)       = 0;
  SDL2_DISP_Y( disp)       = 0;
  SDL2_DISP_WIDTH( disp)   = width;
  SDL2_DISP_HEIGHT( disp)  = height;

  SDL2_MAGIC( disp)        = SDL2_MAGIC_NUMBER;

  return disp;
}

SDL2* SAC_SDL2_copy_disp( SDL2* parent, int x, int y, int w, int h)
{
  SDL2* disp;
  
  disp = (SDL2 *) SAC_MALLOC( sizeof( *disp));
  SDL2_DEBUG( disp)        = SDL2_DEBUG( parent);
  SDL2_SURF( disp)         = SDL2_SURF( parent);
  SDL2_LOCK( disp)         = SDL2_LOCK( parent);
  SDL2_SEM( disp)          = SDL2_SEM( parent);
  SDL2_PARENT( disp)       = parent;
  SDL2_ROOT( disp)         = SDL2_ROOT( parent);
  SDL2_DISP_X( disp)       = SDL2_DISP_X( parent) + x;
  SDL2_DISP_Y( disp)       = SDL2_DISP_Y( parent) + y;
  SDL2_DISP_WIDTH( disp)   = w;
  SDL2_DISP_HEIGHT( disp)  = h;

  SDL2_MAGIC( disp)        = SDL2_MAGIC( parent);

  return disp;
}

void SAC_SDL2_free_disp( SDL2* disp)
{
  SDL2_MAGIC( disp) = 0;
  SDL2_SURF( disp) = NULL;

  SAC_SDL2_sem_free( SDL2_LOCK( disp));
  SDL2_LOCK( disp) = NULL;

  SAC_SDL2_sem_free( SDL2_SEM( disp));
  SDL2_SEM( disp) = NULL;

  free( disp);
}

SDL2* SAC_SDL2_null( void)
{
  return NULL;
}

int SAC_SDL2_isnull( SDL2* disp)
{
  return disp == NULL;
}

const char* When( SDL2* disp)
{
  static char buf[30];
  Uint32 ticks = SDL_GetTicks();

  if (ticks < 1000*1000) {
    snprintf(buf, sizeof buf, "%3u.%03u: ", ticks / 1000, ticks % 1000);
  } else {
    strcpy(buf, "         ");
  }

  return buf;
}


#include "SDL2sac.h"

static const char* get_argv0(void)
{
  int    ac;
  char** av;
  char*  s;
  char*  h;
  size_t len;

  SAC_COMMANDLINE_GET( ac, av);
  s = av[0];

  /* try to skip 's' past the home directory component */
  h = getenv("HOME");
  if (h) {
    len = strlen( h);
    if ( ! strncmp( h, s, len) && s[len] == '/') {
      s += len + 1;
    }
  }

  return s;
}

void SAC_SDL2_heading( const char* heading)
{
  SDL_WM_SetCaption( heading, NULL);
}

void SAC_SDL2_default_heading( void)
{
  SDL_WM_SetCaption( get_argv0(), NULL);
}

void SAC_SDL2_select_heading( void)
{
  SDL_WM_SetCaption( SAC_SDL2_SELECT_HEADING, NULL);
}

void SAC_SDL2_cursor( int enable)
{
  SDL_ShowCursor( enable);
}


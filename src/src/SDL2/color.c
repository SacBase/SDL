#include "SDL2sac.h"

void SAC_SDL2_background( const int color[3])
{
  const unsigned char r = color[0];
  const unsigned char g = color[1];
  const unsigned char b = color[2];

  SAC_SDL2_background_rgb = RGB( r, g, b);
}

void SAC_SDL2_foreground( const int color[3])
{
  const unsigned char r = color[0];
  const unsigned char g = color[1];
  const unsigned char b = color[2];

  SAC_SDL2_foreground_rgb = RGB( r, g, b);
}


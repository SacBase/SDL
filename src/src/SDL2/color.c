#include "SDL2sac.h"

void SAC_SDL2_set_background( const int color[3])
{
  const unsigned char r = color[0];
  const unsigned char g = color[1];
  const unsigned char b = color[2];

  SAC_SDL2_background_rgb = RGB( r, g, b);
}

void SAC_SDL2_get_background( SAC_ND_PARAM_out_nodesc( aks_out_nt, int))
{
  SAC_ND_DECL__DATA( aks_nt, int, );
  const Uint32 rgb = SAC_SDL2_background_rgb;

  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 3);
  SAC_ND_A_FIELD( aks_nt)[2] = rgb & 0xFF;
  SAC_ND_A_FIELD( aks_nt)[1] = rgb >>  8 & 0xFF;
  SAC_ND_A_FIELD( aks_nt)[0] = rgb >> 16 & 0xFF;

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}

void SAC_SDL2_set_foreground( const int color[3])
{
  const unsigned char r = color[0];
  const unsigned char g = color[1];
  const unsigned char b = color[2];

  SAC_SDL2_foreground_rgb = RGB( r, g, b);
}

void SAC_SDL2_get_foreground( SAC_ND_PARAM_out_nodesc( aks_out_nt, int))
{
  SAC_ND_DECL__DATA( aks_nt, int, );
  const Uint32 rgb = SAC_SDL2_foreground_rgb;

  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 3);
  SAC_ND_A_FIELD( aks_nt)[2] = rgb & 0xFF;
  SAC_ND_A_FIELD( aks_nt)[1] = rgb >>  8 & 0xFF;
  SAC_ND_A_FIELD( aks_nt)[0] = rgb >> 16 & 0xFF;

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}

void SAC_SDL2_get_color( SAC_ND_PARAM_out_nodesc( aks_out_nt, int), char* name)
{
  SAC_ND_DECL__DATA( aks_nt, int, );
  unsigned char r = 255;
  unsigned char g = 255;
  unsigned char b = 255;
  size_t namelen = strlen( name);

  if (*name == '#') {
    if (namelen == 4) {
      unsigned rgb;
      if (sscanf( name + 1, "%x", &rgb) == 1) {
        r = rgb >> 8 & 0xF;
        g = rgb >> 4 & 0xF;
        b = rgb >> 0 & 0xF;
        r = r * 16 + r;
        g = g * 16 + g;
        b = b * 16 + b;
      }
    }
    else if (namelen == 7) {
      unsigned rgb;
      if (sscanf( name + 1, "%x", &rgb) == 1) {
        r = rgb >> 16 & 0xFF;
        g = rgb >>  8 & 0xFF;
        b = rgb >>  0 & 0xFF;
      }
    }
  }
  else {
    static const char* files[] = {
      "/usr/share/X11/rgb.txt",
      "/usr/lib/X11/rgb.txt",
      "/etc/X11/rgb.txt",
      "/usr/X11R6/lib/X11/rgb.txt",
    };
    FILE *fp = NULL;
    int i;
    for (i = 0; i < sizeof files / sizeof(files[0]); ++i) {
      if ((fp = fopen( files[i], "r")) != NULL) {
        break;
      }
    }
    if (fp != NULL) {
      char buf[128];
      char str[128];
      while (fgets( buf, sizeof buf, fp)) {
        unsigned u, v, w;
        if (sscanf( buf, " %u %u %u %[0-9A-Za-z ]", &u, &v, &w, str) == 4) {
          if ( ! strcmp( name, str)) {
            r = u;
            g = v;
            b = w;
            break;
          }
        }
      }
      fclose( fp);
    }
  }

  // printf("r = %d, g = %d, b = %d\n", r, g, b);

  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 3);
  SAC_ND_A_FIELD( aks_nt)[0] = r;
  SAC_ND_A_FIELD( aks_nt)[1] = g;
  SAC_ND_A_FIELD( aks_nt)[2] = b;

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}


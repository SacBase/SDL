#include "SDL2sac.h"
#include "fontstruct.h"
#include "fontdata.h"

void SAC_SDL2_text( SDL2* disp, char* text, int offsets[2], int async)
{
  const int xoffset = offsets[1];
  const int yoffset = offsets[0];

  SAC_SDL2_check( disp);
  if (SDL2_DEBUG( disp)) {
    printf("%sSAC_SDL2_text: '%s' on [%d,%d]:[%d,%d]\n",
            When( disp), text, xoffset, yoffset, 
            SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }

  SAC_SDL2_lock( disp);

  if ( xoffset < 0 || yoffset < 0 ||
       xoffset >= SDL2_DISP_WIDTH( disp) ||
       yoffset >= SDL2_DISP_HEIGHT( disp))
  {
    SAC_RuntimeError( "SAC_SDL2_text: Offset [%d,%d] exceeds display [%d,%d]\n",
                      xoffset, yoffset, 
                      SDL2_DISP_WIDTH( disp), SDL2_DISP_HEIGHT( disp));
  }
  else {
    const int xdisp = SDL2_DISP_X( disp);
    const int ydisp = SDL2_DISP_Y( disp);
    const int xend = SDL2_DISP_X( disp) + SDL2_DISP_WIDTH( disp);
    const int yend = SDL2_DISP_Y( disp) + SDL2_DISP_HEIGHT( disp);
    const int rgb = SAC_SDL2_foreground_rgb;
    int x = xdisp + xoffset;
    int y = ydisp + yoffset;
    int xmin = x;
    int xmax = x;
    int ymin = y - font.ascent;
    int ymax = y + font.descent;
    int i;
    for (i = 0; text[i]; ++i) {
      unsigned char c = text[i];
      if (c == '\n') {
        x = xdisp + xoffset;
        y += font.ascent + font.descent;
        ymax = y + font.descent;
      }
      else if (c == '\r') {
        x = xdisp + xoffset;
      }
      else if (c == '\t') {
        if (' ' >= font.first && c <= font.last) {
          const int tab = 8 * font.chars[' ' - font.first].width;
          x += tab;
          x -= (x - xdisp - xoffset) % tab;
        }
      }
      else if (c >= font.first && c <= font.last) {
        const struct fontchar *f = &font.chars[c - font.first];
        int bit = f->firstbit;
        int bear, row;
        for (row = f->ascent; row > -f->descent; --row) {
          for (bear = f->lbearing; bear < f->rbearing; ++bear) {
            if (getbit( fontbits, bit)) {
              if (x >= xdisp && x < xend && y >= ydisp && y < yend) {
                SAC_SDL2_draw_a_pixel( disp, x + bear, y - row, rgb);
              }
            }
            ++bit;
          }
        }
        x += f->width;
        if (xmax < x) {
          xmax = x;
        }
      }
    }
    LIMIT(xmax, xdisp, xend - 1);
    LIMIT(ymax, ydisp, yend - 1);
    SAC_SDL2_update_rect( disp, xmin, ymin, xmax - xmin, ymax - ymin, async);
  }

  SAC_SDL2_unlock( disp);
}

void SAC_SDL2_text_extent( SAC_ND_PARAM_out_nodesc( aks_out_nt, int),  char* text)
{
  SAC_ND_DECL__DATA( aks_nt, int, );

  int x = 0;
  int xmax = x;
  int ymin = -font.ascent;
  int ymax = font.descent;
  int i;
  for (i = 0; text[i]; ++i) {
    const unsigned char c = text[i];
    if (c == '\n') {
      x = 0;
      ymax += font.ascent + font.descent;
    }
    else if (c == '\r') {
      x = 0;
    }
    else if (c == '\t') {
      if (' ' >= font.first && c <= font.last) {
        const int tab = 8 * font.chars[' ' - font.first].width;
        x += tab - x % tab;
      }
    }
    else if (c >= font.first && c <= font.last) {
      x += font.chars[c - font.first].width;
      if (xmax < x) {
        xmax = x;
      }
    }
  }

  SAC_ND_A_FIELD( aks_nt) = SAC_MALLOC( sizeof( int) * 4);
  SAC_ND_A_FIELD( aks_nt)[1] = 0;
  SAC_ND_A_FIELD( aks_nt)[3] = xmax;
  SAC_ND_A_FIELD( aks_nt)[0] = ymin;
  SAC_ND_A_FIELD( aks_nt)[2] = ymax;

  SAC_ND_RET_out__NODESC( aks_out_nt, aks_nt);
}


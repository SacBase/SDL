#include "SDL2sac.h"

#define LIMIT(a,b,c)    ((a) = ((a) < (b)) ? (b) : ((a) > (c)) ? (c) : (a))

enum SelectMode {
  SelNone = 0,
  SelStart,
  SelDown,
  SelDone,
};
static enum SelectMode sel_mode;
static SDL2* sel_disp;
static int selection[4];

static void change_mode( enum SelectMode new_mode)
{
  if (SDL2_DEBUG( sel_disp)) {
    printf("%sChanging select mode from %d to %d.\n",
            When( sel_disp), sel_mode, new_mode);
  }
  sel_mode = new_mode;
}

void SAC_SDL2_start_selection( SDL2* disp)
{
  sel_disp = disp;
  change_mode( SelStart);
}

int SAC_SDL2_get_selection( int *sel)
{
  if ( sel_mode == SelDone) {
    sel[0] = (selection[0] <= selection[2]) ? selection[0] : selection[2];
    sel[2] = (selection[0] <= selection[2]) ? selection[2] : selection[0];
    sel[1] = (selection[1] <= selection[3]) ? selection[1] : selection[3];
    sel[3] = (selection[1] <= selection[3]) ? selection[3] : selection[1];
  } else {
    sel[0] = sel[1] = sel[2] = sel[3] = -1;
  }

  return (sel_mode == SelDone);
}

static void invert(void)
{
  SAC_SDL2_invert_rect( sel_disp, selection, TRUE);
}

static int next_event_type(void)
{
  int       r;
  int       kind;
  SDL_Event event;

  kind = -1;
  r = SDL_PeepEvents( &event, 1, SDL_PEEKEVENT, SDL_MOUSEEVENTMASK);
  if (r == 1) {
    kind = event.type;
  }

  return kind;
}

void SAC_SDL2_mouse_down_event( SDL_Event* event)
{
  if (sel_mode == SelStart || sel_mode == SelDown) {
    if (SDL2_DEBUG( sel_disp)) {
      printf("%sSAC_SDL2_mouse_down: button %d, mode %d\n", When( sel_disp), 
             event->button.button, sel_mode);
    }
  }
  if ((event->button.button == 1) && (sel_mode == SelStart))
  {
    change_mode( SelDown);
    LIMIT(event->button.x, SDL2_DISP_X( sel_disp),
          SDL2_DISP_X( sel_disp) + SDL2_DISP_WIDTH( sel_disp) - 1);
    LIMIT(event->button.y, SDL2_DISP_Y( sel_disp),
          SDL2_DISP_Y( sel_disp) + SDL2_DISP_HEIGHT( sel_disp) - 1);
    selection[0] = event->button.x;
    selection[1] = event->button.y;
    selection[2] = event->button.x;
    selection[3] = event->button.y;
    invert();
  }
}

void SAC_SDL2_mouse_up_event( SDL_Event* event)
{
  if (sel_mode == SelDown || sel_mode == SelStart) {
    if (SDL2_DEBUG( sel_disp)) {
      printf("%sSAC_SDL2_mouse_up: button %d, mode %d\n", When( sel_disp), 
             event->button.button, sel_mode);
    }
  }
  if ((event->button.button == 1) && (sel_mode == SelDown))
  {
    invert();
    change_mode( SelDone);
    LIMIT(event->button.x, SDL2_DISP_X( sel_disp),
          SDL2_DISP_X( sel_disp) + SDL2_DISP_WIDTH( sel_disp) - 1);
    LIMIT(event->button.y, SDL2_DISP_Y( sel_disp),
          SDL2_DISP_Y( sel_disp) + SDL2_DISP_HEIGHT( sel_disp) - 1);
    selection[2] = event->button.x;
    selection[3] = event->button.y;
    SAC_SDL2_post( sel_disp);
  }
  if ((event->button.button == 2) &&
      (sel_mode == SelDown || sel_mode == SelStart))
  {
    if (sel_mode == SelDown) {
      invert();
    }
    change_mode( SelNone);
    SAC_SDL2_post( sel_disp);
  }
}

void SAC_SDL2_mouse_motion_event( SDL_Event* event)
{
  if (sel_mode == SelDown && next_event_type() != SDL_MOUSEMOTION)
  {
    if (SDL2_DEBUG( sel_disp)) {
      printf("%sSAC_SDL2_mouse_motion: %d,%d, mode %d\n", When( sel_disp), 
             event->motion.x, event->motion.y, sel_mode);
    }

    LIMIT(event->button.x, SDL2_DISP_X( sel_disp),
          SDL2_DISP_X( sel_disp) + SDL2_DISP_WIDTH( sel_disp) - 1);
    LIMIT(event->button.y, SDL2_DISP_Y( sel_disp),
          SDL2_DISP_Y( sel_disp) + SDL2_DISP_HEIGHT( sel_disp) - 1);
    if (event->motion.x != selection[2] || event->motion.y != selection[3])
    {
      invert();
      selection[2] = event->motion.x;
      selection[3] = event->motion.y;
      invert();
    }
  }
}


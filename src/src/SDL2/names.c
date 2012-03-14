#include "SDL2sac.h"

static const char* SAC_SDL2_event_names[] = {
  /*  0 */  "noevent",
  /*  1 */  "activeevent",
  /*  2 */  "keydown",
  /*  3 */  "keyup",
  /*  4 */  "mousemotion",
  /*  5 */  "mousebuttondown",
  /*  6 */  "mousebuttonup",
  /*  7 */  "joyaxismotion",
  /*  8 */  "joyballmotion",
  /*  9 */  "joyhatmotion",
  /* 10 */  "joybuttondown",
  /* 11 */  "joybuttonup",
  /* 12 */  "quit",
  /* 13 */  "syswmevent",
  /* 14 */  "event_reserveda",
  /* 15 */  "event_reservedb",
  /* 16 */  "videoresize",
  /* 17 */  "videoexpose",
  /* 18 */  "event_reserved2",
  /* 19 */  "event_reserved3",
  /* 20 */  "event_reserved4",
  /* 21 */  "event_reserved5",
  /* 22 */  "event_reserved6",
  /* 23 */  "event_reserved7",
  /* 24 */  "userevent",
  /* 25 */  "create_event",
  /* 26 */  "teardown_event",
  /* 27 */  "update_event",
  /* 28 */  "update_async_event",
};

const char* SAC_SDL2_event_name(int evno)
{
  size_t count = sizeof(SAC_SDL2_event_names) / sizeof(const char *);

  if (evno < 0 || evno >= count) {
    SAC_RuntimeError( "SDL2_event_name: unknown event number %d", evno);
  }
  return SAC_SDL2_event_names[evno];
}


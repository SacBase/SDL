#include "SDL2sac.h"
#include <pthread.h>

static pthread_t SAC_SDL2_thread_id;


  /* Arguments for event loop thread. */
  typedef struct thread_args {
    int*  init_result_ptr;
    SDL2* disp;
  } thread_args_t;


static void* SAC_SDL2_thread( void *vp)
{
  thread_args_t* ta = (thread_args_t *) vp;
  SDL2*          disp = ta->disp;
  int*           result = ta->init_result_ptr;
  int            r;

  if (SDL2_DEBUG( disp)) {
    printf( "%sSDL2_thread starts\n", When( disp));
  }

  r = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
  *result = r;
  SAC_SDL2_post( disp);
  if (r == 0) {
    SAC_SDL2_event_loop( disp);
  }

  if (SDL2_DEBUG( disp)) {
    printf( "%sSDL2_thread ends\n", When( disp));
  }

  return NULL;
}

int SAC_SDL2_setup( SDL2* disp)
{
  pthread_attr_t attr;
  int            r;
  int            sdl_init_result;
  thread_args_t  ta;

  ta.disp = disp;
  ta.init_result_ptr = &sdl_init_result;

  r = pthread_attr_init( &attr);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_setup: pthread_attr_create failed: %d: %m\n", r);
  }
  r = pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_setup: pthread_attr_setscope failed: %d: %m\n", r);
  }
  r = pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_setup: pthread_attr_setdetach failed: %d: %m\n", r);
  }
  r = pthread_create( &SAC_SDL2_thread_id, NULL, SAC_SDL2_thread, &ta);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_setup: pthread_create failed: %d: %m\n", r);
  }

  pthread_attr_destroy( &attr);

  SAC_SDL2_wait( disp);

  return sdl_init_result;
}

void SAC_SDL2_teardown_event( SDL_Event* event)
{
  SDL2* disp = (SDL2 *) event->user.data2;
  SAC_SDL2_post( disp);
}

void SAC_SDL2_teardown( SDL2* disp)
{
  int r;

  SDL_Event event;
  event.type = SDL2_TEARDOWN_EVENT;
  event.user.data2 = disp;
  SDL_PushEvent( &event);
  SAC_SDL2_wait( disp);

  r = pthread_join( SAC_SDL2_thread_id, NULL);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_teardown: pthread_join failed: %d: %m\n", r);
  }
}


#include "SDL2sac.h"

void SAC_SDL2_sem_init( sem_t *sem)
{
  int r = sem_init( sem, SEM_SHARE_THREAD, SEM_INIT_LOCKED);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_sem_alloc: sem_init failed: %d: %m\n", r);
  }
}

sem_t* SAC_SDL2_sem_alloc(void)
{
  sem_t* sem = (sem_t *) SAC_MALLOC( sizeof( sem_t));
  SAC_SDL2_sem_init( sem);
  return sem;
}

void SAC_SDL2_sem_free( sem_t* sem)
{
  int r = sem_destroy( sem);
  if (r) {
    SAC_RuntimeError( "SAC_SDL2_sem_free: sem_destroy failed: %d: %m\n", r);
  }
}

void SAC_SDL2_sem_post( sem_t* sem)
{
  int   r;

  r = sem_post( sem);
  if (r) {
    SAC_RuntimeError( "SDL2_sem_post: sem_post failed: %d: %m\n", r);
  }
}

void SAC_SDL2_sem_wait( sem_t* sem)
{
  int r;

  r = sem_wait( sem);
  if (r) {
    SAC_RuntimeError( "SDL2_sem_wait: sem_wait failed: %d: %m\n", r);
  }
}

void SAC_SDL2_post( SDL2* disp)
{
  sem_t *sem = SDL2_SEM( disp);

  if (SDL2_DEBUG( disp) >= 2) {
    printf("%spost\n", When( disp));
  }
  SAC_SDL2_sem_post( sem);
}

void SAC_SDL2_wait( SDL2* disp)
{
  sem_t *sem = SDL2_SEM( disp);

  if (SDL2_DEBUG( disp) >= 2) {
    printf("%swait...\n", When( disp));
  }
  SAC_SDL2_sem_wait( sem);
  if (SDL2_DEBUG( disp) >= 2) {
    printf("%swaited.\n", When( disp));
  }
}

void SAC_SDL2_lock( SDL2* disp)
{
  sem_t *sem = SDL2_LOCK( disp);

  if (SDL2_DEBUG( disp) >= 2) {
    printf("%slock...\n", When( disp));
  }
  SAC_SDL2_sem_wait( sem);
  if (SDL2_DEBUG( disp) >= 2) {
    printf("%slocked.\n", When( disp));
  }
}

void SAC_SDL2_unlock( SDL2* disp)
{
  sem_t *sem = SDL2_LOCK( disp);

  if (SDL2_DEBUG( disp) >= 2) {
    printf("%sunlock\n", When( disp));
  }
  SAC_SDL2_sem_post( sem);
}


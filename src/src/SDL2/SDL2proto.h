#ifndef SDL2PROTO_H
#define SDL2PROTO_H

/* close.c */

void SAC_SDL2_close( SDL2* disp);

/* data.c */

SDL2* SAC_SDL2_alloc_disp(void);
void SAC_SDL2_free_disp( SDL2* disp);
SDL2* SAC_SDL2_null( void);
int SAC_SDL2_isnull( SDL2* disp);
const char* When( SDL2* disp);

/* display.c */

void SAC_SDL2_enable_fullscreen( int enable);
void SAC_SDL2_enable_noframe( int enable);
void SAC_SDL2_create_display_event( SDL_Event* event);
void SAC_SDL2_display( SDL2** disp_ptr,
                       SAC_ND_PARAM_in( shp_nt, int));
void SAC_SDL2_resize( SDL2* disp,
                      SAC_ND_PARAM_in( shp_nt, int));

/* draw.c */

void SAC_SDL2_draw( SDL2* disp,
                    SAC_ND_PARAM_in( ar_nt, int));
void SAC_SDL2_draw2( SDL2* disp, SAC_ND_PARAM_in( ar_nt, int), int offsets[2]);

/* event.c */

void SAC_SDL2_event_loop( SDL2* disp);

/* extent.c */

void SAC_SDL2_extent( SAC_ND_PARAM_out_nodesc( aks_out_nt, int), SDL2* disp);

/* init.c */

void SAC_SDL2_check( SDL2* disp);
int SAC_SDL2_init( SDL2* disp);
void SAC_SDL2_stop( SDL2* disp);

/* invert.c */

void SAC_SDL2_invert_rect( SDL2* disp, int *pos, int update);
void SAC_SDL2_invert2( SDL2* disp, int offsets[2], int sizes[2]);

/* mouse.c */

void SAC_SDL2_start_selection( SDL2* disp);
int SAC_SDL2_get_selection( int *sel);
void SAC_SDL2_mouse_down_event( SDL_Event* event);
void SAC_SDL2_mouse_up_event( SDL_Event* event);
void SAC_SDL2_mouse_motion_event( SDL_Event* event);

/* names.c */

const char* SAC_SDL2_event_name(int evno);

/* pixel.c */

void SAC_SDL2_pixel( SDL2* disp, SAC_ND_PARAM_in( shp_nt, int), SAC_ND_PARAM_in( color_nt, int));

/* select.c */

void SAC_SDL2_select( SAC_ND_PARAM_out_nodesc( aks_out_nt, int), SDL2* disp);

/* sem.c */

void SAC_SDL2_sem_init( sem_t *sem);
sem_t* SAC_SDL2_sem_alloc(void);
void SAC_SDL2_sem_free( sem_t* sem);
void SAC_SDL2_sem_post( sem_t* sem);
void SAC_SDL2_sem_wait( sem_t* sem);
void SAC_SDL2_post( SDL2* disp);
void SAC_SDL2_wait( SDL2* disp);
void SAC_SDL2_lock( SDL2* disp);
void SAC_SDL2_unlock( SDL2* disp);

/* setup.c */

int SAC_SDL2_setup( SDL2* disp);
void SAC_SDL2_teardown_event( SDL_Event* event);
void SAC_SDL2_teardown( SDL2* disp);

/* title.c */

void SAC_SDL2_heading( const char* heading);
void SAC_SDL2_default_heading();
void SAC_SDL2_select_heading();
void SAC_SDL2_cursor( int enable);

/* update.c */

void SAC_SDL2_update_display_event( SDL_Event* event);
void SAC_SDL2_update_rect( SDL2* disp, int x, int y, int w, int h, int async);
void SAC_SDL2_update( SDL2* disp, int async);
void SAC_SDL2_update2( SDL2* disp, int offsets[2], int sizes[2], int async);


#endif

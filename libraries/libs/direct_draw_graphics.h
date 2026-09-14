#ifndef direct_draw_graphics
#define direct_draw_graphics

#include "libraries/include/direct_draw_graphics_headers.h"

extern DirectDrawCanvas_t initiate_direct_draw_canvas(int width, int height);
extern int draw_membuffer(DirectDrawCanvas_t canvas);
extern void wait_frame_gap(int fps);

#endif
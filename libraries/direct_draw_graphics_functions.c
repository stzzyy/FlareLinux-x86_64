#include "libraries/include/direct_draw_graphics_headers.h"
#include "libraries/libs/direct_draw_graphics_functions.h"

extern int draw_quadralateral(DirectDrawCanvas_t canvas, int x, int y, int width, int height, ColorComponent *color) {}

extern int draw_circle(DirectDrawCanvas_t canvas, int x, int y, int radius, int hollow, int ifhollow_width, ColorComponent *color) {}

extern int draw_text(DirectDrawCanvas_t canvas, int x, int y, char *text, int text_len, FontType *font, ColorComponent *color) {}

extern int draw_line(DirectDrawCanvas_t canvas, int x, int y, int length, int width, ColorComponent *color) {}

extern int draw_bitmap(DirectDrawCanvas_t canvas, int x, int y, ImageBitmap *image) {}


extern FontType * define_font(char *font_path, int size, int scalar) {}

extern ImageBitmap * define_image(char *image_path, int scalar) {}

extern ColorComponent * color(int aplha, int red, int green, int blue) {}
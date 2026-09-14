#ifndef direct_draw_graphics_functions
#define direct_draw_graphics_functions

struct __FontType__ {};
struct __ImageBitmap__ {};
struct __ColorComponent__ {};

typedef struct __FontType__ FontType;
typedef struct __ImageBitmap__ ImageBitmap;
typedef struct __ColorComponent__ ColorComponent;


extern int draw_quadralateral(DirectDrawCanvas_t canvas, int x, int y, int width, int height, ColorComponent *color);
extern int draw_circle(DirectDrawCanvas_t canvas, int x, int y, int radius, int hollow, int ifhollow_width, ColorComponent *color);
extern int draw_text(DirectDrawCanvas_t canvas, int x, int y, char *text, int text_len, FontType *font, ColorComponent *color);
extern int draw_line(DirectDrawCanvas_t canvas, int x, int y, int length, int width, ColorComponent *color);
extern int draw_bitmap(DirectDrawCanvas_t canvas, int x, int y, ImageBitmap *image, ColorComponent *color);

extern FontType * define_font(char *font_path, int size, int scalar);
extern ImageBitmap * define_image(char *image_path, int scalar);
extern ColorComponent * color(int aplha, int red, int green, int blue);

#endif
// direct_draw_grasphics.h

#include "libraries/include/direct_draw_graphics_headers.h"
#include "libraries/libs/direct_draw_graphics.h"


DirectDrawCanvas_t initiate_direct_draw_canvas(int width, int height) {
    DirectDrawCanvas_t canvas_struct;

    struct wl_display *display = wl_display_connect(NULL);
    if (!display) return;

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display); // Sync to fetch available interfaces

    if (!compositor || !shm || !layer_shell) {
        fprintf(stderr, "Required Wayland / wlroots protocols missing.\n");
        return;
    }

    int stride = width * 4;
    int size = stride * height;

    struct wl_surface *surface = wl_compositor_create_surface(compositor);
    struct zwlr_layer_surface_v1 *layer_surface = zwlr_layer_shell_v1_get_layer_surface(
        layer_shell, surface, NULL, ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY, "screen-overlay"
    );

    zwlr_layer_surface_v1_set_size(layer_surface, width, height);
    zwlr_layer_surface_v1_set_anchor(layer_surface, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);
    zwlr_layer_surface_v1_set_keyboard_interactivity(layer_surface, ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE);
    zwlr_layer_surface_v1_add_listener(layer_surface, &layer_surface_listener, NULL);

    struct wl_region *empty_region = wl_compositor_create_region(compositor);
    wl_surface_set_input_region(surface, empty_region);
    wl_region_destroy(empty_region);

    int fd = create_shm_file(size);
    uint32_t *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
    close(fd);
    wl_shm_pool_destroy(pool);

    memset(data, 0, size); // clears memory

    canvas_struct.display = display;
    canvas_struct.registry = registry;
    canvas_struct.width = width;
    canvas_struct.size = size;
    canvas_struct.stride = stride;
    canvas_struct.surface = surface;
    canvas_struct.layer_surface = layer_surface;
    canvas_struct.empty_region = empty_region;
    canvas_struct.data = data;
    canvas_struct.buffer = buffer;

    return canvas_struct;
}

int draw_membuffer(DirectDrawCanvas_t canvas) {
    wl_surface_attach(canvas.surface, canvas.buffer, 0, 0);
    wl_surface_damage(canvas.surface, 0, 0, canvas.width, canvas.height);
    wl_surface_commit(canvas.surface);
    return 0;
}




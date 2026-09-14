#ifndef direct_draw_graphics_dev_headers
#define direct_draw_graphics_dev_headers

// idk why i have to define these but the code doesnt work without
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <wayland-client.h>
#include "include/wlr-layer-shell-unstable-v1-client.h"

struct wl_compositor *compositor = NULL;
struct wl_shm *shm = NULL;
struct zwlr_layer_shell_v1 *layer_shell = NULL;

static int create_shm_file(off_t size) {
    int fd = memfd_create("wayland-shm", MFD_CLOEXEC);
    if (fd < 0) return -1;
    if (ftruncate(fd, size) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static void registry_handle_global(void *data, struct wl_registry *registry, 
                                   uint32_t id, const char *interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 3);
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, "zwlr_layer_shell_v1") == 0) {
        layer_shell = wl_registry_bind(registry, id, &zwlr_layer_shell_v1_interface, 1);
    }
}
static const struct wl_registry_listener registry_listener = { .global = registry_handle_global };

static void layer_surface_configure(void *data, struct zwlr_layer_surface_v1 *surface, 
                                    uint32_t serial, uint32_t width, uint32_t height) {
    zwlr_layer_surface_v1_ack_configure(surface, serial);
}
static const struct zwlr_layer_surface_v1_listener layer_surface_listener = { .configure = layer_surface_configure };

struct DirectDrawCanvas {
    struct wl_display *display;
    struct wl_registry *registry;

    int width;
    int height;
    int stride;
    int size;

    struct wl_surface *surface;
    struct zwlr_layer_surface_v1 *layer_surface;
    struct wl_region *empty_region;
    uint32_t *data;
    struct wl_buffer *buffer;
};
typedef struct DirectDrawCanvas DirectDrawCanvas_t;

#endif
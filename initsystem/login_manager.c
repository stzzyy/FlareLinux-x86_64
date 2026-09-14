#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <shadow.h>
#include <crypt.h>
#include <time.h>
#include <memory.h>
#include "libraries/libs/direct_draw_graphics.h"
#include "libraries/libs/direct_draw_graphics_functions.h"

#define GETSCREEN_W 1980
#define GETSCREEN_L 1080
#define GET_PICTURE_QUALITY "uhd"
#define GETSCALAR(quality) 1

int loginVerification(char pwd_entry[], char username_entry[]) {
    struct spwd *shadow_entry = getspnam(username_entry);
    if (shadow_entry == NULL) {
        return 0; 
    }
    struct crypt_data data;
    data.initialized = 0;
    char *hashed_input = crypt_r(pwd_entry, shadow_entry->sp_pwdp, &data);
    if (hashed_input == NULL) {
        return -1; // Hashing failed internally
    }
    int match = 1;
    for (size_t i = 0; i < strlen(shadow_entry->sp_pwdp); i++) {
        if (hashed_input[i] != shadow_entry->sp_pwdp[i]) match = 0;
    }
    return match;
}


int main(int argc, char *argv[], char **envp) {
    printf("Starting login manager\n");
    printf("As user %i\n\n", getuid());

    DirectDrawCanvas_t canvas = initiate_direct_draw_canvas(GETSCREEN_W, GETSCREEN_L);
    printf("Initalized Display...");

    int runtime = 0;

    char *background_image_path = "/flarelinux/assets/bitmap/login_background.bitmap";
    strcat(background_image_path, *GET_PICTURE_QUALITY);
    ImageBitmap * background = define_image(background_image_path, GETSCALAR(GET_PICTURE_QUALITY));

    while (runtime == 0) {
        memcpy(canvas.data, 0, canvas.size);

        draw_bitmap(canvas, 0, 0, background); // background image
        
        wait_frame_gap(30);
    }
}
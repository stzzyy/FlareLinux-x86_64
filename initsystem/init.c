// Initial binary to be executed at the start of the boot

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include "include/panic.h"


int execute_login_manager() {
    uid_t target_uid = 1001; 
    gid_t target_gid = 1001;
    pid_t pid = fork();
    if (pid < 0) {
        perror("PANIC - ROOT: Unable to fork of into login manager");
        panic_restricted_shell_mode();
    }
    if (pid == 0) {
        if (setgid(target_gid) != 0) {
            perror("PANIC - LOGINMANAGER_PARENT: Unable to set group id of process");
            panic_restricted_shell_mode();
        }
        if (setuid(target_uid) != 0) {
            perror("PANIC - LOGINMANAGER_PARENT: Unable to set user id of process");
            panic_restricted_shell_mode();
        }
        execve("/flarelinux/userland/loginmanager.bin", NULL, NULL);
        perror("PANIC - LOGINMANAGER_PARENT: Unable to branch to child process");
        panic_restricted_shell_mode();
    }
    return 0;
}



int main(int argc, char *argv[], char **envp) {
    printf("Welcome to FlareLinux Standard Edition\n");

    int bootmode = open("/flarelinux/boot/bootmode.txt", O_RDONLY);
    char *bootmode_val;
    read(bootmode, bootmode_val, 1);

    if (strcmp(bootmode_val, *"b")) {
        printf("\nCreating protected fs");
        mkdir("/flarelinux/userland", 0x740);
        mount("/flarelinux/tempuserland.iso", "/flarelinux/userland", "FAT32", 0, NULL);
        execute_login_manager();
    };
}
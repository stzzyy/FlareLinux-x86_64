#include <unistd.h>
#include <fcntl.h>

// IMPORTANT NOTE: this function is designed to be used by pid 1, any program without pid 1 will be rejected from shell
void panic_root_shell_mode() {
    if (getpid() == 1) {
        write(0, "System PANIC, hibernating in root shell!", 40);
        execve("/bin/sh", NULL, NULL);
    } else {
        return;
    }
}

void panic_restricted_shell_mode() {
    write(0, "System PANIC, hibernating in root shell!", 40);
    setgid(1001);
    setuid(1001);
    execve("/bin/sh", NULL, NULL);
}
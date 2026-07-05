#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define LOG_FILE "system.log"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"message\"\n", argv[0]);
        return 1;
    }

    int fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct flock fl;
    fl.l_type   = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;

    fcntl(fd, F_SETLKW, &fl);

    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    char timebuf[32];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tmv);

    char line[512];
    int len = snprintf(line, sizeof(line), "[PID:%d] [%s] [INFO] %s\n",
                        getpid(), timebuf, argv[1]);

    write(fd, line, len);

    /* BUG: declares a fresh, uninitialized flock struct for the unlock
     * instead of reusing/resetting `fl`. Only l_type is set here, so
     * l_whence/l_start/l_len are garbage stack values. This can unlock
     * the wrong byte range (or fail) depending on what garbage ends up
     * in l_start/l_len, instead of reliably releasing the whole-file
     * lock that was acquired above. */
    struct flock fl_unlock;
    fl_unlock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &fl_unlock);

    close(fd);
    return 0;
}

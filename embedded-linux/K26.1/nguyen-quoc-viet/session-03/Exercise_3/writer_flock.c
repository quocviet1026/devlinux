#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
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

    flock(fd, LOCK_EX);

    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    char timebuf[32];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tmv);

    char line[512];
    int len = snprintf(line, sizeof(line), "[PID:%d] [%s] [INFO] %s\n",
                        getpid(), timebuf, argv[1]);

    write(fd, line, len);

    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}

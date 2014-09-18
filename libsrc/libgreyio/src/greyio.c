/*
 * Copyright (c) 2014 Damian Jason Lapidge
 *
 * The contents of this file are subject to the terms and conditions defined 
 * within the file LICENSE.txt, located within this project's root directory.
 */

#include <greyio.h>

/*
 * stdlib.h: realloc()
 * unistd.h: ssize_t, size_t, read(), write()
 * string.h: memcpy()
 */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

static const char *version = "v1.0.0";
static const char *authors = "Damian Jason Lapidge <grey@greydamian.org>";

ssize_t readall(int fd, void **buf) {
    ssize_t total  = 0;
    ssize_t rbytes = 0;

    char tmpbuf[BUFSIZE];

    rbytes = read(fd, tmpbuf, BUFSIZE);
    while (rbytes > 0) {
        *buf = realloc(*buf, total + rbytes);
        if (*buf == NULL)
            return -1; /* failure */
        memcpy(*buf + total, tmpbuf, rbytes);
        total += rbytes;
        rbytes = read(fd, tmpbuf, BUFSIZE);
    }

    if (rbytes == -1)
        return -1; /* failure */

    return total;
}

ssize_t writeall(int fd, void *buf, size_t count) {
    ssize_t total  = 0;
    ssize_t wbytes = 0;

    while (total < count) {
        wbytes = write(fd, buf + total, count - total);
        if (wbytes == -1)
            return -1; /* failure */
        total += wbytes;
    }

    return total;
}


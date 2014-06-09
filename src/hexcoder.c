/*
 * Copyright (c) 2014 Damian Jason Lapidge
 *
 * The contents of this file are subject to the terms and conditions defined 
 * within the file LICENSE.txt, located within this project's root directory.
 */

/*
 * stdio.h : stderr, fprintf()
 * stdlib.h: EXIT_SUCCESS, EXIT_FAILURE, exit(), malloc(), realloc(), free()
 * stddef.h: size_t, ssize_t
 * unistd.h: STDIN_FILENO, STDOUT_FILENO, optopt, read(), write(), close(), 
 *           getopt()
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

/*
 * string.h: memcpy(), strchr()
 * ctype.h : toupper()
 */
#include <string.h>
#include <ctype.h>

/*
 * wordspace.h: getword(), getindex()
 * greyio.h   : writeall()
 */
#include <wordspace.h>
#include <greyio.h>

#define BUFSIZE 32 /* must be mulitple of 32, so beautify works correctly */

const char *HEXCHARS_MIXED = "0123456789ABCDEFabcdef";
const char *HEXCHARS_UPPER = "0123456789ABCDEF";

enum cmdmode {
    ENCODE,
    DECODE,
    BEAUTIFY
};

struct cmdopts {
    enum cmdmode mode; /* encode, decode, or beautify */
};

void print_usage() {
    fprintf(stderr, "usage: hexcoder [-e] [-d] [-b]\n");
}

int parse_args(struct cmdopts *opts, int argc, char *argv[]) {
    const char *optstr = ":edb";
    char        optchr;

    opts->mode = ENCODE; /* default mode */

    optchr = getopt(argc, argv, optstr);
    while (optchr != -1) {
        switch (optchr) {
            case 'e':
                /* -e option */
                opts->mode = ENCODE;
                break;
            case 'd':
                /* -d option */
                opts->mode = DECODE;
                break;
            case 'b':
                /* -b option */
                opts->mode = BEAUTIFY;
                break;
            case ':':
                /* missing argument */
                return -1; /* failure */
            case '?':
                /* unknown option */
                return -1; /* failure */
            default:
                /* should never reach here */
                return -1;
        }
        optchr = getopt(argc, argv, optstr);
    }

    return 0; /* success */
}

/*
 * Read only hexadecimal characters.
 */
ssize_t readhex(int fd, void *buf, size_t count) {
    int rtotal = 0, rbytes = 0;

    char c = '\0';
    while (rtotal < count) {
        rbytes = read(fd, &c, 1);
        if (rbytes < 1)
            break;

        if (strchr(HEXCHARS_MIXED, c) == NULL || c == '\0')
            continue; /* ignore non-hex chars */

        memcpy(buf + rtotal, &c, 1);
        rtotal++;
    }
    if (rbytes < 0)
        return rbytes; /* failure */

    return rtotal; /* success */
}

/*
 * Convert a string (in place) to uppercase.
 */
void strupper(char *s, size_t n) {
    int i = 0;
    for (i = 0; i < n; i++)
        s[i] = toupper(s[i]);
}

/*
 * Compute maximum length of buffer once beautified.
 */
size_t beautified_len(size_t n) {
    size_t bytes     = (n - 1) / 2 + 1;
    size_t quadbytes = (bytes - 1) / 4 + 1;
    size_t lines     = (quadbytes - 1) / 4 + 1;

    return bytes * 3 + quadbytes - lines;
}

/* hex -> hex */
ssize_t beautify(char **dst, char *src, size_t n) {
    size_t dstlen = beautified_len(n);

    char *tmpdst = realloc(*dst, dstlen);
    if (tmpdst == NULL)
        return -1; /* failure */
    *dst = tmpdst;

    int i = 0, j = 0;
    for (i = 0, j = 0; i < n; i++, j++) {
        (*dst)[j] = src[i];

        if (i + 1 >= n) {
            (*dst)[++j] = '\n'; /* append final newline */
            continue;           /* skip any further formatting */
        }

        if ((i + 1) % 2 == 0)
            (*dst)[++j] = ' '; /* append space */

        if ((i + 1) % 32 == 0)
            (*dst)[j] = '\n';  /* replace space with newline */
        else if ((i + 1) % 8 == 0)
            (*dst)[++j] = ' '; /* append second space */
    }

    return j;
}

/* bin -> hex */
ssize_t encode(char **dst, void *src, size_t n) {
    char *tmpdst = realloc(*dst, n * 2);
    if (tmpdst == NULL)
        return -1; /* failure */
    *dst = tmpdst;

    char *char_src = (char *)src;

    int i = 0;
    for (i = 0; i < n; i++)
        getword(char_src[i] & 0xff, *dst + i * 2, HEXCHARS_UPPER, 2);

    return n * 2;
}

/* hex -> bin */
ssize_t decode(void **dst, char *src, size_t n) {
    void *tmpdst = realloc(*dst, n / 2);
    if (tmpdst == NULL)
        return -1; /* failure */
    *dst = tmpdst;

    strupper(src, n);

    char *char_dst = (char *)*dst;

    int i = 0;
    for (i = 0; i < n / 2; i++)
        char_dst[i] = (char)getindex(src + i * 2, HEXCHARS_UPPER, 2);

    return n / 2;
}

int main(int argc, char *argv[]) {
    struct cmdopts opts;
    if (parse_args(&opts, argc, argv) != 0) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    /* read function pointer */
    ssize_t (*read_func)(int, void *, size_t);

    read_func = read; /* use read() from unistd.h for encode mode */
    if (opts.mode == DECODE || opts.mode == BEAUTIFY)
        read_func = readhex; /* use readhex() for decode and beautify modes */

    int ifile = STDIN_FILENO;  /* input file descriptor */
    int ofile = STDOUT_FILENO; /* output file descriptor */

    /* ibuf is twice size to store encode output for beautify input */
    char *ibuf = malloc(BUFSIZE * 2);
    char *obuf = NULL;

    if (ibuf == NULL) {
        fprintf(stderr, "error: unable to allocate space for input buffer");
        exit(EXIT_FAILURE);
    }

    int exit_status = EXIT_SUCCESS;
    int nbytes = 0, rbytes = 0, wbytes = 0;

    rbytes = read_func(ifile, ibuf, BUFSIZE);
    while (rbytes > 0) {
        switch(opts.mode) {
            case ENCODE:
                nbytes = encode(&obuf, ibuf, rbytes);
                if (nbytes < 0)
                    break; /* break on encode failure */
                /* redirect encode output into beautify input */
                memcpy(ibuf, obuf, nbytes);
                rbytes = nbytes;
            case BEAUTIFY:
                nbytes = beautify(&obuf, ibuf, rbytes);
                break;
            case DECODE:
                nbytes = decode((void **)&obuf, ibuf, rbytes);
                break;
            default:
                /* should never reach here */
                break;
        }
        if (nbytes < 0) {
            /* encode/decode/beautify failure */
            exit_status = EXIT_FAILURE;
            break;
        }

        wbytes = writeall(ofile, obuf, nbytes);
        if (wbytes < 0) {
            exit_status = EXIT_FAILURE;
            break;
        }

        rbytes = read_func(ifile, ibuf, BUFSIZE);
    }
    if (rbytes < 0) {
        exit_status = EXIT_FAILURE;
    }

    free(ibuf);
    free(obuf);

    close(ifile);
    close(ofile);

    exit(exit_status);
}


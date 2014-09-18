/*
 * Copyright (c) 2014 Damian Jason Lapidge
 *
 * The contents of this file are subject to the terms and conditions defined 
 * within the file LICENSE.txt, located within this project's root directory.
 */

#include <wordspace.h>

/*
 * string.h: strlen(), strchr()
 */
#include <string.h>

static const char *version = "v1.0.0";
static const char *authors = "Damian Jason Lapidge <grey@greydamian.org>";

char *getword(long idx, char *word, const char *chars, unsigned int len) {
    if (len < 1)
        return word;

    int chars_len = strlen(chars);
    int chars_idx = idx % chars_len;

    word[len - 1] = chars[chars_idx];

    return getword(idx / chars_len, word, chars, len - 1);
}

long getindex(const char *word, const char *chars, unsigned int len) {
    if (len < 1)
        return 0;

    int chars_len = strlen(chars);
    int chars_idx = strchr(chars, word[len - 1]) - chars;

    return chars_idx + chars_len * getindex(word, chars, len - 1);
}

long spacesize(const char *chars, unsigned int len) {
    if (len < 1)
        return 0; /* empty word space */
    if (len < 2)
        return strlen(chars);
    return strlen(chars) * spacesize(chars, len - 1);
}


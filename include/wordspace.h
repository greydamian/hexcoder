/*
 * Copyright (c) 2014 Damian Jason Lapidge
 *
 * The contents of this file are subject to the terms and conditions defined 
 * within the file LICENSE.txt, located within this project's root directory.
 */

#ifndef WORDSPACE_H
#define WORDSPACE_H

/*
 * Computes the word located at an index within a wordspace. Takes parameters 
 * for the index within the wordspace {idx}, a pointer to the null-terminated 
 * character set of the wordspace {chars}, the word length of the wordspace 
 * {len}.
 *
 * On success, a pointer to the resulting word is returned. On error, results 
 * are not specified.
 */
char *getword(long idx, char *word, const char *chars, unsigned int len);

/*
 * Computes the index for the location of a word within a wordspace. Takes 
 * parameters for a pointer to the null-terminated word, a pointer to the 
 * null-terminated character set of the wordspace {chars}, the word length 
 * {len}.
 *
 * On success, the index of the word is returned. On error, results are not 
 * specified.
 *
 * TODO: defend against NULL returned by {strchr()} when char in {word} is not 
 * found in {chars}.
 */
long getindex(const char *word, const char *chars, unsigned int len);

/*
 * Computes the size of a wordspace. The size of a wordspace is defined as the 
 * number of unique words within the wordspace.
 *
 * On success, the size of the wordspace is returned. On error, -1 is returned.
 *
 * TODO: defend against integer overflow.
 */
long spacesize(const char *chars, unsigned int len);

#endif /* ifndef WORDSPACE_H */


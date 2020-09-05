#ifndef brainfuc_common_h
#define brainfuc_common_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Report an error to the user and
 * that's it, pretty much.
 */
void reportError(const char* error);
/**
 * Given a filename, get the source
 * from the file and return the source
 * code as a cstring.
 */
const char* getSource(const char* fileName);
size_t getSourceLength(const char* source);
#endif
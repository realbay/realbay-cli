
#ifndef REALBAY_KEYWORD_H
#define REALBAY_KEYWORD_H

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#define MAX_KEYWORD_LENGTH 32
#define MAX_TEXT_LENGTH 1024

typedef void (keyword_func)(const char *keyword, const size_t len);

size_t keyword_parse(const char *text, keyword_func *f);

#endif

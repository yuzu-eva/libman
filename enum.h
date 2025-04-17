#ifndef _ENUM_
#define _ENUM_

#include <string.h>

typedef enum {
    GET,
    SET,
    ADD,
    ANIME,
    MANGA,
    BOOK,
} args_e;

static const struct {
    args_e val;
    const char *str;
} conversion_args [] = {
    {GET, "get"},
    {SET, "set"},
    {ADD, "add"},
    {ANIME, "anime"},
    {MANGA, "manga"},
    {BOOK, "book"},
};

args_e str2enum(const char *str);

#endif

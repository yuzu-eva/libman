#ifndef _ENUM_
#define _ENUM_

#include <string.h>

typedef enum {
    ANIME,
    MANGA,
} target_e;

typedef enum {
    GET,
    SET,
    ADD,
} mode_e;

static const struct {
    mode_e val;
    const char *str;
} conversion_mode [] = {
    {GET, "get"},
    {SET, "set"},
    {ADD, "add"},
};

static const struct {
    target_e val;
    const char *str;
} conversion_target [] = {
    {ANIME, "anime"},
    {MANGA, "manga"},
};

mode_e str2enum_mode(const char *str);
target_e str2enum_target(const char *str);

#endif

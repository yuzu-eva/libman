#include "enum.h"

mode_e str2enum_mode(const char *str)
{
    for (size_t i = 0; i < sizeof(conversion_mode) / sizeof(conversion_mode[0]); ++i) {
        if (!strcmp(str, conversion_mode[i].str))
            return conversion_mode[i].val;
    }
    return -1;
}

target_e str2enum_target(const char *str)
{
    for (size_t i = 0; i < sizeof(conversion_target) / sizeof(conversion_target[0]); ++i) {
        if (!strcmp(str, conversion_target[i].str))
            return conversion_target[i].val;
    }
    return -1;
}

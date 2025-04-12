#include "enum.h"

args_e str2enum(const char *str)
{
    for (size_t i = 0; i < sizeof(conversion_args) / sizeof(conversion_args[0]); ++i) {
        if (!strcmp(str, conversion_args[i].str))
            return conversion_args[i].val;
    }
    return -1;
}


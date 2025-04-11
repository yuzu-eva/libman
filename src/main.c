#include <stdint.h>

#include "dbhandling.h"

const uint8_t PATH_MAX = 64;
const char *filepath = ".local/share/sqlite";
const char *filename = "library.db";

typedef enum {
    GET,
    SET,
    ADD,
} mode_e;

static const struct {
    mode_e val;
    const char *str;
} conversion [] = {
    {GET, "get"},
    {SET, "set"},
    {ADD, "add"},
};

mode_e str2enum(const char *str)
{
    for (size_t i = 0; i < sizeof(conversion) / sizeof(conversion[0]); ++i) {
        if (!strcmp(str, conversion[i].str))
            return conversion[i].val;
    }
    return -1;
}

void print_help(void)
{
    printf("\n");
    printf("usage: myal MODE TARGET NAME [EPISODE|CHAPTER]          \n");
    printf("possible modes are: get|set|add                         \n");
    printf("possible targets are: anime|manga                       \n");
    printf("EXAMPLES: myal get anime %% | Prints all anime           \n");
    printf("          myal set manga Murcielago 10 | Set chapter of " \
    "Murcielago to 10\n");
    printf("mode get is fuzzy; set and add have to match exactly    \n");
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr, "missing argument...\n");
        print_help();
        exit(69);
    }

    mode_e mode;
    char *target, *name, *value, *status;
    mode = str2enum(argv[1]);
    target = argv[2];
    name = argv[3];

    char fullpath[PATH_MAX];
    snprintf(fullpath, PATH_MAX, "%s/%s/%s", getenv("HOME"), filepath, filename);

    sqlite3 *db;
    int rc;

    rc = sqlite3_open(fullpath, &db);

    if (rc) {
        exit_with_error(db, "Can't open database: ");
    }

    switch (mode) {
    case GET:
        select_from_table(db, target, name);
        break;
    case SET:
        if (argc < 5) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        if (argc == 5) {
            value = argv[4];
            update_entry(db, target, name, value, NULL);
        } else if (argc == 6) {
            value = argv[4];
            status = argv[5];
            update_entry(db, target, name, value, status);
        }
        break;
    case ADD:
        if (argc < 6) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        value = argv[4];
        status = argv[5];
        add_entry(db, target, name, value, status);
        break;
    default:
        fprintf(stderr, "unknown option...\n");
        print_help();
        sqlite3_close(db);
        exit(69);
    }

    sqlite3_close(db);
    return 0;
}

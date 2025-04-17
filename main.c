#include <stdint.h>

#include "dbhandling.h"
#include "enum.h"

const uint8_t PATH_MAX = 64;
const char *filepath = ".local/share/sqlite";
const char *filename = "library.db";

typedef struct {
    args_e target;
    char *name;
    char *author;
    char *value;
    char *status;
} entry_t;

void print_help(void)
{
    printf("\n");
    printf("usage: myal MODE TARGET NAME [EPISODE|CHAPTER]          \n");
    printf("possible modes are: get|set|add                         \n");
    printf("possible targets are: anime|manga|book                  \n");
    printf("EXAMPLES: myal get anime %% | Prints all anime          \n");
    printf("          myal set manga Murcielago 10 | Set chapter of " \
    "Murcielago to 10\n");
    printf("mode get is fuzzy; set and add have to match exactly    \n");
    printf("See more examples in the readme.                        \n");
    printf("\n");
}

entry_t *set_entry(entry_t *entry, int argc, char **argv)
{
    entry->target = str2enum(argv[2]);
    entry->name = argv[3];
    if (entry->target == BOOK) {
        entry->author = argv[4];
        if (argc == 6) {
            entry->value = argv[5];
            entry->status = NULL;
        } else if (argc == 7) {
            entry->value = argv[5];
            entry->status = argv[6];
        }
    } else {
        entry->author = NULL;
        if (argc == 5) {
            entry->value = argv[4];
            entry->status = NULL;
        } else if (argc == 6) {
            entry->value = argv[4];
            entry->status = argv[5];
        }
    }
    return entry;
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr, "missing argument...\n");
        print_help();
        exit(69);
    }

    args_e mode;
    entry_t *entry = malloc(sizeof(entry_t));
    mode = str2enum(argv[1]);

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
        set_entry(entry, argc, argv);
        select_from_table(db, entry->target, entry->name);
        break;
    case SET:
        if (argc < 5) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        set_entry(entry, argc, argv);
        update_entry(db, entry->target, entry->name, entry->value, entry->status);
        break;
    case ADD:
        if (argc < 6) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        set_entry(entry, argc, argv);
        add_entry(db, entry->target, entry->name, entry->author, entry->value, entry->status);
        break;
    default:
        fprintf(stderr, "unknown option...\n");
        print_help();
        sqlite3_close(db);
        exit(69);
    }

    sqlite3_close(db);
    free(entry);
    return 0;
}

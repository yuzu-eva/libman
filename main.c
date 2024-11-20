#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *filename = "./anime.csv";

typedef struct {
    char name[80];
    char episode[12];
} entry_t;

typedef enum {
    MATCH_MODE,
    SEARCH_MODE,
    APPEND_MODE,
    EDIT_MODE,
} mode_e;

void print_help()
{
    printf("usage: myal [-s <name>] [-ae <name> <episode>]                   \n");
    printf(" Prints all entries by default                                   \n");
    printf("\n");
    printf(" -m <name>          Prints all anime containing the given string \n");
    printf(" -s <name>          Search for a specific anime by name          \n");
    printf(" -a <name> <number> Append new anime and episode to file         \n");
    printf(" -e <name> <number> Edit the episode number of an anime          \n");
    printf("\n");
}

char *getfield(char *line, int num)
{
    char *tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",")) {
        if (!--num) {
            return tok;
        }
    }
    return NULL;
}

// print everything
void print_all()
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        printf("Name: %s Episode: %s", name, episode);
        free(tmp);
    }
    fclose(fp);
}

// print any entry containing the search string
void print_matches(const char *sel)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    int entry_found = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        if (strcasestr(name, sel) != NULL) {
            printf("Name: %s Episode: %s", name, episode);
            entry_found = 1;
        }
        free(tmp);
    }
    if (!entry_found) {
        printf("Entry not found\n");
    }
    fclose(fp);
}

// print exactly one entry, comparing from the first character
void print_single_match(const char *sel)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    int entry_found = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        if (strncasecmp(name, sel, strlen(sel)) == 0) {
            printf("Name: %s Episode: %s", name, episode);
            entry_found = 1;
        }
        free(tmp);
    }
    if (!entry_found) {
        printf("Entry not found\n");
    }
    fclose(fp);
}

// construct an entry from the give command line arguments
entry_t *get_entry(char **argv, int pos)
{
    entry_t *res = malloc(sizeof(entry_t));
    if (res == NULL) {
        fprintf(stderr, "ERROR: allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(res->name, argv[pos], sizeof(res->name) - 1);
    res->name[sizeof(res->name) - 1] = '\0';
    if (argv[pos + 1]) {
        strncpy(res->episode, argv[pos + 1], sizeof(res->episode) - 1);
    }
    res->episode[sizeof(res->episode) - 1] = '\0';

    return res;
}

// append new entry to the end of the csv
void append_entry(const entry_t *entry)
{
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s,%s\n", entry->name, entry->episode);
}

// edit the episode number of an anime
void edit_entry(const entry_t *entry)
{
    FILE *fp_old = fopen(filename, "r");
    FILE *fp_new = fopen("temp.csv", "w");
    if (fp_old == NULL || fp_new == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    int entry_found = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp_old)) {
        char *tmp = strdup(line);
        char *old_name = getfield(tmp, 1);
        tmp = strdup(line);
        char *old_ep = getfield(tmp, 2);
        
        if (strncasecmp(old_name, entry->name, strlen(entry->name)) == 0) {
            fprintf(fp_new, "%s,%s\n", old_name, entry->episode);
            entry_found = 1;
        } else {
            fprintf(fp_new, "%s,%s", old_name, old_ep);
        }
        free(tmp);
    }
    if (!entry_found) {
        printf("Entry not found\n");
    }
    fclose(fp_old);
    fclose(fp_new);
    remove(filename);
    rename("temp.csv", filename);
}

void check_args(int argc, int expected_count) {
    if (argc < expected_count) {
        fprintf(stderr, "ERROR: missing argument\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    if (argc > expected_count) {
        fprintf(stderr, "ERROR: too many arguments\n");
        print_help();
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char **argv)
{
    mode_e mode;
    int opt;
    while ((opt = getopt(argc, argv, "msae")) != -1) {
        switch (opt) {
        case 'm': mode = MATCH_MODE; break;
        case 's': mode = SEARCH_MODE; break;
        case 'a': mode = APPEND_MODE; break;
        case 'e': mode = EDIT_MODE; break;
        default:
            print_help();
            exit(EXIT_FAILURE);
        }
    }

    if (optind == 1 && argc == 1) {
        print_all();
        return 0;
    }

    entry_t *entry = NULL;
    
    switch (mode) {
    case MATCH_MODE:
        check_args(argc, 3);
        entry = get_entry(argv, optind);
        if (entry == NULL) {
            fprintf(stderr, "ERROR: failed to allocate memory for entry\n");
            exit(EXIT_FAILURE);
        }
        print_matches(entry->name);
        break;
    case SEARCH_MODE:
        check_args(argc, 3);
        entry = get_entry(argv, optind);
        if (entry == NULL) {
            fprintf(stderr, "ERROR: failed to allocate memory for entry\n");
            exit(EXIT_FAILURE);
        }
        print_single_match(entry->name);
        break;
    case APPEND_MODE:
        check_args(argc, 4);
        entry = get_entry(argv, optind);
        if (entry == NULL) {
            fprintf(stderr, "ERROR: failed to allocate memory for entry\n");
            exit(EXIT_FAILURE);
        }
        append_entry(entry);
        break;
    case EDIT_MODE:
        check_args(argc, 4);
        entry = get_entry(argv, optind);
        if (entry == NULL) {
            fprintf(stderr, "ERROR: failed to allocate memory for entry\n");
            exit(EXIT_FAILURE);
        }
        edit_entry(entry);
        break;
    default:
        print_help();
        break;
    }
    free(entry);
    return 0;
}

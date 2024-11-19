#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *filename = "./anime.csv";

typedef struct {
    char name[80];
    char episode[5];
} entry_t;

typedef enum {
    MATCH_MODE,
    SEARCH_MODE,
    APPEND_MODE,
    EDIT_MODE,
    PRINT_MODE
} mode_e;

void print_help()
{
    printf("usage: myal [-s <name>] [-ae <name> <episode>]                   \n");
    printf(" Prints all entries by default                                   \n");
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

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        if (strcasestr(name, sel) != NULL) {
            printf("Name: %s Episode: %s", name, episode);
        }
        free(tmp);
    }
    fclose(fp);
}

// return one single entry matching the search string from the first character
entry_t *get_entry(const char *sel)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    entry_t *res = malloc(sizeof(entry_t));
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        if (strncasecmp(name, sel, strlen(sel)) == 0) {
            strcpy(res->name, name);
            strcpy(res->episode, episode);
            break;
        }
        free(tmp);
    }
    fclose(fp);
    return res;
}

// append new entry to the end of the csv
void append_entry(const char *name, const char *ep)
{
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    entry_t *entry = malloc(sizeof(entry_t));
    strncpy(entry->name, name, sizeof(entry->name));
    strncpy(entry->episode, ep, sizeof(entry->episode));
    fprintf(fp, "%s,%s\n", entry->name, entry->episode);
}

// edit the episode number of an anime
void edit_entry(const char *name, const char *new_ep)
{
    FILE *fp_old = fopen(filename, "r");
    FILE *fp_new = fopen("temp.csv", "w");
    if (fp_old == NULL || fp_new == NULL) {
        fprintf(stderr, "ERROR: no such file\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    while (fgets(line, sizeof(line), fp_old)) {
        char *tmp = strdup(line);
        char *old_name = getfield(tmp, 1);
        tmp = strdup(line);
        char *old_ep = getfield(tmp, 2);
        
        if (strcasestr(old_name, name) != NULL) {
            fprintf(fp_new, "%s,%s\n", old_name, new_ep);
        } else {
            fprintf(fp_new, "%s,%s", old_name, old_ep);
        }
        free(tmp);
    }
    remove(filename);
    rename("temp.csv", filename);
    fclose(fp_old);
    fclose(fp_new);
}

int main(int argc, char **argv)
{
    mode_e mode = PRINT_MODE;
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

    if (optind >= argc && mode != PRINT_MODE) {
        fprintf(stderr, "ERROR: missing argument\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    if (optind == 1 && argc == 2) {
        fprintf(stderr, "ERROR: no option for argument given\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    
    switch (mode) {
    case MATCH_MODE:
        print_matches(argv[optind]);
        break;
    case SEARCH_MODE:
        // maybe useful for sanitizing input, but not needed for now
        // 
        // size_t buffer_size = strlen(argv[optind]) + 1;
        // char *selection = (char*)malloc(buffer_size);

        // if (selection == NULL) {
        //     fprintf(stderr, "ERROR: failed to allocate memory for buffer\n");
        //     exit(EXIT_FAILURE);
        // }

        // strncpy(selection, argv[optind], buffer_size - 1);
        // selection[buffer_size - 1] = '\0';
        // [...]
        // free(selection);

        entry_t *res = get_entry(argv[optind]);
        if (res->name[0] == '\0') {
            printf("Entry not found");
            break;
        }
        printf("Name: %s Episode: %s", res->name, res->episode);
        break;
    case APPEND_MODE:
        append_entry(argv[optind], argv[optind+1]);
        break;
    case EDIT_MODE:
        edit_entry(argv[optind], argv[optind+1]);
        break;
    case PRINT_MODE:
        print_all();
        break;
    default:
        print_help();
        break;
    }
    return 0;
}

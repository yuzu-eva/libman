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
    SEARCH_MODE,
    APPEND_MODE,
    EDIT_MODE,
    PRINT_MODE
} mode_e;

void print_help()
{
    printf("usage: myal [-s <name>] [-ae <name> <episode>]\n");
    printf(" Prints all entries by default                                \n");
    printf(" -s <name>          Search for a specific anime by name       \n");
    printf(" -a <name> <number> Append new anime and episode to file      \n");
    printf(" -e <name> <number> Edit the episode number of an anime       \n");
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
void *print_all(FILE *fp)
{
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        printf("Name: %s Episode: %s", name, episode);
        free(tmp);
    }
}

// print any entry containing the search string
void *print_matches(FILE *fp, const char *sel)
{
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
}

// return one single entry matching the search string from the first character
entry_t *get_entry(FILE *fp, const char *sel)
{
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
    return res;
}

int main(int argc, char **argv)
{
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        fprintf(stderr, "No such file\n");
        exit(EXIT_FAILURE);
    }

    mode_e mode = PRINT_MODE;
    int opt;
    while ((opt = getopt(argc, argv, "sae")) != -1) {
        switch (opt) {
        case 's': mode = SEARCH_MODE; break;
        case 'a': mode = APPEND_MODE; break;
        case 'e': mode = EDIT_MODE; break;
        default:
            print_help();
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }

    if (mode == PRINT_MODE) {
        print_all(fp);
        fclose(fp);
        exit(EXIT_SUCCESS);
    }

    if (optind >= argc) {
        fprintf(stderr, "ERROR: missing argument\n");
        print_help();
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    
    size_t buffer_size = strlen(argv[optind]) + 1;
    char *selection = (char*)malloc(buffer_size);

    if (selection == NULL) {
        fprintf(stderr, "Failed to allocate memory for buffer!\n");
        exit(EXIT_FAILURE);
    }

    strncpy(selection, argv[optind], buffer_size - 1);
    selection[buffer_size - 1] = '\0';

    switch (mode) {
    case SEARCH_MODE:
        print_matches(fp, selection);
        break;
    case APPEND_MODE:
    case EDIT_MODE:
    default:
        print_help();
        break;
    }
    
    free(selection);
    fclose(fp);
    
    return 0;
}

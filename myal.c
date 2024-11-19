#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *filename = "./anime.csv";

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

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }
    
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "No such file\n");
        return 1;
    }

    size_t buffer_size = strlen(argv[1]) + 1;
    char *selection = (char*)malloc(buffer_size);

    if (selection == NULL) {
        fprintf(stderr, "Failed to allocate memory for buffer!\n");
        return 1;
    }

    strncpy(selection, argv[1], buffer_size - 1);
    selection[buffer_size - 1] = '\0';

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strdup(line);
        char *name = getfield(tmp, 1);
        tmp = strdup(line);
        char *episode = getfield(tmp, 2);

        if (strncasecmp(name, selection, strlen(selection)) == 0) {
            printf("Name: %s Episode: %s", name, episode);
        }
        free(tmp);
    }
    
    free(selection);
    fclose(fp);
    
    return 0;
}

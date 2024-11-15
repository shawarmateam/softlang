#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT 0xfe

char* readFile(const char* filename) {
    FILE *file;
    char line[256];
    char *content = NULL;
    size_t total_length = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "\033[31mSFT001\033[0m: Error during open file: \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        size_t line_length = strlen(line);

        char *new_content = realloc(content, total_length + line_length + 1);
        if (new_content == NULL) {
            perror("\033[31mSFT002\033[0m: Error during allocate memory.");
            free(content);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        content = new_content;

        strcpy(content + total_length, line);
        total_length += line_length;
    }

    fclose(file);
    return content;
}

int main(int argv, char **argc) {
    if (argv < 2) return 1;
    unsigned char* data = "";

    unsigned char tokens[] = {};

    for (int i=0;tokens[i]!=255;i++) {
        switch (tokens[i]) {
        }
    }

    return 0;
}

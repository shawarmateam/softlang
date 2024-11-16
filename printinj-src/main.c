#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PRINT  254
#define ADDDT  253
#define SEMI   252
#define MEMSET 251

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

char containsAtI(const char* str, const char* substring, int index) {
    if (index < 0 || index >= strlen(str)) return 0x00;

    return strncmp(str + index, substring, strlen(substring)) == 0;
}

unsigned char** splitStr(const char* str, int* count) {
    // Сначала подсчитаем количество слов
    int words = 0;
    const char* temp = str;
    while (*temp) {
        while (*temp == ' ') temp++; // Пропускаем пробелы
        if (*temp) {
            words++;
            while (*temp && *temp != ' ') temp++; // Пропускаем слово
        }
    }

    // Выделяем память для массива указателей
    unsigned char** result = malloc(words * sizeof(unsigned char*));
    if (!result) {
        return NULL; // Ошибка выделения памяти
    }

    // Разбиваем строку
    int index = 0;
    char* token = strtok(strdup(str), " "); // Копируем строку, чтобы не изменять оригинал
    while (token) {
        result[index] = (unsigned char*)malloc(strlen(token) + 1);
        if (!result[index]) {
            // Освобождаем ранее выделенную память в случае ошибки
            for (int j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        strcpy((char*)result[index], token);
        index++;
        token = strtok(NULL, " ");
    }

    *count = words; // Возвращаем количество слов
    return result;
}

char isNum(const char *str) {
    if (*str == '\0') return 0x00;
    while (*str) {
        if (!isdigit(*str)) {
            return 0x00;
        }
        str++;
    }
    return 0xff;
}

unsigned char *tokenParse(char *file) {
    int tokens_len = 0;
    for (int i=0; file[i]!=0;++i) {
        if (file[i] == '\n') file[i]=' ';
    } printf("FILE: {%s}\n", file);

    unsigned char** tokensStr = splitStr(file, &tokens_len);
    unsigned char *tokens = malloc(tokens_len);
    for (int i=0; i<tokens_len; ++i) {
        printf("TOKENS STR: (%s)\n", tokensStr[i]);
        if (strcmp(tokensStr[i], "memset")==0) tokens[i]=MEMSET;
        else if (strcmp(tokensStr[i], "adddt")==0) tokens[i]=ADDDT;
        else if (strcmp(tokensStr[i], "print")==0) tokens[i]=PRINT;
        else if (strcmp(tokensStr[i], ";")==0) tokens[i]=SEMI;
        else if (isNum(tokensStr[i])) tokens[i]=atoi(tokensStr[i]);
        //else if (tokensStr[i][0]=='"' && tokensStr[i][strlen(tokensStr[i]-1)]=='"') {
        //    for (int j=1; tokenStr[i][j]!='"'; ++j) {
        //        // TODO: finish this
        //    }
        //}
    }
    tokens[tokens_len-1]=0xff;
    return tokens;
}

int main(int argv, char **argc) {
    if (argv < 2) return 1;
    unsigned char* data;
    char isNewCmd = 0xff; // boolean (for ';')
    /*unsigned char tokens[] = {
        MEMSET, 12,SEMI,
        ADDDT,  'h','e','l','l','o',' ','w','o','r','l','d','\n',SEMI,
        PRINT,  0,  11,SEMI,
        0xff
    };*/
    char *file = readFile(argc[1]);
    unsigned char *tokens = tokenParse(file);

    for (int i=0;tokens[i]!=255;i++) {
        printf("TOKEN: %02x (%d)\n", tokens[i], tokens[i]);
        if (!isNewCmd) {
            if (tokens[i] == SEMI) isNewCmd = 0xff;
            continue;
        }


        unsigned char temp[tokens[i+2]+2];
        switch (tokens[i]) {
            case PRINT:
                strncpy((char*)temp, (char*)data, tokens[i+2]+1);
                temp[tokens[i+2]+1] = '\0';

                printf("%s", temp);
                isNewCmd=0x00;
                break;
            case ADDDT:
                int j;
                for (j=0; tokens[j+i]!=SEMI;++j) {
                    // TODO: сделать функцию чтобы можно было создавать новый * с +1 ячейкой (realloc)
                    data[j]=tokens[i+j+1];
                }
                i=i+j;
                break;
            case MEMSET:
                if (tokens[i+1]==SEMI) exit(5);
                data=malloc(tokens[i+1]);
                break;
        }
    }

    return 0;
}

#include "common.h"

size_t getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

const char* getSource(const char* fileName) {
    // This is mostly taken from Crafting Interpreters
    // Though it seems like it is used by everbody,
    // I still wanted to note it.
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
        reportError("Specified source file not found.");
    }
    size_t fileSize = getFileSize(file);
    char* buffer = (char*) malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char),
                            fileSize, file);
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}

void reportError(const char* error) {
    fprintf(stderr, "%s", error);
}

size_t getSourceLength(const char* source) {
    const char *c = source;
    int size = 0;
    while (*c != '\0') {
        size++;
    } 
    return size;
}
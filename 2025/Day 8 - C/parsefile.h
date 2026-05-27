#include <stdio.h>
#include <stdlib.h>

#ifndef PARSEFILE
#define PARSEFILE

typedef struct {
    int count;
    char *data_ptr;
} Puzzle;

Puzzle parseFile(char* file_name){
    Puzzle output;

    FILE* file_ptr = fopen(file_name, "r");

    if (file_ptr == NULL) {
        fclose(file_ptr);
        output.data_ptr = NULL;
        return output;
    }

    //get the size of the file
    fseek(file_ptr, 0, SEEK_END);
    unsigned long int file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    //allocate enough space to fit the whole file.
    output.data_ptr = (char*)malloc(file_size + 1);
    if (output.data_ptr == NULL){
        fclose(file_ptr);
        return output;
    }

    fread(output.data_ptr, sizeof(char), file_size, file_ptr);
    output.data_ptr[file_size] = '\0';

    for (unsigned long int i = 0; i <= file_size; i++){
        if (output.data_ptr[i] == '\n') {
            output.count++;
        }
    }

    fclose(file_ptr);
    return output;
}

#endif

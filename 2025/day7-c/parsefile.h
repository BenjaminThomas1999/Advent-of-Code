#include <stdio.h>
#include <stdlib.h>

#ifndef PARSEFILE
#define PARSEFILE

typedef struct {
    short unsigned int width;
    short unsigned int height;
    char *data_ptr;
} Puzzle;

Puzzle parseFile(char* file_name){
    Puzzle output;

    output.width = 0;
    output.height = 0;
    FILE* file_ptr = fopen(file_name, "r");

    if (file_ptr == NULL) {
        fclose(file_ptr);
        output.data_ptr = NULL;
        return output;
    }

    //get the size of the file
    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    //allocate enough space to fit the whole file.
    output.data_ptr = malloc(file_size + 1);
    if (output.data_ptr == NULL){
        fclose(file_ptr);
        return output;
    }

    fread(output.data_ptr, sizeof(char), file_size, file_ptr);
    output.data_ptr[file_size] = '\0';

    for (int i = 0; i <= file_size; i++){
        if (output.data_ptr[i] == '\n') {
            output.width = i + 1;
            break;
        }
    }

    output.height = file_size/output.width;
    //don't consider \n in output.width
    output.width -= 1;

    fclose(file_ptr);
    return output;
}

#endif

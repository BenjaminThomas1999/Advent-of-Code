//todo: add ifdef guards
#include <stdio.h>
#include <stdlib.h>

#ifndef SCREENLIB
#define SCREENLIB

typedef struct {
    int width;
    int height;
    char* data_ptr;
} Screen;


char get(Screen* input, int x, int y){
    return input->data_ptr[y*input->width + x];
}

void draw(Screen* screen, char c, int x, int y){
    //to do: add guard rails
    screen->data_ptr[y*screen->width + x] = c;
}

void printScreen(Screen* screen){
    int cursor = 0;
    while (cursor < screen->width*screen->height){
        printf("%c", screen->data_ptr[cursor++]);
        if (cursor%screen->width == 0){
            printf("\n");
        }
    }
}

void clearScreen(Screen* screen, char* background){
    printf("\033[2J\033[H");
    //set background
    int cursor = 0;
    for (int y = 0; y < screen->height; y++){
        for (int x = 0; x < screen->width; x++){
             screen->data_ptr[cursor++] = background[y*(screen->width+1)+x];
        }
    }
}

#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "parsefile.h"
#include "screenlib.h"

typedef struct Beam Beam;
struct Beam{
    int tail_length;
    int x;
    int y;
    int64_t timeline_count;
    bool printed;

    Beam* left_child;
    Beam* right_child;
};

static const Beam BEAM_DEFAULT = {1, 0, 1, 0, false, NULL, NULL};

void propogateBeam(Screen* screen, Beam* base_beam);

void splitBeam(Beam* base_beam, Beam*** beam_buffer, int* beam_buffer_len, Beam*** next_beam_buffer, int* next_beam_buffer_len);

void walkBeam(Beam* base_beam, Beam*** beam_buffer, int* beam_buffer_len, Beam*** next_beam_buffer, int* next_beam_buffer_len);

void printBeam(Screen* screen, Beam* base_beam, bool fast_print);

void print_binary(unsigned int x, int bits);

void navigateTree(Beam* base_beam);

void freeTree(Beam* base_beam);

int main() {
    Puzzle input = parseFile("input");
    // Puzzle input = parseFile("test_input");
    Screen output;
    output.data_ptr = malloc(input.width*input.height + 1);

    if (output.data_ptr == NULL){
        free(input.data_ptr);
        return 1;
    }

    output.data_ptr[input.width*input.height] = '\0';
    output.width = input.width;
    output.height = input.height;
    clearScreen(&output, input.data_ptr);

    Beam* base_beam = malloc(sizeof(Beam));
    *base_beam = BEAM_DEFAULT;
    
    //find start
    for(int x = 0; x < output.width; x++){
        if (get(&output, x, 0) == 'S'){
            base_beam->x = x;
            break;
        }
    }

    clearScreen(&output, input.data_ptr);
    propogateBeam(&output, base_beam);
    navigateTree(base_beam);

    printBeam(&output, base_beam, false);
    printf("Timeline Count: %" PRId64, base_beam->timeline_count);

    // freeTree(base_beam);
    free(output.data_ptr);
    free(input.data_ptr);

    return 0;
}

void propogateBeam(Screen* screen, Beam* base_beam){
    int BUFFER_SIZE = sizeof(Beam*)*screen->width;
    Beam** beam_buffer = malloc(BUFFER_SIZE);//no malloc check. could improve.
    beam_buffer[0] = base_beam;
    int beam_buffer_len = 1;


    int working_line = 1;

    while(working_line < screen->height){
        Beam** next_beam_buffer = malloc(BUFFER_SIZE);//no malloc check, could improve.
        int next_beam_buffer_len = 0;

        for (int i = 0; i < beam_buffer_len; i++){
            char cursor = get(screen, beam_buffer[i]->x, beam_buffer[i]->y+1);
            if (cursor == '^'){
                splitBeam(beam_buffer[i], &beam_buffer, &beam_buffer_len, &next_beam_buffer, &next_beam_buffer_len);
            }
            else {
                walkBeam(beam_buffer[i], &beam_buffer, &beam_buffer_len, &next_beam_buffer, &next_beam_buffer_len);
            }
        }

        memcpy(beam_buffer, next_beam_buffer, next_beam_buffer_len*sizeof(Beam*));
        beam_buffer_len = next_beam_buffer_len;
        free(next_beam_buffer);
        working_line++;
    }
}

void splitBeam(Beam* base_beam, Beam*** beam_buffer, int* beam_buffer_len, Beam*** next_beam_buffer, int* next_beam_buffer_len){

    bool left_child_pos_occupied = 0;

    for (int i = 0; i < *next_beam_buffer_len; i++){
        if (base_beam->x-1 == (*next_beam_buffer)[i]->x){
            left_child_pos_occupied = true;
            base_beam->left_child = (*next_beam_buffer)[i];
            break;
        }
    }
    if (!left_child_pos_occupied){
        Beam* left_child = malloc(sizeof(Beam));
        *left_child = BEAM_DEFAULT;
        left_child->x = base_beam->x-1;
        left_child->y = base_beam->y+1;

        (*next_beam_buffer)[(*next_beam_buffer_len)++] = left_child;
        base_beam->left_child = left_child;
    }

    bool right_child_pos_occupied = 0;

    for (int i = 0; i < *next_beam_buffer_len; i++){
        if (base_beam->x+1 == (*next_beam_buffer)[i]->x){
            right_child_pos_occupied = true;
            base_beam->right_child = (*next_beam_buffer)[i];
            break;
        }
    }

    if (!right_child_pos_occupied){
        Beam* right_child = malloc(sizeof(Beam));
        *right_child = BEAM_DEFAULT;
        right_child->x = base_beam->x+1;
        right_child->y = base_beam->y+1;

        (*next_beam_buffer)[(*next_beam_buffer_len)++] = right_child;
        base_beam->right_child = right_child;
    }
}

void walkBeam(Beam* base_beam, Beam*** beam_buffer, int* beam_buffer_len, Beam*** next_beam_buffer, int* next_beam_buffer_len){
    //check that position not already occupied.
    //if occupied link as a direct child 
    bool next_pos_occupied = false;

    for (int i = 0; i < *next_beam_buffer_len; i++){
        if (base_beam->x == (*next_beam_buffer)[i]->x){
            next_pos_occupied = true;
            base_beam->left_child = (*next_beam_buffer)[i];
            break;
        }
    }

    if (!next_pos_occupied){
        base_beam->y += 1;
        base_beam->tail_length += 1;
        (*next_beam_buffer)[(*next_beam_buffer_len)++] = base_beam;
    }
}

void printBeam(Screen* screen, Beam* beam, bool fast_print){

    if (!fast_print) { printf("\033[2J\033[H"); }
    
    for (int i = 0; i < beam->tail_length; i++){
        char cursor = get(screen, beam->x, beam->y-i);

        if (cursor == '.'){
            // draw(screen, '0' + beam->timeline_count, beam->x, beam->y-i);
            draw(screen, '|', beam->x, beam->y-i);
        }
        //debug values below to make sure overlaps are not happening
        else if (cursor == '^') {
            draw(screen, '*', beam->x, beam->y-i);
        }
        else if (cursor == '|') {
            draw(screen, '!', beam->x, beam->y-i);
        }
    }

    beam->printed = true;

    if (!fast_print){
        printScreen(screen);
        usleep(3000);
    }

    if (beam->left_child != NULL){
        if (beam->left_child->printed == false){
            printBeam(screen, beam->left_child, fast_print);
        }
    }
    if (beam->right_child != NULL){
        if (beam->right_child->printed == false){
            printBeam(screen, beam->right_child, fast_print);
        }
    }
}

void navigateTree(Beam* base_beam){
    if (base_beam->left_child == NULL && base_beam->right_child == NULL){
        base_beam->timeline_count = 1;
        return;
    }
    int64_t timeline_increment = 0;
    //if a left child exists...
    if (base_beam->left_child != NULL){
        //and if it hasn't already been explored.
        if (base_beam->left_child->timeline_count == 0){
            navigateTree(base_beam->left_child);
        }
        timeline_increment += base_beam->left_child->timeline_count;
    }
    if (base_beam->right_child != NULL){
        if (base_beam->right_child->timeline_count == 0){
            navigateTree(base_beam->right_child);
        }
        timeline_increment += base_beam->right_child->timeline_count;
    }

    base_beam->timeline_count += timeline_increment;

}

void freeTree(Beam* base_beam){
    //there's a problem with this relating to a child having two parents.
    if (base_beam->left_child != NULL){
        freeTree(base_beam->left_child);
        base_beam->left_child = NULL;
    }

    if (base_beam->right_child != NULL){
        freeTree(base_beam->right_child);
        base_beam->right_child = NULL;
    }

    free(base_beam);
}

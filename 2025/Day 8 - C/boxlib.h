#include <stdio.h>
#include <stdlib.h>

typedef struct JunctionBox JunctionBox;
struct JunctionBox{
    int x;
    int y;
    int z;
    float distance; //used for comparisons.
    char id; //for debugging.
    JunctionBox* p_closest_box;
    bool explored;
};

void parseBoxes(JunctionBox* points, char* input){
    int points_len = 0;
    int cursor = 0;
    char id = 'A';
    while (input[cursor] != '\0'){
        int* line_buffer = malloc(sizeof(int)*3);//only three numbers per line
        int line_buffer_len = 0;
        while (line_buffer_len < 3){
            char* number_buffer = malloc(sizeof(char)*4);//only three chars per number
            int number_buffer_len = 0;
            while (input[cursor] != ',' && input[cursor] != '\n'){
                number_buffer[number_buffer_len++] = input[cursor++];
            }
            number_buffer[number_buffer_len] = '\0';
            line_buffer[line_buffer_len++] = atoi(number_buffer);
            free(number_buffer);
            cursor++;//then go to the next line or number
        }
        JunctionBox p;
        p.x = line_buffer[0];
        p.y = line_buffer[1];
        p.z = line_buffer[2];
        p.distance = 0.0;//initialisation
        p.id = id++;//for debugging
        p.explored = false;
        points[points_len++] = p;
        free(line_buffer);
    }
}

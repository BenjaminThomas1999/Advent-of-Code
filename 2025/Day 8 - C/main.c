#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "parsefile.h"
#include "boxlib.h"

typedef struct {
    JunctionBox** boxes;
    int boxes_len;
    float total_length;
    bool deleted;
} Circuit;

double calcDistance(JunctionBox* box_1, JunctionBox* box_2){
    int dx = box_1->x - box_2->x;
    int dy = box_1->y - box_2->y;
    int dz = box_1->z - box_2->z;
    // return sqrt(dx*dx + dy*dy + dz*dz);
    return (dx*dx + dy*dy + dz*dz)/3.0;
}

void findClosestBoxes(JunctionBox* j_boxes, int j_boxes_len){
    for (int i = 0; i < j_boxes_len; i++){

        double shortest_distance = INT32_MAX;
        JunctionBox* closest_box = NULL;

        //search through other junction boxes
        for (int j = 0; j < j_boxes_len; j++){
            //don't compare a box to itself
            if (i == j){
                continue;
            }

            double distance = calcDistance(&(j_boxes[i]), &(j_boxes[j]));

            if (distance < shortest_distance){
                shortest_distance = distance;
                closest_box = &(j_boxes[j]);
            }
        }

        j_boxes[i].p_closest_box = closest_box;
        j_boxes[i].distance = shortest_distance;
    }

    for (int i = 0; i < j_boxes_len; i++){
        printf("id: %c, %d, %d, %d connected to: %c \n", j_boxes[i].id, j_boxes[i].x, j_boxes[i].y, j_boxes[i].z, j_boxes[i].p_closest_box->id);
    }
}

void buildCircuits(JunctionBox* j_box, Circuit** circuits, int* circuits_len){
    Circuit* circuit = malloc(sizeof(Circuit));

    circuit->boxes_len = 1;
    circuit->total_length = 0;//not yet implemented
    circuit->boxes = malloc(sizeof(JunctionBox));

    JunctionBox* current = j_box;
    JunctionBox* next = j_box->p_closest_box;
    JunctionBox* previous = NULL;

    printf("%c", current->id);
    while (next != previous) {
        circuit->boxes = realloc(circuit->boxes, sizeof(JunctionBox)*circuit->boxes_len);
        circuit->boxes[circuit->boxes_len] = current;
        circuit->boxes_len++;
        previous = current;
        current = next;
        next = current->p_closest_box;
        printf(" -> %c", current->id);
        current->explored = true;
    }

    *circuits_len = (*circuits_len)+1;
    circuits[*circuits_len] = circuit;

    printf("\n");

}

void connectCircuits(Circuit** circuits, int* circuits_len){

}

int main(){

    Puzzle input = parseFile("test_input");
    int j_boxes_len = input.count;
    JunctionBox* j_boxes = malloc(sizeof(JunctionBox)*j_boxes_len);
    parseBoxes(j_boxes, input.data_ptr);
    findClosestBoxes(j_boxes, j_boxes_len);

    Circuit** circuits = malloc(sizeof(Circuit*)*1000);//size enough for 1000 circuits
    int circuits_len = 0;

    for (int i = 0; i < j_boxes_len; i++){
        if(!j_boxes[i].explored){
            buildCircuits(&(j_boxes[i]), circuits, &circuits_len);
        }
    }

    printf("-----------\n");


    //below to test that my circuits data structure is correct
    for (int i = 0; i < circuits_len; i++){
        for (int j = 0; j < circuits[i]->boxes_len; j++){
            printf("->%c", circuits[i]->boxes[j]->id);
        }
        printf("\n");
    }

    //not yet implemented
    connectCircuits(circuits, &circuits_len);
    
    for (int i = 0; i < circuits_len; i++){
        free(circuits[i]->boxes);
        free(circuits[i]);
    }
    free(circuits);
    free(input.data_ptr);
    free(j_boxes);
    return 0;
}


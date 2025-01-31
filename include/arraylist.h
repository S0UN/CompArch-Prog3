#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdlib.h>

typedef struct {
    void *data;       
    int size;         
    int capacity;     
    size_t elementSize; 
} ArrayList;

// Function declarations
ArrayList createArrayList(int initialCapacity, size_t elementSize);
void addToArrayList(ArrayList *list, void *element);
void *getFromArrayList(ArrayList *list, int index);
void freeArrayList(ArrayList *list);

#endif 

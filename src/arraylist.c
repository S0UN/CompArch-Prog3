#include "arraylist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
ArrayList createArrayList(int initialCapacity, size_t elementSize) {
    ArrayList list;
    list.size = 0;
    list.capacity = initialCapacity;
    list.elementSize = elementSize;

    list.data = malloc(elementSize * initialCapacity);
    if (!list.data) {
        fprintf(stderr, "Error: Memory allocation failed for ArrayList\n");
        exit(1);
    }

    return list;
}


void addToArrayList(ArrayList *list, void *element)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 2;
        void *newData = realloc(list->data, list->capacity * list->elementSize);
        if (!newData)
        {
            fprintf(stderr, "Error: Memory reallocation failed for ArrayList\n");
            exit(1);
        }
        list->data = newData;
    }
    memcpy((char *)list->data + (list->size * list->elementSize), element, list->elementSize);
    list->size++;
}

void *getFromArrayList(ArrayList *list, int index)
{
    if (index < 0 || index >= list->size)
    {
        fprintf(stderr, "Error: Index out of bounds\n");
        exit(1);
    }
    return (char *)list->data + (index * list->elementSize);
}

void freeArrayList(ArrayList *list)
{
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

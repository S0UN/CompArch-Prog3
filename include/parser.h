#ifndef PARSER_H
#define PARSER_H
#include "arraylist.h"


typedef struct Device {
    char type[10];    
    int uniqueID;       
    ArrayList inputs;  
    ArrayList outputs;
    ArrayList connections; 

} Device;

int parseInput(const char* filename, ArrayList *devices, int* deviceCount);
int extractInputs(const char *line, ArrayList *list);
void extractKey(const char *line, char *key);

#endif 

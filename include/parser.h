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

// Function declarations
int parseInput(const char *filename, ArrayList *devices, int *numInputs);
void extractInputs(const char *value, ArrayList *list); 
void extractKey(const char *line, char *key);
void extractKeyValue(const char *line, char *key, char *value); 


#endif 

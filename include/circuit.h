#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "arraylist.h" 
#include "parser.h"     
#define MAX_DEVICES 100

void makeGraph(ArrayList *devices);
void computeTruthTable(ArrayList *devices, int numInputs);
int evaluate(Device *device, int *inputs, ArrayList *devices);
int compareInt(const void *a, const void *b);

//void collectAndSortDeviceIDs(ArrayList *devices, const char *type, ArrayList *result);
//void printTruthTableHeader(ArrayList *inputDevices, ArrayList *outputDevices);
//void printTruthTableRow(ArrayList *inputDevices, ArrayList *outputDevices, int *inputs, ArrayList *devices);
//int evaluateDevice(Device *device, int *inputs, ArrayList *devices);
//void buildDeviceConnections(Device *device, ArrayList *devices);
#endif 

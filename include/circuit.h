#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "arraylist.h" 
#include "parser.h"     
#define MAX_DEVICES 10000  
void makeGraph(ArrayList *devices, int *deviceMap);
 void computeTruthTable(ArrayList *devices, int numInputs);
int evaluate(Device *device, int *inputs, ArrayList *devices);
int compareInt(const void *a, const void *b);
void printDeviceConnections(ArrayList *devices); 
void collectAndSortDeviceIDs(ArrayList *devices, const char *type, ArrayList *result);
void printTruthTableHeader(ArrayList *inputDevices, ArrayList *outputDevices);
void printTruthTableRow(ArrayList *inputDevices, ArrayList *outputDevices, int *inputs, ArrayList *devices, int *deviceMap);
int evaluateDevice(Device *device, int *inputs, ArrayList *devices);
void buildDeviceConnections(Device *device, ArrayList *devices, int *deviceMap);
Device *getDeviceByID(int uniqueID, ArrayList *devices, int *deviceMap);
#endif 

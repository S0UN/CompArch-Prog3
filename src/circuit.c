#include "arraylist.h"
#include "parser.h"
#include <stdio.h>  
#include <string.h>  
#include "circuit.h"

// Helper function to build connections for a single device
void buildDeviceConnections(Device *device, ArrayList *devices) {
    freeArrayList(&device->connections);
    device->connections = createArrayList(2, sizeof(Device *));

    for (int j = 0; j < device->inputs.size; j++) {
        int *inputID = (int *)getFromArrayList(&device->inputs, j);
        if (!inputID) continue;

        for (int k = 0; k < devices->size; k++) {
            Device *potentialInputDevice = (Device *)getFromArrayList(devices, k);
            if (!potentialInputDevice) continue;

            if (potentialInputDevice->uniqueID == *inputID) {
                addToArrayList(&device->connections, &potentialInputDevice);
                break;
            }
        }
    }
}

void makeGraph(ArrayList *devices) {
    for (int i = 0; i < devices->size; i++) {
        Device *device = (Device *)getFromArrayList(devices, i);
        if (!device) continue;

        buildDeviceConnections(device, devices);
    }
}

int *cache;

// Helper function to evaluate a device based on its type
int evaluateDevice(Device *device, int *inputs, ArrayList *devices) {
    if (strcmp(device->type, "INPUT") == 0) {
        return inputs[device->uniqueID];
    } else if (strcmp(device->type, "NOT") == 0) {
        if (device->connections.size != 1) {
            fprintf(stderr, "ERROR: NOT gate must have exactly one input.\n");
            exit(1);
        }
        Device *inputDevice = *(Device **)getFromArrayList(&device->connections, 0);
        return !evaluate(inputDevice, inputs, devices);
    } else if (strcmp(device->type, "AND") == 0) {
        int result = 1;
        for (int i = 0; i < device->connections.size; i++) {
            Device *inputDevice = *(Device **)getFromArrayList(&device->connections, i);
            result &= evaluate(inputDevice, inputs, devices);
            if (result == 0) break;
        }
        return result;
    } else if (strcmp(device->type, "OR") == 0) {
        int result = 0;
        for (int i = 0; i < device->connections.size; i++) {
            Device *inputDevice = *(Device **)getFromArrayList(&device->connections, i);
            result |= evaluate(inputDevice, inputs, devices);
            if (result == 1) break;
        }
        return result;
    } else if (strcmp(device->type, "XOR") == 0) {
        int result = 0;
        for (int i = 0; i < device->connections.size; i++) {
            Device *inputDevice = *(Device **)getFromArrayList(&device->connections, i);
            result ^= evaluate(inputDevice, inputs, devices);
        }
        return result;
    } else if (strcmp(device->type, "OUTPUT") == 0) {
        if (device->connections.size != 1) {
            fprintf(stderr, "ERROR: OUTPUT device must have exactly one input.\n");
            exit(1);
        }
        Device *inputDevice = *(Device **)getFromArrayList(&device->connections, 0);
        return evaluate(inputDevice, inputs, devices);
    } else {
        fprintf(stderr, "ERROR: Unknown device type: %s\n", device->type);
        exit(1);
    }
}

int evaluate(Device *device, int *inputs, ArrayList *devices) {
    if (!device) {
        fprintf(stderr, "ERROR: NULL device encountered in evaluate().\n");
        exit(1);
    }

    // Check cache
    if (cache[device->uniqueID] != -1) {
        return cache[device->uniqueID];
    }

    int result = evaluateDevice(device, inputs, devices);

    // Cache the result
    cache[device->uniqueID] = result;
    return result;
}

int compareInt(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Helper function to collect and sort device IDs by type
void collectAndSortDeviceIDs(ArrayList *devices, const char *type, ArrayList *result) {
    for (int i = 0; i < devices->size; i++) {
        Device *device = (Device *)getFromArrayList(devices, i);
        if (strcmp(device->type, type) == 0) {
            addToArrayList(result, &device->uniqueID);
        }
    }
    qsort(result->data, result->size, sizeof(int), compareInt);
}

// Helper function to print the truth table header
void printTruthTableHeader(ArrayList *inputDevices, ArrayList *outputDevices) {
    for (int j = 0; j < inputDevices->size; j++) {
        int *inputID = (int *)getFromArrayList(inputDevices, j);
        printf("%d ", *inputID);
    }
    printf("| ");
    for (int j = 0; j < outputDevices->size; j++) {
        int *outputID = (int *)getFromArrayList(outputDevices, j);
        printf("%d ", *outputID);
    }
    printf("\n");
}

// Helper function to print a row of the truth table
void printTruthTableRow(ArrayList *inputDevices, ArrayList *outputDevices, int *inputs, ArrayList *devices) {
    for (int j = 0; j < inputDevices->size; j++) {
        int *inputID = (int *)getFromArrayList(inputDevices, j);
        printf("%d ", inputs[*inputID]);
    }
    printf("| ");
    for (int j = 0; j < outputDevices->size; j++) {
        int *outputID = (int *)getFromArrayList(outputDevices, j);
        Device *outputDevice = (Device *)getFromArrayList(devices, *outputID);
        if (!outputDevice) {
            fprintf(stderr, "ERROR: Output device ID %d not found.\n", *outputID);
            exit(1);
        }
        int outputValue = evaluate(outputDevice, inputs, devices);
        printf("%d ", outputValue);
    }
    printf("\n");
}

void computeTruthTable(ArrayList *devices, int numInputs) {
    int numCombinations = 1 << numInputs;
    cache = malloc(sizeof(int) * devices->size);

    if (!cache) {
        fprintf(stderr, "Error: Memory allocation failed for cache.\n");
        exit(1);
    }

    // Collect and sort input and output device IDs
    ArrayList inputDevices = createArrayList(numInputs, sizeof(int));
    ArrayList outputDevices = createArrayList(devices->size, sizeof(int));

    collectAndSortDeviceIDs(devices, "INPUT", &inputDevices);
    collectAndSortDeviceIDs(devices, "OUTPUT", &outputDevices);

    printTruthTableHeader(&inputDevices, &outputDevices);

    // Generate and print truth table rows
    for (int i = 0; i < numCombinations; i++) {
        memset(cache, -1, sizeof(int) * devices->size);

        // Generate input values
        int inputs[devices->size];
        for (int j = 0; j < inputDevices.size; j++) {
            int *inputID = (int *)getFromArrayList(&inputDevices, j);
            inputs[*inputID] = (i >> j) & 1;
        }

        // Print the row
        printTruthTableRow(&inputDevices, &outputDevices, inputs, devices);
    }

    free(cache);
    freeArrayList(&inputDevices);
    freeArrayList(&outputDevices);
}
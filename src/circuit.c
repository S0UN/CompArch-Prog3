#include "arraylist.h"
#include "parser.h"
#include <stdio.h>  
#include <string.h>  
#include "circuit.h"

void makeGraph(ArrayList *devices) {
    for (int i = 0; i < devices->size; i++) {
        Device *device = (Device *)getFromArrayList(devices, i);
        if (!device) continue;

        // Clear existing connections
        freeArrayList(&device->connections);
        device->connections = createArrayList(2, sizeof(Device *));

        // Find devices that are inputs to this device
        for (int j = 0; j < device->inputs.size; j++) {
            int *inputID = (int *)getFromArrayList(&device->inputs, j);
            if (!inputID) continue;

            // Find the device with the matching uniqueID
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
}

int *cache;

int evaluate(Device *device, int *inputs, ArrayList *devices) {
    if (!device) {
        fprintf(stderr, "ERROR: NULL device encountered in evaluate().\n");
        exit(1);
    }

    // Check cache
    if (cache[device->uniqueID] != -1) {
        return cache[device->uniqueID];
    }

    int result = 0;

    if (strcmp(device->type, "INPUT") == 0) {
        // INPUT devices get their value directly from the inputs array
        result = inputs[device->uniqueID];
    } else if (strcmp(device->type, "NOT") == 0) {
        // NOT gate has exactly one input
        if (device->connections.size != 1) {
            fprintf(stderr, "ERROR: NOT gate must have exactly one input.\n");
            exit(1);
        }
        Device *inputDevice = *(Device **)getFromArrayList(&device->connections, 0);
        result = !evaluate(inputDevice, inputs, devices);
    } else if (strcmp(device->type, "AND") == 0) {
        // AND gate: result is 1 only if all inputs are 1
        result = 1;  // Start with 1 (AND identity)
        for (int i = 0; i < device->connections.size; i++) {
            Device *inputDevice = *(Device **)getFromArrayList(&device->connections, i);
            result &= evaluate(inputDevice, inputs, devices);
            if (result == 0) break;  // Early exit if any input is 0
        }
    } else if (strcmp(device->type, "OR") == 0) {
        // OR gate: result is 1 if at least one input is 1
        result = 0;  // Start with 0 (OR identity)
        for (int i = 0; i < device->connections.size; i++) {
            Device *inputDevice = *(Device **)getFromArrayList(&device->connections, i);
            result |= evaluate(inputDevice, inputs, devices);
            if (result == 1) break;  // Early exit if any input is 1
        }
    } else if (strcmp(device->type, "XOR") == 0) {
        // XOR gate: result is 1 if an odd number of inputs are 1
        result = 0;  // Start with 0 (XOR identity)
        for (int i = 0; i < device->connections.size; i++) {
            Device *inputDevice = *(Device **)getFromArrayList(&device->connections, i);
            result ^= evaluate(inputDevice, inputs, devices);
        }
    } else if (strcmp(device->type, "OUTPUT") == 0) {
        // OUTPUT devices simply pass through the value of their input
        if (device->connections.size != 1) {
            fprintf(stderr, "ERROR: OUTPUT device must have exactly one input.\n");
            exit(1);
        }
        Device *inputDevice = *(Device **)getFromArrayList(&device->connections, 0);
        result = evaluate(inputDevice, inputs, devices);
    } else {
        fprintf(stderr, "ERROR: Unknown device type: %s\n", device->type);
        exit(1);
    }

    // Cache the result
    cache[device->uniqueID] = result;
    return result;
}

int compareInt(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void computeTruthTable(ArrayList *devices, int numInputs) {
    int numCombinations = 1 << numInputs;
    cache = malloc(sizeof(int) * devices->size);

    if (!cache) {
        fprintf(stderr, "Error: Memory allocation failed for cache.\n");
        exit(1);
    }

    // Identify and sort input and output devices
    ArrayList inputDevices = createArrayList(numInputs, sizeof(int));
    ArrayList outputDevices = createArrayList(devices->size, sizeof(int));

    for (int i = 0; i < devices->size; i++) {
        Device *device = (Device *)getFromArrayList(devices, i);
        if (strcmp(device->type, "INPUT") == 0) {
            addToArrayList(&inputDevices, &device->uniqueID);
        } else if (strcmp(device->type, "OUTPUT") == 0) {
            addToArrayList(&outputDevices, &device->uniqueID);
        }
    }

    qsort(inputDevices.data, inputDevices.size, sizeof(int), compareInt);
    qsort(outputDevices.data, outputDevices.size, sizeof(int), compareInt);

    // Print header row
    for (int j = 0; j < inputDevices.size; j++) {
        int *inputID = (int *)getFromArrayList(&inputDevices, j);
        printf("%d ", *inputID);
    }
    printf("| ");
    for (int j = 0; j < outputDevices.size; j++) {
        int *outputID = (int *)getFromArrayList(&outputDevices, j);
        printf("%d ", *outputID);
    }
    printf("\n");

    for (int i = 0; i < numCombinations; i++) {
        memset(cache, -1, sizeof(int) * devices->size);

        // Generate input values
        int inputs[devices->size];  // Use devices->size to handle arbitrary input IDs
        for (int j = 0; j < inputDevices.size; j++) {
            int *inputID = (int *)getFromArrayList(&inputDevices, j);
            inputs[*inputID] = (i >> j) & 1;  // Extract j-th bit
        }

        // Print sorted input values
        for (int j = 0; j < inputDevices.size; j++) {
            int *inputID = (int *)getFromArrayList(&inputDevices, j);
            printf("%d ", inputs[*inputID]);
        }

        printf("| ");

        // Print sorted output values
        for (int j = 0; j < outputDevices.size; j++) {
            int *outputID = (int *)getFromArrayList(&outputDevices, j);
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

    free(cache);
    freeArrayList(&inputDevices);
    freeArrayList(&outputDevices);
}
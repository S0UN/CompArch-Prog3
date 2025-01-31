#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/circuit.h"
#include "../include/arraylist.h"

// Function to print device details
void printDevice(Device *device) {
    printf("Device: %s (ID: %d)\n", device->type, device->uniqueID);

    printf("Inputs: ");
    for (int i = 0; i < device->inputs.size; i++) {
        int *input = (int *)getFromArrayList(&device->inputs, i);
        if (input) {
            printf("%d ", *input);
        }
    }
    printf("\n");

    printf("Outputs: ");
    for (int i = 0; i < device->outputs.size; i++) {
        int *output = (int *)getFromArrayList(&device->outputs, i);
        if (output) {
            printf("%d ", *output);
        }
    }
    printf("\n");
}

// Function to free memory for individual devices
void freeDevice(Device *device) {
    freeArrayList(&device->inputs);
    freeArrayList(&device->outputs);
    freeArrayList(&device->connections);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    ArrayList devices = createArrayList(10, sizeof(Device));
    int numInputs = 0;  // Track number of INPUT devices

    // Parse the circuit and track `numInputs`
    if (parseInput(inputFile, &devices, &numInputs) == -1) {
        fprintf(stderr, "Error parsing input file: %s\n", inputFile);
        return 1;
    }



    makeGraph(&devices);

    computeTruthTable(&devices, numInputs);

    // Free memory for individual devices
    for (int i = 0; i < devices.size; i++) {
        Device *device = (Device *)getFromArrayList(&devices, i);
        if (device) {
            freeDevice(device);
        }
    }

    freeArrayList(&devices);

    return 0;
}
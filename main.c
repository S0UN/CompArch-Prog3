#include <stdio.h>
#include "../include/parser.h"
#include "../include/arraylist.h"

void printDevice(Device *device)
{
    printf("Device: %s (ID: %d)\n", device->type, device->uniqueID);

    printf("Inputs: ");
    for (int i = 0; i < device->inputs.size; i++)
    {
        int *input = (int *)getFromArrayList(&device->inputs, i);
        if (input) {
            printf("%d ", *input);
        }
    }
    printf("\n");

    printf("Outputs: ");
    for (int i = 0; i < device->outputs.size; i++)
    {
        int *output = (int *)getFromArrayList(&device->outputs, i);
        if (output) {
            printf("%d ", *output);
        }
    }
    printf("\n");
}

int main()
{
    ArrayList devices = createArrayList(10, sizeof(Device)); // ✅ Create an ArrayList for devices
    int deviceCount = 0;

    // ✅ Pass deviceCount as the third argument
    if (parseInput("test_input.txt", &devices, &deviceCount) == -1)
    {
        fprintf(stderr, "Error parsing input file.\n");
        freeArrayList(&devices);
        return 1;
    }

    // Print the parsed devices
    printf("Parsed Devices:\n");
    for (int i = 0; i < deviceCount; i++)
    {
        Device *device = (Device *)getFromArrayList(&devices, i);
        if (device) {
            printDevice(device);
        }
    }

    // Free all allocated memory
    for (int i = 0; i < deviceCount; i++)
    {
        Device *device = (Device *)getFromArrayList(&devices, i);
        if (device) {
            freeArrayList(&device->inputs);
            freeArrayList(&device->outputs);
        }
    }
    freeArrayList(&devices);

    return 0;
}

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void countInputs(Device *device, int *numInputs) {
    if (strcmp(device->type, "INPUT") == 0) {
        (*numInputs)++;
    }
}

int parseInput(const char *filename, ArrayList *devices, int *numInputs) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't read file %s\n", filename);
        return -1;
    }

    char line[256];
    *numInputs = 0;
    Device device;
    int inStanza = 0; 

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "{")) {
            inStanza = 1;
            memset(&device, 0, sizeof(Device));
            device.inputs = createArrayList(2, sizeof(int));
            device.outputs = createArrayList(2, sizeof(int));
            device.connections = createArrayList(2, sizeof(Device *));
        } else if (strstr(line, "}")) {
            if (inStanza) {
                countInputs(&device, numInputs);
                addToArrayList(devices, &device);
                inStanza = 0;
            }
        } else if (inStanza) {
            char key[256], value[256];
            extractKeyValue(line, key, value);

            if (strcmp(key, "Type") == 0) {
                strncpy(device.type, value, sizeof(device.type) - 1);
                device.type[sizeof(device.type) - 1] = '\0';
            } else if (strcmp(key, "UniqueID") == 0) {
                device.uniqueID = atoi(value);
            } else if (strcmp(key, "Input") == 0) {
                extractInputs(value, &device.inputs);
            } else if (strcmp(key, "Output") == 0) {
                extractInputs(value, &device.outputs);
            } else {
                fprintf(stderr, "Unknown key: %s\n", key);
            }
        }
    }

    fclose(file);
    return 0;
}

void extractKeyValue(const char *line, char *key, char *value) {
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    char *equalSign = strchr(line, '=');
    if (equalSign) {
        size_t keyLength = equalSign - line;
        strncpy(key, line, keyLength);
        key[keyLength] = '\0';
        
        char *valStart = equalSign + 1;
        while (*valStart == ' ') {
            valStart++;
        }
        strncpy(value, valStart, 255);
        value[255] = '\0';
        
        size_t len = strlen(value);
        if (len > 0 && value[len - 1] == '\n') {
            value[len - 1] = '\0';
        }
    } else {
        key[0] = '\0';
        value[0] = '\0';
    }
}

void extractInputs(const char *value, ArrayList *list) {
    char *token = strtok((char *)value, ",");
    while (token) {
        while (*token == ' ') {
            token++;
        }
        if (*token != '\0') {
            int num = atoi(token);
            addToArrayList(list, &num);
        }
        token = strtok(NULL, ",");
    }
}
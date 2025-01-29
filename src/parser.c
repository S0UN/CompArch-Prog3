#include "parser.h"
#include <stdio.h>
#include <string.h>

int parseInput(const char *filename, ArrayList *devices, int *deviceCount)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Couldn't read file %s\n", filename);
        return -1;
    }

    char line[256];

    *deviceCount = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '\0')
        {
            continue;
        }

        if (strstr(line, "{"))
        {
            struct Device device;

            device.inputs = createArrayList(5, sizeof(int));
            device.outputs = createArrayList(5, sizeof(int));

            while (fgets(line, sizeof(line), file) && !strstr(line, "}"))
            {
                char key[256];
                extractKey(line, key);

                if (strcmp(key, "Type") == 0)
                {
                    char *value = strstr(line, "Type=");
                    if (value)
                    {
                        value += 5; 
                        while (*value == ' ')
                            value++; 
                        strncpy(device.type, value, sizeof(device.type) - 1);
                        device.type[sizeof(device.type) - 1] = '\0';     
                        printf("DEBUG: Read Type = '%s'\n", device.type); 
                    }
                    else
                    {
                        fprintf(stderr, "Error reading Type\n");
                    }
                }

                else if (strcmp(key, "UniqueID") == 0)
                {
                    char *value = strstr(line, "UniqueID=");
                    if (value)
                    {
                        value += 9;
                        while (*value == ' ')
                            value++; 
                        device.uniqueID = atoi(value);
                        printf("DEBUG: Read UniqueID = %d\n", device.uniqueID); 
                    }
                    else
                    {
                        fprintf(stderr, "Error reading UniqueID\n");
                    }
                }

                else if (strcmp(key, "Input") == 0)
                {
                    extractInputs(line, &device.inputs);
                }
                else if (strcmp(key, "Output") == 0)
                {
                    extractInputs(line, &device.outputs);
                }
                else
                {
                    fprintf(stderr, "Unknown key: %s\n", key);
                }
            }

            addToArrayList(devices, &device);
            (*deviceCount)++;
        }
    }

    fclose(file);
    return 0;
}

int extractInputs(const char *line, ArrayList *list)
{
    char lineCopy[256];
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';

    char *equalSign = strchr(lineCopy, '=');
    if (!equalSign)
    {
        return 0;
    }

    int count = 0;
    char *token = strtok(equalSign + 1, ",");
    while (token)
    {
        int value = atoi(token);
        addToArrayList(list, &value);
        count++;
        token = strtok(NULL, ",");
    }

    return count;
}

void extractKey(const char *line, char *key)
{
    while (*line == ' ' || *line == '\t')
    {
        line++; // Skip leading spaces/tabs
    }

    char *equalSign = strchr(line, '=');
    if (equalSign)
    {
        size_t keyLength = equalSign - line;
        strncpy(key, line, keyLength);
        key[keyLength] = '\0';
    }
    else
    {
        key[0] = '\0';
    }
}

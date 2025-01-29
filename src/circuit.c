#include <arraylist.h>
#include <parser.h>

void makeGraph(ArrayList *devices)
{
    for (int i = 0; i < devices->size; i++)
    {
        for (int j = i; j < devices->size; j++)
        {
            Device *device = (Device *)getFromArrayList(&devices, i);
            if (devices) {
                // add your code here
            }
        }
    }
}

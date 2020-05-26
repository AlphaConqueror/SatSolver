#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LogischeAusdrücke.h"

int intArrayContains(int* arr, int value, int arrayEnd) {
    for (int i = 0; i < arrayEnd; i++) {
        if (arr[i] == value)
            return 1;
    }

    return 0;
}

int* getVariables(e3CNF e, int* sizeOfArray) {
    int* array = malloc(2 * e.m * sizeof(int));

    for(int i = 0; i < e.m; i++) {
        char* line = (e.lines)[i];

        char *token = strtok(line, " ");

        while(token != NULL && strcmp(token, "0") != 0) {
            int v = atoi(token);

            if(intArrayContains(array, v, *sizeOfArray) == 0) {
                array[*sizeOfArray] = v;
                (*sizeOfArray)++;
            }

            token = strtok(NULL, " ");
        }
    }

    return array;
}

int* getPureLiterals(e3CNF e, int* sizeOfArray) {
    int* array = malloc(e.m * sizeof(int)),
         cVar = 0;
    int* var = getVariables(e, &cVar);

    for(int i = 0; i < cVar; i++) {
        int current = var[i];

        if(intArrayContains(var, -1 * current, cVar) == 0) {
            if (intArrayContains(array, current, *sizeOfArray) == 0) {
                array[*sizeOfArray] = current;
                (*sizeOfArray)++;
            }
        }
    }

    free(var);

    return array;
}




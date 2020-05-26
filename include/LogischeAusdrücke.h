#include <stdio.h>
#include <string.h>

typedef struct {
    int n,  //obere Schranke für die Anzahl der Variablen
        m;  //exakte Anzahl an Klauseln
    char** lines;
} e3CNF;

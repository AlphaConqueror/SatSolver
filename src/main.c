#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LogischeAusdrücke.h"
#include "PureLiterals.h"

int main(int argc, char *argv[]) {
    // todo: implement a SatSolver
    fprintf(stderr, "1 - %s\n", argv[1]);

    FILE* fp = fopen(argv[1], "r");
    int n, m;
    
    fscanf(fp, "%*s %*s %d %d\n", &n, &m);

    fprintf(stderr, "2 - %d %d\n", n, m);
    
    e3CNF inputCNF = {n, m, malloc(m * sizeof(char*))};
    char lines[100][m];
    
    for(int i = 0; i < m; i++) {
        fscanf(fp, "%[^\n]%*c", lines[i]);
        fprintf(stderr, "LINE = %s\n", lines[i]);
        (inputCNF.lines)[i] = lines[i];
    }

    fprintf(stderr, "3 - %s\n", (inputCNF.lines)[0]);
    
    fclose(fp);

    fprintf(stderr, "4\n");

    if(argc == 4 && strcmp(argv[3], "-p") == 0) {
        fprintf(stderr, "5\n");

        int c = 0;
        int* v = getPureLiterals(inputCNF, &c);

        fprintf(stderr, "6 - %s\n", argv[2]);

        FILE* f = fopen(argv[2], "w");

        for(int i = 0; i < c; i++) {
            fprintf(f, "%d ", v[i]);
        }

        fprintf(f, "0\n");
        fclose(f);
        free(v);
    }

    free(inputCNF.lines);

    return 0;
}

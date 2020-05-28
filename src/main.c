#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Clause.h"
#include "Literal.h"
#include "IOManager.h"

int main(int argc, char *argv[]) {
    // todo: implement a SatSolver
    int n, m;
    clause_t* input = readClauseInput(argv[1], &n, &m);

    if(argc == 4 && strcmp(argv[3], "-p") == 0) {
        literal_t* literals = getPureLiterals(input);

        printLiteral(literals, argv[2]);
        freeLiteral(literals);
    }

    printClause(input);

    freeClause(input);

    return 0;
}

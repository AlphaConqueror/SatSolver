#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Clause.h"
#include "Literal.h"
#include "Formula.h"
#include "IOManager.h"
#include "SatSolver.h"

int main(int argc, char *argv[]) {
    clause_t* input = readClauseInput(argv[1], &n, &m);

    if(argc == 4) {
        if (strcmp(argv[3], "-p") == 0) {
            literal_t *literals = getPureLiterals(input);

            printLiteral(literals, argv[2]);
            freeLiteral(literals);
        } else if (strcmp(argv[3], "-b") == 0) {
            formula_t *formula = createFormula(input);
            int isSolved = solveSat(formula, argv[2], 1);

            if (isSolved == UNSAT)
                printFailure(argv[2]);

            freeClause(input);

            return isSolved;
        }
    } else if(argc == 3) {
        formula_t* formula = createFormula(input);
        int isSolved = solveSat(formula, argv[2], 0);

        if(isSolved == UNSAT)
            printFailure(argv[2]);

        freeClause(input);

        return isSolved;
    }

    freeClause(input);

    return 0;
}

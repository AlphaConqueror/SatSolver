#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Clause.h"
#include "Literal.h"
#include "Formula.h"
#include "IOManager.h"
#include "SatSolver.h"

int solveSat(formula_t* formula, char* path) {
    int isSolved = checkClause(formula->clause, formula->values);

    if(isSolved) {
        printSolution(formula->values, path);
        return SAT;
    }

    int undefinedIndex = isUndefined(formula->clause, formula->values);

    if(isSolved == FALSE) {
        return UNSAT;
    } else if(undefinedIndex != 0) {
        int unitClauseIndex = isUnitClause(formula->clause, formula->values);

        if (unitClauseIndex != 0) {
            formula_t* newFormula = createFormula(formula->clause);

            cloneValues(newFormula->values, formula->values);
            newFormula->values[abs(unitClauseIndex) - 1] = unitClauseIndex < 0 ? FALSE : TRUE;
            freeFormula(newFormula);

            return solveSat(newFormula, path);
        } else {
            formula_t* newFormulaPos = createFormula(formula->clause),
                       *newFormulaNeg = createFormula(formula->clause);

            cloneValues(newFormulaPos->values, formula->values);
            newFormulaPos->values[undefinedIndex - 1] = TRUE;

            cloneValues(newFormulaNeg->values, formula->values);
            newFormulaNeg->values[undefinedIndex - 1] = FALSE;

            return solveSat(newFormulaPos, path) == SAT || solveSat(newFormulaNeg, path) == SAT;
        }
    }

    return UNSAT;
}

/*
 * int solveSat(formula_t* formula, char* path) {
    int isSolved = checkClause(formula->clause, formula->values);

    if(isSolved) {
        printSolution(formula->values, path);
        return SAT;
    }

    int undefinedIndex = isUndefined(formula->clause, formula->values);

    if(isSolved == FALSE) {
        if(formula->previous != NULL) {
            return solveSat(formula->previous, path);
        } else
            return UNSAT;
    } else if(undefinedIndex != 0) {
        formula_t *newFormula = createFormula(formula, formula->clause);
        int unitClauseIndex = isUnitClause(formula->clause, formula->values);

        cloneValues(newFormula->values, formula->values);

        if (unitClauseIndex != 0) {
            newFormula->values[abs(unitClauseIndex) - 1] = unitClauseIndex < 0 ? FALSE : TRUE;
        } else {
            for (int i = 0; i < n; i++) {
                if ((formula->values)[i] == -1) {
                    newFormula->values[i] = 1;
                    break;
                }
            }
        }

        return solveSat(newFormula, path);
    }

    return UNSAT;
}
 */
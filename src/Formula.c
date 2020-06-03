#include <stdlib.h>
#include "Literal.h"
#include "Clause.h"
#include "Formula.h"
#include "SatSolver.h"

formula_t* createFormula(clause_t* clause) {
    formula_t* formula = malloc(sizeof(formula_t));

    formula->next = NULL;
    formula->clause = clause;
    formula->values = malloc(n * sizeof(int));

    for(int i = 0; i < n; i++)
        formula->values[i] = UNDEFINED;

    return formula;
}

formula_t* getLastFormula(formula_t* root) {
    formula_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->next == NULL)
            return iterator;

        iterator = iterator->next;
    }

    return NULL;
}

void freeFormula(formula_t* formula) {
    if(formula != NULL) {
        if(formula->next != NULL)
            freeFormula(formula->next);
        if(formula->values != NULL)
            free(formula->values);
        free(formula);
    }
}
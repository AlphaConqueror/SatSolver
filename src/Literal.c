#include <stdio.h>
#include <stdlib.h>
#include "Clause.h"
#include "Literal.h"
#include "SatSolver.h"
#include "IOManager.h"

literal_t* createLiteral() {
    literal_t* literal = malloc(sizeof(literal_t));

    literal->next = NULL;
    literal->index = 0;

    return literal;
}

literal_t* getLastLiteral(literal_t* root) {
    literal_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->next == NULL)
            return iterator;

        iterator = iterator->next;
    }

    return NULL;
}

void addLiteralNext(literal_t* root, literal_t* next) {
    literal_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->next == NULL) {
            iterator->next = next;
            return;
        }

        iterator = iterator->next;
    }
}

int containsIndex(literal_t* root, int index) {
    literal_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->index == index)
            return 1;

        iterator = iterator->next;
    }

    return 0;
}

void freeLiteral(literal_t* literal) {
    if(literal != NULL) {
        if(literal->next != NULL)
            freeLiteral(literal->next);
        free(literal);
    }
}

int getValue(literal_t* literal, int* values) {
    int value = values[abs(literal->index) - 1];

    if(value == UNDEFINED)
        return UNDEFINED;

    return literal->index < 0 ? !value : value;
}

int checkLiteral(literal_t* root, int* values) {
    literal_t* iterator = root;

    while(iterator != NULL) {
        int value = getValue(iterator, values);

        if(value == TRUE)
            return TRUE;

        iterator = iterator->next;
    }

    return FALSE;
}

literal_t* getLiterals(clause_t* root) {
    literal_t* literal = createLiteral();
    clause_t* clauseIterator = root;

    while(clauseIterator != NULL) {
        literal_t* literalIterator = clauseIterator->head;

        while(literalIterator != NULL) {
            if(containsIndex(literal, literalIterator->index) == 0) {
                if(literal->index == 0)
                    literal->index = literalIterator->index;
                else {
                    literal_t* next = createLiteral();

                    next->index = literalIterator->index;
                    addLiteralNext(literal, next);
                }
            }

            literalIterator = literalIterator->next;
        }

        clauseIterator = clauseIterator->next;
    }

    return literal;
}

literal_t* getPureLiterals(clause_t* root) {
    literal_t* pureLiterals = createLiteral(),
               *literals = getLiterals(root),
               *iterator = literals;

    while(iterator != NULL) {
        if(containsIndex(literals, -1 * iterator->index) == 0 && containsIndex(pureLiterals, iterator->index) == 0) {
            if(pureLiterals->index == 0)
                pureLiterals->index = iterator->index;
            else {
                literal_t* next = createLiteral();

                next->index = iterator->index;
                addLiteralNext(pureLiterals, next);
            }
        }

        iterator = iterator->next;
    }

    freeLiteral(literals);

    return pureLiterals;
}

void cloneValues(int* values, int* initValues) {
    for(int i = 0; i < n; i++) {
        values[i] = initValues[i];
    }
}


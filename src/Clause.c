#include <stdio.h>
#include <stdlib.h>
#include "Clause.h"
#include "Literal.h"

clause_t* createClause() {
    clause_t* clause = malloc(sizeof(clause_t));

    clause->head = NULL;
    clause->next= NULL;

    return clause;
}

clause_t* getLastClause(clause_t* root) {
    clause_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->next == NULL)
            return iterator;

        iterator = iterator->next;
    }

    return NULL;
}

void addClauseNext(clause_t* root, clause_t* next) {
    clause_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->next == NULL) {
            iterator->next = next;
            return;
        }

        iterator = iterator->next;
    }
}

void freeClause(clause_t* clause) {
    if(clause != NULL) {
        if(clause->head != NULL)
            freeLiteral(clause->head);
        if(clause->next != NULL)
            freeClause(clause->next);
        free(clause);
    }
}

//return literal index if undefined
int isUndefined(clause_t* clause, int* values) {
    literal_t* iterator = clause->head;
    int bool = FALSE;
    int undefinedIndex = 0;

    while(iterator != NULL) {
        int value = getValue(iterator, values);

        if(value == TRUE)
            return FALSE;
        else if(value == UNDEFINED)
            undefinedIndex = iterator->index;

        iterator = iterator->next;
    }

    return (bool == FALSE && undefinedIndex != 0) ? undefinedIndex : FALSE;
}

//returns literal index if true, 0 if false
int isUnitClause(clause_t* root, int* values) {
    clause_t* clauseIterator = root;
    int index = 0;

    while(clauseIterator != NULL) {
        if (clauseIterator != NULL && clauseIterator->head != NULL) {
            literal_t* literalIterator = clauseIterator->head;

            while (literalIterator != NULL) {
                int value = getValue(literalIterator->index, values);

                if (value == TRUE)
                    return 0;
                if (value == UNDEFINED) {
                    if (index == 0 || index == literalIterator->index)
                        index = literalIterator->index;
                    else
                        return 0;
                }

                literalIterator = literalIterator->next;
            }
        }

        if(index != 0)
            return index;

        clauseIterator = clauseIterator->next;
    }

    return index;
}

int checkClause(clause_t* root, int* values) {
    clause_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->head == NULL)
            return UNDEFINED;

        if(checkLiteral(iterator->head, values) == FALSE)
            return FALSE;

        iterator = iterator->next;
    }

    return TRUE;
}








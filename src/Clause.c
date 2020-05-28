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

int isEmptyClause(clause_t* root) {
    clause_t* iterator = root;

    while(iterator != NULL) {
        if(iterator->head == NULL)
            return 1;
        iterator = iterator->next;
    }

    return 0;
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
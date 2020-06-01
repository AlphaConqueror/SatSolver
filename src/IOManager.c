#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Clause.h"
#include "Literal.h"
#include "Formula.h"
#include "SatSolver.h"

clause_t* readClauseInput(char* path, int* n, int* m) {
    FILE* file;
    char line[100];
    char* token;
    clause_t* input = createClause();

    file = fopen(path, "r");
    fscanf(file, "p cnf %d %d\n", n, m);

    for(int i = 0; i < *m; i++) {
        clause_t* clause = getLastClause(input);
        literal_t* literal = createLiteral();

        fscanf(file, "%[^\n]%*c", line);
        token = strtok(line, " ");

        while(token != NULL && strcmp(token, "0") != 0) {
            literal_t* currentLiteral = getLastLiteral(literal);
            int index = atoi(token);

            if(currentLiteral->index == 0)
                currentLiteral->index = index;
            else {
                literal_t* newLiteral = createLiteral();

                newLiteral->index = index;
                addLiteralNext(currentLiteral, newLiteral);
            }

            token = strtok(NULL, " ");
        }

        if(clause->head == NULL)
            clause->head = literal;
        else {
            clause_t* newClause = createClause();

            newClause->head = literal;
            addClauseNext(input, newClause);
        }
    }

    fclose(file);

    return input;
}

void printClause(clause_t* root) {
    clause_t* clauseIterator = root;

    while(clauseIterator != NULL) {
        literal_t* literalIterator = clauseIterator->head;

        while(literalIterator != NULL) {
            printf("%d ", literalIterator->index);

            literalIterator = literalIterator->next;
        }

        printf("0\n");
        clauseIterator = clauseIterator->next;
    }
}

void printLiteral(literal_t* root, char* path) {
    literal_t* iterator = root;
    FILE* file = fopen(path, "w");

    while(iterator != NULL) {
        fprintf(stderr, "%d ", iterator->index);
        fprintf(file, "%d ", iterator->index);

        iterator = iterator->next;
    }

    fprintf(stderr, "0\n");
    fprintf(file, "0\n");
    fclose(file);
}

void printSolution(int* values, char* path) {
    FILE* file = fopen(path, "w");
    printf("Solution: \n");

    for(int i = 0; i < n; i++) {
        printf("%s%d ", values[i] == FALSE ? "-" : "", i+1);
        fprintf(file, "%s%d ", values[i] == FALSE ? "-" : "", i+1);
    }

    printf("\n");
    fprintf(file, "\n");
}
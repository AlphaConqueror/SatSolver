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

/*void printClause(clause_t* root) {
    clause_t* clauseIterator = root;

    while(clauseIterator != NULL) {
        literal_t* literalIterator = clauseIterator->head;

        printf("(");

        while(literalIterator != NULL) {
            if(literalIterator->next != NULL)
                printf("%d |", literalIterator->index);
            else
                printf("%d", literalIterator->index);

            literalIterator = literalIterator->next;
        }

        if(clauseIterator->next != NULL)
            printf(") & ");
        else
            printf(")");

        clauseIterator = clauseIterator->next;
    }

    printf("\n");
}*/

void printLiteral(literal_t* root, char* path) {
    literal_t* iterator = root;
    int* array = malloc(n * sizeof(int));
    FILE* file = fopen(path, "w");

    for(int i = 0; i < n; i++)
        array[i] = 0;

    while(iterator != NULL) {
        array[abs(iterator->index) - 1] = iterator->index;

        iterator = iterator->next;
    }

    for(int i = 0; i < n; i++)
        if(array[i] != 0)
            fprintf(file, "%d ", array[i]);

    fprintf(file, "0\n");
    free(array);
    fclose(file);
}

/*void printValues(int* values) {
    printf("Values: ");

    for(int i = 0; i < n; i++)
        printf("%d ", values[i]);

    printf("\n");
}*/

void printSolution(int* values, char* path) {
    FILE* file = fopen(path, "w");

    fprintf(file, "SAT\n");

    for(int i = 0; i < n; i++) {
        fprintf(file, "%s%d", values[i] == FALSE ? "-" : "", i+1);

        if(i < n - 1)
            fprintf(file, " ");
    }

    fprintf(file, "\n");
    fclose(file);
}

void printFailure(char* path) {
    FILE* file = fopen(path, "w");

    fprintf(file, "UNSAT\n");
    fclose(file);
}
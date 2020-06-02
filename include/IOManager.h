clause_t* readClauseInput(char* path, int* n, int* m);
void printClause(clause_t* root);
void printLiteral(literal_t* root, char* path);
void printValues(int* values);
void printSolution(int* values, char* path);
void printFailure(char* path);
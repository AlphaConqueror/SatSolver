typedef struct clause_t clause_t;

struct clause_t {
    struct literal_t* head;
    clause_t* next;
};

clause_t* createClause();
clause_t* getLastClause(clause_t* root);
void addClauseNext(clause_t* root, clause_t* next);
int containsEmptyClause(clause_t* root);
clause_t* cloneClause(clause_t* clause);
void freeClause(clause_t* clause);

int isUndefined(clause_t* clause, int* values);
int isUnitClause(clause_t* root, int* values);
int checkClause(clause_t* root, int* values);
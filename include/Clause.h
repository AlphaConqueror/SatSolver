typedef struct clause_t clause_t;

struct clause_t {
    struct literal_t* head;
    clause_t* next;
};

clause_t* createClause();
clause_t* getLastClause(clause_t* root);
void addClauseNext(clause_t* root, clause_t* next);
int isEmptyClause(clause_t* root);
void freeClause(clause_t* clause);
typedef struct formula_t formula_t;

struct formula_t {
    formula_t* next;
    struct clause_t* clause;
    int* values;
};

formula_t* createFormula(struct clause_t* clause);
int getFormulaSize(formula_t* root);
formula_t* getLastFormula(formula_t* root);
void freeFormula(formula_t* formula);


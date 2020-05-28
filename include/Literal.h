typedef struct literal_t literal_t;

struct literal_t {
    literal_t* next;
    int index;
};

literal_t* createLiteral();
literal_t* getLastLiteral(literal_t* root);
void addLiteralNext(literal_t* root, literal_t* next);
int containsIndex(literal_t* root, int index);
void freeLiteral(literal_t* literal);

literal_t* getLiterals(clause_t* root);
literal_t* getPureLiterals(clause_t* root);
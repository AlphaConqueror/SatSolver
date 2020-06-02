#define TRUE 1
#define FALSE 0
#define UNDEFINED -1

typedef struct literal_t literal_t;

struct literal_t {
    literal_t* next;
    int index;
};

literal_t* createLiteral();
literal_t* getLastLiteral(literal_t* root);
void addLiteralNext(literal_t* root, literal_t* next);
int containsIndex(literal_t* root, int index);
literal_t* cloneLiteral(literal_t* literal);
void freeLiteral(literal_t* literal);

int getValue(literal_t* literal, int* values);
int checkLiteral(literal_t* root, int* values);
literal_t* getLiterals(clause_t* root);
literal_t* getPureLiterals(clause_t* root);
void cloneValues(int* values, int* initValues);
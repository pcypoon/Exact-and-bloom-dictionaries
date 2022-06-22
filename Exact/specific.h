#include "../dict.h"

typedef struct item{
    char *word;
    struct item *next;
} item;

struct dict{
    item *table;
    int sz;
};

unsigned int hash_string(dict* x, const char *s);
void test();

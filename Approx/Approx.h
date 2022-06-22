#include "../dict.h"
#define KHASHES 11

struct dict{
    bool * table;
    int sz;
};

unsigned int hash_string(dict *x, const char *s);
unsigned long* _hashes(dict *x, const char* s);
void test();

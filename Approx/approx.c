#include "Approx.h"

dict *dict_init(unsigned int maxwords)
{
    // initialize an empty bool table
    dict *initframe = (dict *)ncalloc(1, sizeof(dict));
    initframe->table = ncalloc(maxwords * 20, sizeof(bool));
    initframe->sz = maxwords * 20;
    test();
    return initframe;
}

bool dict_add(dict *x, const char *s)
{
    // if x is not a possible case, i.e. NULL
    if (!x)
    {
        return false;
    }
    // get the hash number from master hash
    unsigned long *index = _hashes(x, s);
    for (int i = 0; i < KHASHES; i++)
    {
        x->table[index[i]] = true;
    }
    free(index);
    return true;
}

bool dict_spelling(dict *x, const char *s)
{
    // if x is not a possible case, i.e. NULL
    if (!x)
    {
        return false;
    }

    unsigned long *index = _hashes(x, s);

    for (int i = 0; i < KHASHES; i++)
    { // if inside is empty then its
        if (x->table[index[i]] == false)
        {
            free(index);
            return false;
        }
    }
    free(index);
    return true;
}

// Hash function that is big enough
unsigned int hash_string(dict *x, const char *s)
{
    int len = strlen(s);
    unsigned int hash = 0;
    for (int i = 0; i < len; i++)
    {
        hash += s[i];
        hash = (hash * s[i]);
    }
    int re = hash % x->sz;
    return re;
}

// Master hash which make 11 of hash_string
unsigned long *_hashes(dict *x, const char *s)
{
    unsigned long *hashes = ncalloc(KHASHES, sizeof(unsigned long));
    // Bernstein hash
    unsigned long bh = hash_string(x, s);
    int ln = strlen(s);
    /* If two different strings have the same bh, then
    we need a separate way to distiguish them when using
    bh to generate a sequence */
    int ln_1 = ln - 1;
    srand(bh * (ln * s[0] + s[ln_1]));
    unsigned long h2 = bh;
    for (int i = 0; i < KHASHES; i++)
    {
        h2 = 33 * h2 ^ rand();
        h2 = h2 % (x->sz);
        hashes[i] = h2;
    }
    return hashes;
}

void dict_free(dict *x)
{
    free(x->table);
    free(x);
}

void test()
{
    unsigned int maxwords = 20;
    dict *x = (dict *)ncalloc(1, sizeof(dict));
    x->table = ncalloc(maxwords * 20, sizeof(bool));
    x->sz = maxwords * 20;

    assert(!dict_add(NULL, "oldnotebook"));
    assert(!dict_add(NULL, " "));
    assert(dict_add(x, "oldnotebook"));
    assert(dict_add(x, "oldnotebook"));

    assert(!dict_add(NULL, "cookieisgood"));
    assert(!dict_add(NULL, " "));
    assert(dict_add(x, "cookieisgood"));
    assert(dict_add(x, "cookieisgood"));

    assert(!dict_add(NULL, "sold"));
    assert(!dict_add(NULL, " "));
    assert(dict_add(x, "sold"));
    assert(dict_add(x, "sold"));

    assert(!dict_spelling(NULL, "oldnotebook"));
    assert(dict_spelling(x, "oldnotebook"));
    assert(!dict_spelling(x, "Oldnotebook"));

    assert(!dict_spelling(NULL, "cookieisgood"));
    assert(dict_spelling(x, "cookieisgood"));
    assert(!dict_spelling(x, "Cookieisgood"));

    assert(!dict_spelling(NULL, "sold"));
    assert(dict_spelling(x, "sold"));
    assert(!dict_spelling(x, "sOld"));

    assert(!dict_spelling(x, "yoshi"));
    assert(!dict_spelling(x, "tyson"));

    assert(hash_string(x, "oldnotebook") < (maxwords * 20));
    assert(hash_string(x, "cookieisgood") < (maxwords * 20));
    assert(hash_string(x, "sold") < (maxwords * 20));

    assert(hash_string(x, "oldnotebook") == 19);
    assert(hash_string(x, "cookieisgood") == 156);
    assert(hash_string(x, "sold") == 112);

    unsigned long *hash = _hashes(x, "oldnotebook");
    assert(hash[1] < (maxwords * 20));

    unsigned long *hash_1 = _hashes(x, "cookieisgood");
    assert(hash_1[1] < (maxwords * 20));

    unsigned long *hash_2 = _hashes(x, "sold");
    assert(hash_2[1] < (maxwords * 20));
}

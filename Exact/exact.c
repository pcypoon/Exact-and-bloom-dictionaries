#include "specific.h"

dict *dict_init(unsigned int maxwords)
{
    // initialize an empty table
    dict *initframe = (dict *)ncalloc(1, sizeof(dict));
    initframe->table = ncalloc(maxwords * 2, sizeof(item));
    initframe->sz = maxwords;

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
    // get the hash number from hash function
    int index = hash_string(x, s);
    // initiate a char* and calloc inside so to store s
    int len = strlen(s);
    char *temp = (char *)ncalloc((len + 1), sizeof(char));
    strcpy(temp, s);

    // if table[hash number] contains NULL,store temp inside
    if (x->table[index].word == NULL)
    {
        x->table[index].word = temp;
    }
    else // if table[hash number] contains words, initiate a item *, store the word and point to previous word's next
    {
        item *temp_1 = (item *)ncalloc(1, sizeof(item));
        temp_1->word = temp;

        temp_1->next = x->table[index].next;
        x->table[index].next = temp_1;
    }
    return true;
}

bool dict_spelling(dict *x, const char *s)
{
    // if x is not a possible case, i.e. NULL
    if (!x)
    {
        return false;
    }

    int index = hash_string(x, s);
    // if table[hash number] contains NULL, then word is not in the list
    if (x->table[index].word == NULL)
    {
        return false;
    }
    else // if table [hash number] contains a word
    {
        // strcmp the new string with the stored word(blocking repeat cases)
        if (strcmp(x->table[index].word, s) == 0)
        {
            return true;
        }
        // initiate new item pointer to check if table[hash number]'s next contains words or not
        item *temp;
        temp = x->table[index].next;
        // printf("word %s \n",temp->word);

        // keep looking for a repeat word with temp pointer

        while (temp != NULL)
        {
            if (strcmp(temp->word, s) == 0)
            {
                return true;
            }
            else
            {
                temp = temp->next;
            }
        }

        return false;
    }
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

void dict_free(dict *x)
{ // free linked list inside table
    for (int i = 0; i < x->sz; i++)
    { // if it contains word, do free action
        if (x->table[i].word != NULL)
        { // two item pointers, temp_1 points to table[i]'s next
            item *temp_1 = x->table[i].next;
            item *temp_2;
            // if temp_1 contains word, use temp_2 to point to temp_1's next to keep the next word
            while (temp_1 != NULL)
            {
                temp_2 = temp_1->next;
                free(temp_1->word);
                free(temp_1);
                temp_1 = temp_2;
            }
        }
        free(x->table[i].word);
    }
    free(x->table);
    free(x);
}

void test()
{
    // initialize an empty table
    unsigned int maxwords = 20;
    dict *x = (dict *)ncalloc(1, sizeof(dict));
    x->table = ncalloc(maxwords * 2, sizeof(item));
    x->sz = maxwords;

    assert(!dict_add(NULL, "twenty"));
    assert(!dict_add(NULL, " "));
    assert(dict_add(x, "twenty"));
    assert(dict_add(x, "twenty"));

    assert(!dict_add(NULL, "hahahah"));
    assert(!dict_add(NULL, " "));
    assert(dict_add(x, "hahahah"));
    assert(dict_add(x, "hahahah"));

    assert(!dict_add(NULL, "christy"));
    assert(!dict_add(NULL, " "));
    assert(dict_add(x, "christy"));
    assert(dict_add(x, "christy"));

    assert(!dict_spelling(NULL, "twenty"));
    assert(dict_spelling(x, "twenty"));
    assert(!dict_spelling(x, "Twenty"));

    assert(!dict_spelling(NULL, "hahahah"));
    assert(dict_spelling(x, "hahahah"));
    assert(!dict_spelling(x, "hahahaH"));

    assert(!dict_spelling(NULL, "christy"));
    assert(dict_spelling(x, "christy"));
    assert(!dict_spelling(x, "Christy"));

    assert(!dict_spelling(x, "thirty"));
    assert(!dict_spelling(x, "better"));

    assert(hash_string(x, "twenty") < maxwords);
    assert(hash_string(x, "hahahah") < maxwords);
    assert(hash_string(x, "christy") < maxwords);
    assert(hash_string(x, "twenty") == 17);
    assert(hash_string(x, "hahahah") == 8);
    assert(hash_string(x, "christy") == 13);
}

/*
 * PLEASE SEE LICENSE FILE FOR LICENSING AND COPYRIGHT INFORMATION
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "dictionary.h"
#include "vector.h"


const char* global_defaultSignatures[] = {
    "about",
    "after",
    "all",
    "also",
    "and",
    "any",
    "back",
    "because",
    "but",
    "can",
    "come",
    "could",
    "day",
    "even",
    "first",
    "for",
    "from",
    "get",
    "give",
    "good",
    "have",
    "him",
    "how",
    "into",
    "its",
    "just",
    "know",
    "like",
    "look",
    "make",
    "most",
    "new",
    "not",
    "now",
    "one",
    "only",
    "other",
    "out",
    "over",
    "people",
    "say",
    "see",
    "she",
    "some",
    "take",
    "than",
    "that",
    "their",
    "them",
    "then",
    "there",
    "these",
    "they",
    "think",
    "this",
    "time",
    "two",
    "use",
    "want",
    "way",
    "well",
    "what",
    "when",
    "which",
    "who",
    "will",
    "with",
    "work",
    "would",
    "year",
    "your"
};

const long global_numDefaultSignature =
    sizeof(global_defaultSignatures) / sizeof(global_defaultSignatures[0]);

dictionary_t::dictionary_t()
{
    stuff = vector_alloc(global_numDefaultSignature);

    for (long s = 0; s < global_numDefaultSignature; s++) {
        const char* sig = global_defaultSignatures[s];
        bool status = vector_pushBack(stuff,
                                      (void*)sig);
        assert(status);
    }
}

dictionary_t::~dictionary_t()
{
    vector_free(stuff);
}

bool dictionary_t::add(char* str)
{
    return vector_pushBack(stuff, (void*)str);
}

char* dictionary_t::get(long i)
{
    return (char*)vector_at(stuff, i);
}

/* =============================================================================
 * dictionary_match
 * =============================================================================
 */
char* dictionary_t::match(char* str)
{
    long s;
    long numSignature = vector_getSize(stuff);

    for (s = 0; s < numSignature; s++) {
        char* sig = (char*)vector_at(stuff, s);
        if (strstr(str, sig) != NULL) {
            return sig;
        }
    }

    return NULL;
}


/* #############################################################################
 * TEST_DICTIONARY
 * #############################################################################
 */
#ifdef TEST_DICTIONARY


#include <assert.h>
#include <stdio.h>


int
main ()
{
    puts("Starting...");

    dictionary_t* dictionaryPtr;

    dictionaryPtr = dictionary_alloc();
    assert(dictionaryPtr);

    assert(dictionary_add(dictionaryPtr, "test1"));
    char* sig = dictionary_match(dictionaryPtr, "test1");
    assert(strcmp(sig, "test1") == 0);
    sig = dictionary_match(dictionaryPtr, "test1s");
    assert(strcmp(sig, "test1") == 0);
    assert(!dictionary_match(dictionaryPtr, "test2"));

    long s;
    for (s = 0; s < global_numDefaultSignature; s++) {
        char* sig = dictionary_match(dictionaryPtr, global_defaultSignatures[s]);
        assert(strcmp(sig, global_defaultSignatures[s]) == 0);
    }

    puts("All tests passed.");

    return 0;
}


#endif /* TEST_DICTIONARY */


#include "engine/transposition_table.h"

// !! TT IS A HASH TABLE
//  sizeof (TTEntry) = 104 bit -> 13 byte with padding 16 byte

#define TT_MB 64

static TTEntry* TT = NULL;
static size_t desired = TT_MB * 1024 * 1024 / sizeof(TTEntry); // 64mb 
static size_t TT_size;

int TT_init()
{
    // finding power of 2
    TT_size = 1;
    while (TT_size * 2 <= desired)
        TT_size *= 2;

    TT = malloc(TT_size * sizeof(TTEntry));
    if (!TT)
    {
        fprintf(stderr, "TT: malloc failed\n");
        return -1;
    }
    
    memset(TT, 0, TT_size * sizeof(TTEntry));

    return 0;
}

void TT_clear()
{
    if (TT)
    {
        free(TT);
        TT = NULL;
        TT_size = 0;
    }
}

// might make this modulo or something if its colliding
static inline int get_TT_index(uint64_t hash)
{
    return hash & (TT_size - 1);
}

void TT_store(TTEntry* new_entry)
{
    size_t index = get_TT_index(new_entry->hash);
    TTEntry* slot = &TT[index];

    // only store if greater or equal depth
    if (slot->depth <= new_entry->depth)
        *slot = *new_entry;
}

TTEntry* TT_lookup(uint64_t hash)
{
    size_t index = get_TT_index(hash);

    return &TT[index];
}


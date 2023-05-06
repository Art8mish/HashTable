
#include "../include/hash_table.h"

uint64_t hash_cnst(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    return 1;
}

uint64_t hash_symb(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    uint64_t hash = *word;

    return hash;
}

uint64_t hash_strlen(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    uint64_t hash = strlen(word);

    return hash;
}


uint64_t hash_ascii(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    uint64_t hash = 0;
    while (*word != '\0')
    {
        hash += *word;
        word++;
    }

    return hash;
}


static uint64_t rol1(uint64_t num)
{
    uint64_t bit = num >> (sizeof(num) * 8 - 1);

    return (num << 1) | bit;
}

static uint64_t ror1(uint64_t num)
{
    uint64_t bit = num << (sizeof(num) * 8 - 1);

    return (num >> 1) | bit;
}

uint64_t hash_rol(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    uint64_t hash = 0;
    while (*word != '\0')
    {
        hash = rol1(hash) ^ (*word);
        word++;
    }

    return hash;
}

uint64_t hash_ror(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    uint64_t hash = 0;
    while (*word != '\0')
    {
        hash = ror1(hash) ^ (*word);
        word++;
    }

    return hash;
}
 
static int HSH_CNST = 33;
uint64_t hash_addmul(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    uint64_t hash = HSH_CNST;
    while (*word != '\0')
    {
        hash += *word;
        hash *= HSH_CNST;
        word++;
    }

    return hash;
}
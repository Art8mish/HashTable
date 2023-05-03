
#include "../include/hash_table.h"

ull hash_cnst(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    return 1;
}

ull hash_symb(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    ull hash = *word;

    return hash;
}

ull hash_strlen(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    ull hash = strlen(word);

    return hash;
}


ull hash_ascii(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    ull hash = 0;
    while (*word != '\0')
    {
        hash += *word;
        word++;
    }

    return hash;
}


static ull rol1(ull num)
{
    ull bit = num >> (sizeof(num) * 8 - 1);

    return (num << 1) | bit;
}

static ull ror1(ull num)
{
    ull bit = num << (sizeof(num) * 8 - 1);

    return (num >> 1) | bit;
}

ull hash_rol(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    ull hash = 0;
    while (*word != '\0')
    {
        hash = rol1(hash) ^ (*word);
        word++;
    }

    return hash;
}

ull hash_ror(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    ull hash = 0;
    while (*word != '\0')
    {
        hash = ror1(hash) ^ (*word);
        word++;
    }

    return hash;
}
 
static int HSH_CNST = 33;
ull hash_mrot(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);

    ull hash = HSH_CNST;
    while (*word != '\0')
    {
        hash += *word;
        hash *= HSH_CNST;
        word++;
    }

    return hash;
}

ull hash_130k(const char *word)
{
    ERR_CHCK(word == NULL, ERR_CALC_HASH);


}
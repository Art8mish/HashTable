#ifndef HASHTBL_H_INCLUDED
#define HASHTBL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <cctype>

#include <unistd.h>

#include <immintrin.h>

#include "../list_func/include/ptr_list.h"


#ifdef SOFT_ASSERT
#undef SOFT_ASSERT
#endif

#define SOFT_ASSERT(condition)                                                    \
            do                                                                    \
            {                                                                     \
                if (condition)                                                    \
                    printf("Error in %s = %d; file: %s; num of line: %d \n",      \
                           #condition, condition, __FILE__, __LINE__);            \
            } while(false)

#define ERR_CHCK(cond, error)                               \
            do                                              \
            {                                               \
                SOFT_ASSERT(cond);                          \
                if (cond)                                   \
                    return error;                           \
            } while(false)

#define FILE_ERR_CHCK(cond, error, closing_file)            \
            do                                              \
            {                                               \
                SOFT_ASSERT(cond);                          \
                if (cond)                                   \
                {                                           \
                    fclose(closing_file);                   \
                    return error;                           \
                }                                           \
            } while(false)

const size_t MAX_TBL_SIZE = 10000;

enum Errors
{
    ERR_CALC_HASH         = 0,
    ERR_FILE_OPENING      = 1,
    ERR_STAT              = 2,
    ERR_FILE_CLOSING      = 4,
    ERR_BIG_LST_SIZE      = 6,
    ERR_CREATE_ONG_FIELD  = 7,
    ERR_TBL_ADD           = 8,
    ERR_OVERSIZE          = 9,
    ERR_CHCK_WRD_EXST     = 10,
    ERR_HASH_FUNC         = 11,
    ERR_HASH_TBL_SORT     = 12,
    ERR_EXCEL_DUMP        = 13,
    ERR_HASH_TBL_CLEAN    = 14,
    ERR_DATA_CLEAN        = 15,
    ERR_TBL_CTOR          = 16,
    ERR_NO_LIST           = 17,
    ERR_NULL_FICT_NODE    = 18,
    ERR_VEC_DATA          = 19,
    ERR_TBL_DTOR          = 20,
    ERR_BIG_STR           =21
};

enum HashFuncNums
{
    HASH_CNST   = 0,
    HASH_SYMB   = 1,
    HASH_STRLEN = 2,
    HASH_ASCII  = 3,
    HASH_ROL    = 4,
    HASH_ROR    = 5,
    HASH_MROT   = 6,
};


typedef struct Data
{
    char *char_buf = NULL;
    void **wrd_buf = NULL;
    unsigned wrd_amnt = 0;
    bool vec32 = false;
} Data;

// typedef struct _Data256i
// {
//     char *char_buf = NULL;
//     const __m256i **wrd_buf = NULL;
//     unsigned wrd_amnt = 0;
// } D256i;


typedef struct HashTable
{
    Data *data;
    List **lst_arr;
    unsigned size;
    uint64_t (*hsh_fnc)(const char *);

} HshTbl;


const int MAX_WORD_LEN = 10;

Data *dataCtor(const char  *file_path);
int   dataVec32(Data *data);
int   dataDtor (Data *data);

HshTbl *tblCtor(const unsigned tbl_size);
int     tblDtor(HshTbl *hsh_tbl);

int tblLstDump(HshTbl *hsh_tbl);
int tblCsvDump(HshTbl *hsh_tbl, const char *hash_f_name);

int tblHashSort(HshTbl *hsh_tbl, const char *file_path, uint64_t (*hash_func)(const char *), bool vec_f);
int tblAdd     (HshTbl *hsh_tbl, unsigned index, void *str);
int tblClean   (HshTbl **hsh_tbl);

int tblFindKey(HshTbl *hsh_tbl, void *word);

uint64_t hash_cnst  (const char *word);
uint64_t hash_symb  (const char *word);
uint64_t hash_strlen(const char *word);
uint64_t hash_ascii (const char *word);
uint64_t hash_rol   (const char *word);
uint64_t hash_ror   (const char *word);
uint64_t hash_addmul(const char *word);

extern uint64_t _asm_hash_addmul(const char *word);

int inline asm_strcmp(const char* str1, const char* str2);
int avx_strcmp(__m256i str1, __m256i str2);

#endif //HASHTBL_H_INCLUDED
#ifndef HASHTBL_H_INCLUDED
#define HASHTBL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

#include <sstream>
#include <iostream>

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


typedef unsigned long ul;
typedef unsigned long long ull;

typedef struct Data
{
          char *char_buf = NULL;
    const char **wrd_buf = NULL;
    unsigned wrd_amnt = 0;
} Data;

typedef struct HashTable
{
    Data *data = NULL;
    List **lst_arr = NULL;
    unsigned size = 0;
    ull (*hsh_fnc)(const char *) = NULL;

} HshTbl;



const int MAX_WORD_LEN = 10;

struct Data *GetData(const char  *file_path);
int ClnData(struct Data *data);

HshTbl *tblCtor(const unsigned tbl_size);
int tblDtor(HshTbl *hsh_tbl);

int tblLstDump(HshTbl *hsh_tbl);
int tblCsvDump(HshTbl *hsh_tbl, const char *hash_f_name);

int tblHashSort(HshTbl *hsh_tbl, const char *file_path, ull (*hash_func)(const char *));
int tblAdd     (HshTbl *hsh_tbl, unsigned index, const char *str);
int tblClean   (HshTbl **hsh_tbl);

ull hash_cnst  (const char *word);
ull hash_symb  (const char *word);
ull hash_strlen(const char *word);
ull hash_ascii (const char *word);
ull hash_rol   (const char *word);
ull hash_ror   (const char *word);
ull hash_mrot  (const char *word);

int WrdInTbl(HshTbl *hsh_tbl, const char *word);

#endif //HASHTBL_H_INCLUDED
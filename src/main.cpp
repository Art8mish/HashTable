
#include "../include/hash_table.h"

#include <time.h>

const char *txt_f_path = "data_txt/crm_pnshmnt.txt";


int main(void)
{
    HshTbl *hsh_tbl = tblCtor(3001);
    ERR_CHCK(hsh_tbl == NULL, 1);

    // uint64_t (*hash_funcs[7])(const char *) = {hash_cnst, hash_symb, hash_strlen, hash_ascii, hash_rol, hash_ror, hash_mrot};
    // const char *hash_f_names[7] = {"hash_cnst", "hash_symb", "hash_strlen", "hash_ascii", "hash_rol", "hash_ror", "hash_mrot"};

    // for (int f_i = 0; f_i < 7; f_i++)
    // {
    //     int err = tblHashSort(hsh_tbl, txt_f_path, hash_funcs[f_i]);
    //     ERR_CHCK(err, ERR_HASH_TBL_SORT);

    //     err = tblCsvDump(hsh_tbl, hash_f_names[f_i]);
    //     ERR_CHCK(err, ERR_EXCEL_DUMP);

    //     err = tblClean(&hsh_tbl);
    //     ERR_CHCK(err, ERR_HASH_TBL_CLEAN);
    // }

    // uint64_t cmn_hash = hash_mrot("hash_mrot");
    // uint64_t asm_hash = _asm_hash_addmul("hash_mrot");

    // printf("cmn_hash: %ld\n asm_hash: %ld\n", cmn_hash, asm_hash);

    uint64_t (*hash_funcs[2])(const char *) = {hash_addmul, _asm_hash_addmul};
    int err = tblHashSort(hsh_tbl, txt_f_path, hash_funcs[1], true);
    ERR_CHCK(err, ERR_HASH_TBL_SORT);

    char word1[32] = {"CRIME"};
    char word2[32] = {"Raskolnikov"};
    char word3[32] = {"underground"};

    char word[] = "Raskolnikov";

    clock_t start_time = clock();

    int wrd_in_tbl = 0;
    int cyc_n = 0;
    while(cyc_n < 10000000)
    {
        wrd_in_tbl = tblFindKey(hsh_tbl, word2);
        //ERR_CHCK(wrd_in_tbl == -1, 1);
        cyc_n++;
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    FILE *log_f = fopen("logs/log.txt", "a");
    ERR_CHCK(log_f == NULL, ERR_FILE_OPENING);

    fprintf(log_f, "%lf\n", elapsed_time);

    printf("Word was ");
    if (wrd_in_tbl)
        printf("found ");
    else
        printf("not found ");
    printf("in %lf seconds (%d cycles)\n", elapsed_time, cyc_n);

    fclose(log_f);
    err = tblDtor(hsh_tbl);
    ERR_CHCK(err, ERR_TBL_DTOR);

    return SUCCESS;
}
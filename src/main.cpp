
#include "../include/hash_table.h"

#include <time.h>

const char *txt_f_path = "data_txt/crm_pnshmnt.txt";

int main(int argc, char *argv[])
{
    HshTbl *hsh_tbl = tblCtor(1009);
    ERR_CHCK(hsh_tbl == NULL, 1);

    // ull (*hash_funcs[7])(const char *) = {hash_cnst, hash_symb, hash_strlen, hash_ascii, hash_rol, hash_ror, hash_mrot};
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

    int err = tblHashSort(hsh_tbl, txt_f_path, hash_mrot);
    ERR_CHCK(err, ERR_HASH_TBL_SORT);

    err = tblCsvDump(hsh_tbl, "hash_mrot");
    ERR_CHCK(err, ERR_EXCEL_DUMP);

    clock_t start_time = clock();

    int wrd_in_tbl = 0;
    int cyc_n = 0;
    while(cyc_n < 10000000)
    {
        wrd_in_tbl = WrdInTbl(hsh_tbl, "");
        ERR_CHCK(wrd_in_tbl == -1, 1);
        cyc_n++;
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) /CLOCKS_PER_SEC;

    printf("Word was ");
    if (wrd_in_tbl)
        printf("found ");
    else
        printf("not found ");
    printf("in %lf seconds (%d cycles)\n", elapsed_time, cyc_n);

    tblDtor(hsh_tbl);

    return SUCCESS;
}
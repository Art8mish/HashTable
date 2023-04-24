
#include "../include/hash_table.h"

HshTbl *tblCtor(unsigned tbl_size)
{
    ERR_CHCK(tbl_size > MAX_TBL_SIZE, NULL);

    HshTbl *hsh_tbl = (HshTbl *) calloc(1, sizeof(HshTbl));
    ERR_CHCK(hsh_tbl == NULL, NULL);

    hsh_tbl->lst_arr = (List **) calloc(tbl_size, sizeof(List *));
    ERR_CHCK(hsh_tbl->lst_arr == NULL, NULL);

    for (unsigned i = 0; i < tbl_size; i++)
    {
        hsh_tbl->lst_arr[i] = listCtor();
        ERR_CHCK(hsh_tbl->lst_arr[i] == NULL, NULL);
    }

    hsh_tbl->size = tbl_size;

    return hsh_tbl;
}

int tblDtor(HshTbl *hsh_tbl)
{
    ERR_CHCK(hsh_tbl == NULL, ERROR_NULL_PTR);

    for (unsigned i = 0; i < hsh_tbl->size; i++)
    {
        if (hsh_tbl->lst_arr[i] != NULL)
        {
            int err = listDtor(hsh_tbl->lst_arr[i]);
            ERR_CHCK(err, ERROR_LIST_DTOR);
        }
    }

    free(hsh_tbl);

    return SUCCESS;
}


static int ChckWrdExst(List *lst, const char *word, bool *flag)
{
    ERR_CHCK(lst  == NULL, ERROR_NULL_PTR);
    ERR_CHCK(word == NULL, ERROR_NULL_PTR);

    bool wrd_exst = false;
    LstNode *crr_nod = lst->fict_node->next;
    for (unsigned data_i = 0; data_i < lst->size; data_i++)
    {
        if (strcmp(word, crr_nod->value) == 0)
        {
            wrd_exst = true;
            break;
        }

        crr_nod = crr_nod->next;
    }

    *flag = wrd_exst;

    return SUCCESS;
}

#define HSH_LST hsh_tbl->lst_arr[hash]

int tblHashSort(HshTbl *hsh_tbl, const char *file_path, ull (*hash_func)(const char *))
{
    ERR_CHCK(hsh_tbl   == NULL, ERROR_NULL_PTR);
    ERR_CHCK(file_path == NULL, ERROR_NULL_PTR);
    ERR_CHCK(hash_func == NULL, ERROR_NULL_PTR);

    hsh_tbl->hsh_fnc = hash_func;

    Data *data = GetData(file_path);
    ERR_CHCK(data == NULL, ERR_CREATE_ONG_FIELD);

    for (unsigned wrd_i = 0; wrd_i < data->wrd_amnt; wrd_i++)
    {
        unsigned hash = hash_func(data->wrd_buf[wrd_i]);

        if (hash == ERR_CALC_HASH)
            printf("err_word: %s\n", data->wrd_buf[wrd_i]);
        ERR_CHCK(hash == ERR_CALC_HASH, ERR_HASH_FUNC);


        hash = hash % hsh_tbl->size;

        bool wrd_exst = false;
        int err = ChckWrdExst(HSH_LST, data->wrd_buf[wrd_i], &wrd_exst);
        ERR_CHCK(err, ERR_CHCK_WRD_EXST);

        if (!wrd_exst)
        {
            err = tblAdd(hsh_tbl, hash, data->wrd_buf[wrd_i]);
            ERR_CHCK(err, ERR_TBL_ADD);
        }
    }

    return SUCCESS;
}



int tblAdd(HshTbl *hsh_tbl, unsigned index, const char *str)
{
    ERR_CHCK(hsh_tbl == NULL, ERROR_NULL_PTR);
    ERR_CHCK(index   >= hsh_tbl->size, ERR_OVERSIZE);

    struct Node *new_node = NULL;
    int err = listPushBack(hsh_tbl->lst_arr[index], str, &new_node);
    ERR_CHCK(err, ERROR_PUSH_BACK);

    return SUCCESS;
}

int tblClean(HshTbl *hsh_tbl)
{
    ERR_CHCK(hsh_tbl == NULL, ERROR_NULL_PTR);

    for (unsigned i = 0; i < hsh_tbl->size; i++)
    {
        int err = listClean(hsh_tbl->lst_arr[i]);
        ERR_CHCK(err, ERROR_LIST_CLEAN);
    }

    return SUCCESS;
}

int tblLstDump(HshTbl *hsh_tbl)
{
    ERR_CHCK(hsh_tbl == NULL, ERROR_NULL_PTR);

    FILE *log_f = fopen("logs/log.txt", "a");
    ERR_CHCK(log_f == NULL, ERR_FILE_OPENING);


    for (unsigned i = 0; i < hsh_tbl->size; i++)
    {

        fprintf(log_f, "list[%d]", i);

        LstNode *crr_nod = hsh_tbl->lst_arr[i]->fict_node;
        for (unsigned lst_i = 0; lst_i < hsh_tbl->lst_arr[i]->size; lst_i++)
        {
            crr_nod = crr_nod->next;
            fprintf(log_f, " -> %s", crr_nod->value);
        }

        fprintf(log_f, "\n\n");
    }

    fclose(log_f);

    return SUCCESS;
}

int tblCsvDump(HshTbl *hsh_tbl, const char *hash_f_name)
{
    ERR_CHCK(hsh_tbl == NULL, ERROR_NULL_PTR);

    FILE *log_f = fopen("logs/hash_info.txt", "a");
    ERR_CHCK(log_f == NULL, ERR_FILE_OPENING);

    fprintf(log_f, "%s\n", hash_f_name);

    for (unsigned i = 0; i < hsh_tbl->size; i++)
        fprintf(log_f, "%-3d    ",  i);
    fprintf(log_f, "\n");
    for (unsigned i = 0; i < hsh_tbl->size; i++)
        fprintf(log_f, "%-3d    ",  hsh_tbl->lst_arr[i]->size);

    fprintf(log_f, "\n\n");

    fclose(log_f);

    return SUCCESS;
}
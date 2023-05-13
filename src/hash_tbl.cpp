
#include "../include/hash_table.h"

HshTbl *tblCtor(unsigned tbl_size)
{
    ERR_CHCK(tbl_size > MAX_TBL_SIZE, NULL);

    HshTbl *hsh_tbl = (HshTbl *) calloc(1, sizeof(HshTbl));
    ERR_CHCK(hsh_tbl == NULL, NULL);

    hsh_tbl->lst_arr = (List **) calloc(tbl_size, sizeof(List *));
    ERR_CHCK(hsh_tbl->lst_arr == NULL, NULL);

    hsh_tbl->data = NULL;

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

    if (hsh_tbl->data != NULL)
    {
        int err = dataDtor(hsh_tbl->data);
        ERR_CHCK(err, ERR_DATA_CLEAN);        
    }

    if (hsh_tbl->lst_arr != NULL)
        free(hsh_tbl->lst_arr);

    free(hsh_tbl);

    return SUCCESS;
}


static int tblChckWrd(List *lst, void *word, bool *flag)
{
    ERR_CHCK(lst  == NULL, ERROR_NULL_PTR);
    ERR_CHCK(word == NULL, ERROR_NULL_PTR);

    bool wrd_exst = false;
    Node *crr_nod = lst->fict_node->next;
    for (unsigned data_i = 0; data_i < lst->size; data_i++)
    {
        if (strcmp((const char *)word, (const char *)crr_nod->value) == 0)
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

int tblHashSort(HshTbl *hsh_tbl, const char *file_path, ull (*hash_func)(const char *), bool vec_f)
{
    ERR_CHCK(hsh_tbl   == NULL, ERROR_NULL_PTR);
    ERR_CHCK(file_path == NULL, ERROR_NULL_PTR);
    ERR_CHCK(hash_func == NULL, ERROR_NULL_PTR);

    hsh_tbl->hsh_fnc = hash_func;

    Data *data = dataCtor(file_path);
    ERR_CHCK(data == NULL, ERR_CREATE_ONG_FIELD);

    hsh_tbl->data = data;

    if (vec_f)
    {
        int err = dataVec32(data);
        ERR_CHCK(err, ERR_VEC_DATA);
        printf("Data is vectorised...\n");
    }

    FILE *log_f = fopen("logs/hash_info.txt", "a");
    ERR_CHCK(log_f == NULL, ERR_FILE_OPENING);

    for (unsigned wrd_i = 0; wrd_i < data->wrd_amnt; wrd_i++)
    {
        unsigned int hash = hash_func((const char *)data->wrd_buf[wrd_i]);
        //fprintf(log_f, "%s = %u; ", data->wrd_buf[wrd_i], hash);

        if (hash == ERR_CALC_HASH)
            printf("err_word: %s\n", data->wrd_buf[wrd_i]);
        ERR_CHCK(hash == ERR_CALC_HASH, ERR_HASH_FUNC);

        hash = hash % hsh_tbl->size;

        bool wrd_exst = false;
        int err = tblChckWrd(HSH_LST, data->wrd_buf[wrd_i], &wrd_exst);
        ERR_CHCK(err, ERR_CHCK_WRD_EXST);

        if (!wrd_exst)
        {
            err = tblAdd(hsh_tbl, hash, data->wrd_buf[wrd_i]);
            ERR_CHCK(err, ERR_TBL_ADD);
        }
    }

    fclose(log_f);

    return SUCCESS;
}

int tblAdd(HshTbl *hsh_tbl, unsigned index, void *str)
{
    ERR_CHCK(hsh_tbl == NULL, ERROR_NULL_PTR);
    ERR_CHCK(index   >= hsh_tbl->size, ERR_OVERSIZE);

    struct Node *new_node = NULL;
    int err = listPushBack(hsh_tbl->lst_arr[index], str, &new_node);
    ERR_CHCK(err, ERROR_PUSH_BACK);

    return SUCCESS;
}

int tblClean(HshTbl **hsh_tbl)
{
    ERR_CHCK( hsh_tbl == NULL, ERROR_NULL_PTR);
    ERR_CHCK(*hsh_tbl == NULL, ERROR_NULL_PTR);

    unsigned size = (*hsh_tbl)->size;

    int err = tblDtor(*hsh_tbl);
    ERR_CHCK(err, ERROR_LIST_DTOR);

    *hsh_tbl = tblCtor(size);
    ERR_CHCK(*hsh_tbl == NULL, ERR_TBL_CTOR);

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

        Node *crr_nod = hsh_tbl->lst_arr[i]->fict_node;
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
        fprintf(log_f, "%-3lu    ",  hsh_tbl->lst_arr[i]->size);

    fprintf(log_f, "\n\n");

    fclose(log_f);


    return SUCCESS;
}

#define AVX_MOD

int tblFindKey(HshTbl *hsh_tbl, void *word)
{
    //ERR_CHCK(hsh_tbl == NULL, -1);
    //ERR_CHCK(word    == NULL, -1);

    unsigned int hash = (unsigned int)hsh_tbl->hsh_fnc((const char *)word);
    hash = hash % hsh_tbl->size;

    List *lst = hsh_tbl->lst_arr[hash];
    //ERR_CHCK(lst            == NULL, -1);
    //ERR_CHCK(lst->fict_node == NULL, -1);

    Node *cur_nod = lst->fict_node->next;
    int wrd_flg = 0;

    #ifdef AVX_MOD

    __m256i wrd1;
    memcpy(&wrd1, word, sizeof(__m256i));

    #endif

    for (unsigned i = 0; i < lst->size; i++)
    {
        #ifndef AVX_MOD

        if (asm_strcmp((const char *)word, (const char *)cur_nod->value) == 0)
        {
            wrd_flg = 1;
            break;
        }

        #else
        
        __m256i wrd2;
        memcpy(&wrd2, cur_nod->value, sizeof(__m256i));

        if (_mm256_testc_si256(wrd1, wrd2))
        {
            wrd_flg = 1;
            break;
        }

        // if (avx_strcmp(wrd1, wrd2) == 0)
        // {
        //     wrd_flg = 1;
        //     break;
        // }

        #endif

        cur_nod = cur_nod->next;
    }

    return wrd_flg;
}

int inline asm_strcmp(const char* str1, const char* str2)
{
    int ret = 0;

    __asm__
    (
        ".intel_syntax noprefix \n\t"

        "mov rdi, %1            \n\t"
        "mov rsi, %2            \n\t"

        "xor eax, eax           \n\t"
        "xor ebx, ebx           \n\t"

        "_asm_strcmp_loop:      \n\t"
        "mov al, byte ptr [rdi] \n\t"
        "mov bl, byte ptr [rsi] \n\t"

        "cmp al, 0              \n\t"
        "je _asm_strcmp_end     \n\t"
        "cmp bl, 0              \n\t"
        "je _asm_strcmp_end     \n\t"

        "cmp al, bl             \n\t"
        "jne _asm_strcmp_end    \n\t"

        "inc rdi                \n\t"
        "inc rsi                \n\t"
        "jmp _asm_strcmp_loop   \n\t"

        "_asm_strcmp_end:       \n\t"
    	"sub eax, ebx           \n\t"
        "mov %0, eax\n\t"

        ".att_syntax prefix\n\t"

        : "=r" (ret) : "r" (str1), "r" (str2) 
        : "rax", "rbx", "rdi", "rsi", "r8", "r9"
    );

    return ret;
}

int avx_strcmp(__m256i str1, __m256i str2)
{
    __m256i mask = _mm256_cmpeq_epi8(str1, str2);   //compare 8-bit integers

    // __m256i _1 = _mm256_set1_epi8(0xFF);
    // mask = _mm256_xor_si256(mask, _1);

    str1 = _mm256_andnot_si256(mask, str1);         //wrd1[i] = !mask[i] & wrd1[i])
    str2 = _mm256_andnot_si256(mask, str2);         //wrd1[i] = !mask[i] & wrd1[i])
    
    __m256i diff = _mm256_sub_epi8(str1, str2);

    for (int i = 0; i < 32; i++)
    {
        if (((int8_t *)&diff)[i])
            return ((int8_t *)&diff)[i];
    }

    return 0;
}
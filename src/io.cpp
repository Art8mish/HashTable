
#include "../include/hash_table.h"

static unsigned ReadFileData(const char *file_path, char *char_buf, 
                             unsigned    char_amnt, void **wrd_buf);
static unsigned CalcFileSize(const char *file_path);

Data *dataCtor(const char *file_path)
{
    ERR_CHCK(file_path == NULL, NULL);

    Data *io_env = (Data*) calloc(1, sizeof(Data));
    ERR_CHCK(io_env == NULL, NULL);

    unsigned size = CalcFileSize(file_path) + 2;
    ERR_CHCK(size == 2, NULL);

    io_env->char_buf = (char*) calloc(size, sizeof(char));
    ERR_CHCK(io_env->char_buf == NULL, NULL);

    io_env->wrd_buf  = (void **) calloc(size, sizeof(void*));
    ERR_CHCK(io_env->wrd_buf == NULL, NULL);
    

    unsigned wrd_amnt = ReadFileData(file_path, io_env->char_buf, size, io_env->wrd_buf);
    ERR_CHCK(wrd_amnt == 0, NULL);

    io_env->wrd_buf = (void **) realloc(io_env->wrd_buf, wrd_amnt * sizeof(void*));
    ERR_CHCK(io_env->wrd_buf == NULL, NULL);

    io_env->wrd_amnt = wrd_amnt;
    io_env->vec32 = false;

    return io_env;
}

unsigned ReadFileData(const char *file_path, char *char_buf, unsigned char_amnt, void **wrd_buf)
{
    ERR_CHCK(file_path == NULL, 0);
    ERR_CHCK(char_buf  == NULL, 0);
    ERR_CHCK(wrd_buf   == NULL, 0);

    FILE *input_file = fopen(file_path, "r");
    ERR_CHCK(input_file == NULL, -1);

    fread(char_buf, sizeof(char), char_amnt, input_file);

    fclose(input_file);

    unsigned words_count = 0;
    bool empt_wrd   = true;

    for (unsigned i = 0; i < char_amnt; i++)
    {
        bool is_word = isalpha(*char_buf);

        if (is_word && empt_wrd)
        {
            *(wrd_buf++) = char_buf;
            empt_wrd = false;
        }

        if (!is_word && !empt_wrd)
        {
            *char_buf = '\0';

            words_count++;
            empt_wrd = true;
        }

        char_buf++;
    }

    return words_count;
}


unsigned CalcFileSize(const char *file_path)
{
    ERR_CHCK(file_path == NULL, 0);
    
    FILE *file = fopen(file_path, "r");
    ERR_CHCK(file == NULL, 0);

    fseek(file, 0, SEEK_END);
    unsigned size = ftell(file);

    fclose(file);

    return size;
}

int dataVec32(Data *data)
{
    ERR_CHCK(data == NULL, ERROR_NULL_PTR);
    
    int vec_len = sizeof(__m256i);

    if (data->vec32)
        return SUCCESS;

    for (unsigned i = 0; i < data->wrd_amnt; i++)
    {
        unsigned word_len = strlen((const char *)data->wrd_buf[i]) + 1; //+1 means last '\0'
        ERR_CHCK(word_len > sizeof(__m256i), ERR_BIG_STR);
        unsigned  ext_len = vec_len - (word_len % vec_len);

        char vec_word[vec_len] = {};
        ERR_CHCK(vec_word == NULL, ERROR_CALLOC);

        strcpy(vec_word, (const char *)data->wrd_buf[i]);
        for (unsigned i_0 = 0; i_0 < ext_len; i_0++)
            vec_word[word_len + i_0] = '\0';

        __m256i *m256i_word = (__m256i *) calloc(1, sizeof(__m256i));
        ERR_CHCK(m256i_word == NULL, ERROR_CALLOC);
        
        memcpy(m256i_word, vec_word, sizeof(__m256i));

        data->wrd_buf[i] = m256i_word;
    }

    data->vec32 = true;
    return SUCCESS;    
}

// D256i *VecData256i(Data *data)
// {
//     ERR_CHCK(data == NULL, NULL);

//     if (!data->vec)
//         return NULL;

//     D256i *m256i_data = (D256i *) calloc(1, sizeof(D256i));
//     ERR_CHCK(data == NULL, NULL);

//     for (unsigned i = 0; i < data->wrd_amnt; i++)
//     {
//         int word_len = strlen(data->wrd_buf[i]) + 1; //+1 means last '\0'
//         ERR_CHCK(word_len > sizeof(__m256i), NULL);

//         __m256i *m256i_word = (__m256i *) calloc(1, sizeof(__m256i));
//         ERR_CHCK(m256i_word == NULL, NULL);

//         memcpy(m256i_word, data->wrd_buf[i], sizeof(__m256i));
//         m256i_data->wrd_buf[i] = m256i_word;
//     }

//     int err = ClnData(data);
//     ERR_CHCK(err, NULL);

//     return m256i_data;    
// }

int dataDtor(Data *data)
{
    ERR_CHCK(data == NULL, ERROR_NULL_PTR);

    if (data->vec32)
    {
        for(unsigned i = 0; i < data->wrd_amnt; i++)
            free((__m256i *)data->wrd_buf[i]);
    }
    
    free(data->char_buf);
    free(data->wrd_buf);
    free(data);

    return SUCCESS;
}
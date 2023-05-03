
#include "../include/hash_table.h"

static int ReadFileData(const char *file_path,    char *char_buf, 
                        unsigned char_amnt, const char **wrd_buf);
static unsigned CalcFileSize(const char *file_path);

Data *GetData(const char *file_path)
{
    ERR_CHCK(file_path == NULL, NULL);

    Data *io_env = (Data*) calloc(1, sizeof(Data));
    ERR_CHCK(io_env == NULL, NULL);

    unsigned size = CalcFileSize(file_path) + 2;
    ERR_CHCK(size == 2, NULL);

    io_env->char_buf = (char*) calloc(size, sizeof(char));
    ERR_CHCK(io_env->char_buf == NULL, NULL);

    io_env->wrd_buf  = (const char **) calloc(size, sizeof(const char*));
    ERR_CHCK(io_env->wrd_buf == NULL, NULL);
    

    unsigned wrd_amnt = ReadFileData(file_path, io_env->char_buf, size, io_env->wrd_buf);
    ERR_CHCK(wrd_amnt == 0, NULL);

    io_env->wrd_buf = (const char **) realloc(io_env->wrd_buf, wrd_amnt * sizeof(const char*));
    ERR_CHCK(io_env->wrd_buf == NULL, NULL);

    io_env->wrd_amnt = wrd_amnt;

    return io_env;
}

int ReadFileData(const char *file_path, char *char_buf, unsigned char_amnt, const char **wrd_buf)
{
    ERR_CHCK(file_path == NULL, 0);
    ERR_CHCK(char_buf  == NULL, 0);
    ERR_CHCK(wrd_buf   == NULL, 0);

    FILE *input_file = fopen(file_path, "r");
    ERR_CHCK(input_file == NULL, -1);

    fread(char_buf, sizeof(char), char_amnt, input_file);

    fclose(input_file);

    int words_count = 0;
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

int ClnData(Data *data)
{
    ERR_CHCK(data == NULL, ERROR_NULL_PTR);

    free(data->char_buf);
    free(data->wrd_buf);
    free(data);

    return SUCCESS;
}
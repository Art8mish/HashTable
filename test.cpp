
#include <stdio.h>
#include <string.h>

#include <immintrin.h>

typedef unsigned long long ull;

static int HSH_CNST = 33;
ull hash_mrot(const char *word)
{
    ull hash = HSH_CNST;
    int i = 0;
    while (*word != '\0')
    {
        hash += *word;
        hash *= HSH_CNST;
        word++;
     
        printf("[%d] = %lld\n", i, hash);
        i++;
    }

    return hash;
}

extern ull _asm_hash_addmul(const char *word);

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

int avx_strcmp(const char *str1, const char *str2)
{
    __m256i wrd1;
    __m256i wrd2;
    memcpy(&wrd1, str1, sizeof(__m256i));
    memcpy(&wrd2, str2, sizeof(__m256i));

    __m256i mask = _mm256_cmpeq_epi8(wrd1, wrd2);   //compare 8-bit integers

    // __m256i _1 = _mm256_set1_epi8(0xFF);
    // mask = _mm256_xor_si256(mask, _1);

    wrd1 = _mm256_andnot_si256(mask, wrd1);         //wrd1[i] = !mask[i] & wrd1[i])
    wrd2 = _mm256_andnot_si256(mask, wrd2);         //wrd1[i] = !mask[i] & wrd1[i])
    
    __m256i diff = _mm256_sub_epi8(wrd1, wrd2);

    for (int i = 0; i < 32; i++)
    {
        if (((signed char *)&diff)[i])
            return ((signed char *)&diff)[i];
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // unsigned int cmn_hash = hash_mrot("CRIME");
    // unsigned int asm_hash = _asm_hash_addmul("CRIME");

    // printf("cmn_hash: %u\nasm_hash: %u\n", cmn_hash % 1009, asm_hash % 1009);

    int i1 = strcmp("CRIME", "CRIME");
    int i2 = avx_strcmp("CRIME", "CRIME");

    printf("i1: %d\ni2: %d\n", i1, i2);
    return 0;
}
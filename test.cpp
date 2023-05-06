
#include <stdio.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    // unsigned int cmn_hash = hash_mrot("CRIME");
    // unsigned int asm_hash = _asm_hash_addmul("CRIME");

    // printf("cmn_hash: %u\nasm_hash: %u\n", cmn_hash % 1009, asm_hash % 1009);

    int i1 = strcmp("sheeesh", "sheeesd");
    int i2 = asm_strcmp("sheeesh", "sheeesd");

    printf("i1: %d\ni2: %d\n", i1, i2);
    return 0;
}
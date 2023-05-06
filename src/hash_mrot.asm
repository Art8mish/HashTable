
section .text
global _Z16_asm_hash_addmulPKc

_Z16_asm_hash_addmulPKc:

        push rbp                ;prologue
        mov rbp, rsp            ;

        mov rax, 33             ;HASH_CNST = 33

        jmp .lchck

.loop:  add rax, rdx            ;hash += *word
        mov rdx, rax            ;
        sal rdx, 5              ;
        add rax, rdx            ;hash *= 33

        inc rdi                 ;word++
        
.lchck: xor rdx, rdx
        mov byte dl, [rdi]
        cmp dl, 0x00
        jne .loop

.end:   pop rbp
        ret


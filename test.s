	.file	"test.cpp"
	.text
	.data
	.align 4
	.type	_ZL8HSH_CNST, @object
	.size	_ZL8HSH_CNST, 4
_ZL8HSH_CNST:
	.long	33
	.section	.rodata
.LC0:
	.string	"[%d] = %lld\n"
	.text
	.globl	_Z9hash_mrotPKc
	.type	_Z9hash_mrotPKc, @function
_Z9hash_mrotPKc:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	_ZL8HSH_CNST(%rip), %eax
	cltq
	movq	%rax, -8(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L2
.L3:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, -8(%rbp)
	movl	_ZL8HSH_CNST(%rip), %eax
	cltq
	movq	-8(%rbp), %rdx
	imulq	%rdx, %rax
	movq	%rax, -8(%rbp)
	addq	$1, -24(%rbp)
	movq	-8(%rbp), %rdx
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -12(%rbp)
.L2:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L3
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z9hash_mrotPKc, .-_Z9hash_mrotPKc
	.section	.text._Z10asm_strcmpPKcS0_,"axG",@progbits,_Z10asm_strcmpPKcS0_,comdat
	.weak	_Z10asm_strcmpPKcS0_
	.type	_Z10asm_strcmpPKcS0_, @function
_Z10asm_strcmpPKcS0_:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movq	%rdi, -32(%rbp)
	movq	%rsi, -40(%rbp)
	movl	$0, -12(%rbp)
	movq	-32(%rbp), %rdx
	movq	-40(%rbp), %rcx
#APP
# 31 "test.cpp" 1
	push r8
	push r9
	.intel_syntax noprefix 
	mov rdi, %rdx            
	mov rsi, %rcx            
	_asm_strcmp_loop:      
	mov byte r8b, [rdi]    
	mov byte r9b, [rsi]    
	cmp byte r8b, 0x0      
	je _asm_strcmp_end     
	cmp byte r9b, 0x0      
	je _asm_strcmp_end     
	cmp byte r8b, r9b      
	jne _asm_strcmp_end    
	inc rdi                
	inc rsi                
	jmp _asm_strcmp_loop   
	_asm_strcmp_end:       
	movzx rax, r8b         
	movzx rbx, r9b         
	sub rax, rbx           
	pop r9
	pop r8
	.att_syntax prefix     
	
# 0 "" 2
#NO_APP
	movl	%edx, -12(%rbp)
	movl	-12(%rbp), %eax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_Z10asm_strcmpPKcS0_, .-_Z10asm_strcmpPKcS0_
	.section	.rodata
.LC1:
	.string	"sheeeshxyi"
.LC2:
	.string	"sheeesh"
.LC3:
	.string	"i1: %d\ni2: %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$-1, -8(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	_Z10asm_strcmpPKcS0_
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:

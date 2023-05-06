	.file	"main.cpp"
	.text
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.local	_ZL17LIST_POISON_VALUE
	.comm	_ZL17LIST_POISON_VALUE,8,8
	.section	.rodata
.LC0:
	.string	"logs/graph.gv"
	.section	.data.rel.ro.local,"aw"
	.align 8
	.type	_ZL10GRAPH_FILE, @object
	.size	_ZL10GRAPH_FILE, 8
_ZL10GRAPH_FILE:
	.quad	.LC0
	.section	.rodata
.LC1:
	.string	"logs/dump_file.txt"
	.section	.data.rel.ro.local
	.align 8
	.type	_ZL9DUMP_FILE, @object
	.size	_ZL9DUMP_FILE, 8
_ZL9DUMP_FILE:
	.quad	.LC1
	.section	.rodata
.LC2:
	.string	"logs/allgraphs.htm"
	.section	.data.rel.ro.local
	.align 8
	.type	_ZL8HTM_FILE, @object
	.size	_ZL8HTM_FILE, 8
_ZL8HTM_FILE:
	.quad	.LC2
	.section	.rodata
	.align 8
	.type	_ZL12MAX_TBL_SIZE, @object
	.size	_ZL12MAX_TBL_SIZE, 8
_ZL12MAX_TBL_SIZE:
	.quad	10000
	.align 4
	.type	_ZL12MAX_WORD_LEN, @object
	.size	_ZL12MAX_WORD_LEN, 4
_ZL12MAX_WORD_LEN:
	.long	10
	.globl	txt_f_path
.LC3:
	.string	"data_txt/crm_pnshmnt.txt"
	.section	.data.rel.local,"aw"
	.align 8
	.type	txt_f_path, @object
	.size	txt_f_path, 8
txt_f_path:
	.quad	.LC3
	.section	.rodata
.LC4:
	.string	"src/main.cpp"
.LC5:
	.string	"hsh_tbl == __null"
	.align 8
.LC6:
	.string	"Error in %s = %d; file: %s; num of line: %d \n"
.LC7:
	.string	"err"
.LC8:
	.string	"asm_hash_mrot"
.LC9:
	.string	""
.LC10:
	.string	"wrd_in_tbl == -1"
.LC12:
	.string	"Word was "
.LC13:
	.string	"found "
.LC14:
	.string	"not found "
.LC15:
	.string	"in %lf seconds (%d cycles)\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6521:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	movl	$1009, %edi
	call	_Z7tblCtorj@PLT
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L2
	cmpq	$0, -32(%rbp)
	sete	%al
	movzbl	%al, %eax
	movl	$13, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rdx, %rcx
	movl	%eax, %edx
	leaq	.LC5(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L2:
	cmpq	$0, -32(%rbp)
	jne	.L3
	movl	$1, %eax
	jmp	.L4
.L3:
	movq	txt_f_path(%rip), %rcx
	movq	-32(%rbp), %rax
	movq	_Z16_asm_hash_addmulPKc@GOTPCREL(%rip), %rdx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_Z11tblHashSortP9HashTablePKcPFyS2_E@PLT
	movl	%eax, -36(%rbp)
	cmpl	$0, -36(%rbp)
	je	.L5
	movl	-36(%rbp), %eax
	movl	$37, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rdx, %rcx
	movl	%eax, %edx
	leaq	.LC7(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L5:
	cmpl	$0, -36(%rbp)
	je	.L6
	movl	$12, %eax
	jmp	.L4
.L6:
	movq	-32(%rbp), %rax
	leaq	.LC8(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_Z10tblCsvDumpP9HashTablePKc@PLT
	movl	%eax, -36(%rbp)
	cmpl	$0, -36(%rbp)
	je	.L7
	movl	-36(%rbp), %eax
	movl	$40, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rdx, %rcx
	movl	%eax, %edx
	leaq	.LC7(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L7:
	cmpl	$0, -36(%rbp)
	je	.L8
	movl	$13, %eax
	jmp	.L4
.L8:
	call	clock@PLT
	movq	%rax, -24(%rbp)
	movl	$0, -44(%rbp)
	movl	$0, -40(%rbp)
	jmp	.L9
.L12:
	movq	-32(%rbp), %rax
	leaq	.LC9(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_Z8WrdInTblP9HashTablePKc@PLT
	movl	%eax, -44(%rbp)
	cmpl	$-1, -44(%rbp)
	jne	.L10
	cmpl	$-1, -44(%rbp)
	sete	%al
	movzbl	%al, %eax
	movl	$49, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rdx, %rcx
	movl	%eax, %edx
	leaq	.LC10(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L10:
	cmpl	$-1, -44(%rbp)
	jne	.L11
	movl	$1, %eax
	jmp	.L4
.L11:
	addl	$1, -40(%rbp)
.L9:
	cmpl	$9999999, -40(%rbp)
	jle	.L12
	call	clock@PLT
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	subq	-24(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC11(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -8(%rbp)
	leaq	.LC12(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$0, -44(%rbp)
	je	.L13
	leaq	.LC13(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L14
.L13:
	leaq	.LC14(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L14:
	movl	-40(%rbp), %edx
	movq	-8(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %xmm0
	leaq	.LC15(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	_Z7tblDtorP9HashTable@PLT
	movl	$0, %eax
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6521:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB7166:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L17
	cmpl	$65535, -8(%rbp)
	jne	.L17
	leaq	_ZStL8__ioinit(%rip), %rax
	movq	%rax, %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	leaq	__dso_handle(%rip), %rax
	movq	%rax, %rdx
	leaq	_ZStL8__ioinit(%rip), %rax
	movq	%rax, %rsi
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rax
	movq	%rax, %rdi
	call	__cxa_atexit@PLT
.L17:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7166:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I_txt_f_path, @function
_GLOBAL__sub_I_txt_f_path:
.LFB7167:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7167:
	.size	_GLOBAL__sub_I_txt_f_path, .-_GLOBAL__sub_I_txt_f_path
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_txt_f_path
	.section	.rodata
	.align 8
.LC11:
	.long	0
	.long	1093567616
	.hidden	__dso_handle
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

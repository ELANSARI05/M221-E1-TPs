	.file	"exercise3.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
	.align 8
.LC4:
	.string	"x\342\220\243=\342\220\243%f,\342\220\243y\342\220\243=\342\220\243%f,\342\220\243time\342\220\243=\342\220\243%f\342\220\243s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	movsd	xmm0, QWORD PTR .LC0[rip]
	movsd	QWORD PTR -32[rbp], xmm0
	movsd	xmm0, QWORD PTR .LC1[rip]
	movsd	QWORD PTR -24[rbp], xmm0
	pxor	xmm0, xmm0
	movsd	QWORD PTR -48[rbp], xmm0
	pxor	xmm0, xmm0
	movsd	QWORD PTR -40[rbp], xmm0
	call	clock@PLT
	mov	QWORD PTR -16[rbp], rax
	mov	DWORD PTR -52[rbp], 0
	jmp	.L2
.L3:
	movsd	xmm0, QWORD PTR -32[rbp]
	mulsd	xmm0, QWORD PTR -24[rbp]
	movsd	xmm1, QWORD PTR -48[rbp]
	addsd	xmm0, xmm1
	movsd	QWORD PTR -48[rbp], xmm0
	movsd	xmm0, QWORD PTR -32[rbp]
	mulsd	xmm0, QWORD PTR -24[rbp]
	movsd	xmm1, QWORD PTR -40[rbp]
	addsd	xmm0, xmm1
	movsd	QWORD PTR -40[rbp], xmm0
	add	DWORD PTR -52[rbp], 1
.L2:
	cmp	DWORD PTR -52[rbp], 99999999
	jle	.L3
	call	clock@PLT
	mov	QWORD PTR -8[rbp], rax
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR -16[rbp]
	pxor	xmm0, xmm0
	cvtsi2sd	xmm0, rax
	movsd	xmm1, QWORD PTR .LC3[rip]
	movapd	xmm2, xmm0
	divsd	xmm2, xmm1
	movsd	xmm0, QWORD PTR -40[rbp]
	mov	rax, QWORD PTR -48[rbp]
	movapd	xmm1, xmm0
	movq	xmm0, rax
	lea	rax, .LC4[rip]
	mov	rdi, rax
	mov	eax, 3
	call	printf@PLT
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	-1717986918
	.long	1072798105
	.align 8
.LC1:
	.long	858993459
	.long	1072902963
	.align 8
.LC3:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
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

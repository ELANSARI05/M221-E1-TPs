	.file	"exercise3.c"
	.intel_syntax noprefix
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"x\342\220\243=\342\220\243%f,\342\220\243y\342\220\243=\342\220\243%f,\342\220\243time\342\220\243=\342\220\243%f\342\220\243s\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB39:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 24
	.cfi_offset 3, -24
	call	clock@PLT
	movsd	xmm0, QWORD PTR .LC1[rip]
	pxor	xmm1, xmm1
	mov	rbx, rax
	mov	eax, 100000000
	.p2align 4,,10
	.p2align 3
.L2:
	addsd	xmm1, xmm0
	addsd	xmm1, xmm0
	sub	eax, 2
	jne	.L2
	movsd	QWORD PTR -24[rbp], xmm1
	call	clock@PLT
	movsd	xmm1, QWORD PTR -24[rbp]
	pxor	xmm2, xmm2
	lea	rsi, .LC3[rip]
	sub	rax, rbx
	mov	edi, 2
	cvtsi2sd	xmm2, rax
	movapd	xmm0, xmm1
	mov	eax, 3
	divsd	xmm2, QWORD PTR .LC2[rip]
	call	__printf_chk@PLT
	add	rsp, 24
	xor	eax, eax
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE39:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	1374389535
	.long	1073028792
	.align 8
.LC2:
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

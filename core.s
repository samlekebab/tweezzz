	.file	"core.cpp"
	.text
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB3011:
	.cfi_startproc
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE3011:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"timer start"
	.text
	.p2align 4
	.globl	_ZN8coreCalc10startTimerEv
	.type	_ZN8coreCalc10startTimerEv, @function
_ZN8coreCalc10startTimerEv:
.LFB6826:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	_ZSt4cout(%rip), %rbp
	movl	$11, %edx
	leaq	.LC0(%rip), %rsi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rbp, %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	_ZSt4cout(%rip), %rax
	movq	-24(%rax), %rax
	movq	240(%rbp,%rax), %rbx
	testq	%rbx, %rbx
	je	.L9
	cmpb	$0, 56(%rbx)
	je	.L5
	movsbl	67(%rbx), %esi
.L6:
	movq	%rbp, %rdi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	movq	%rax, _ZN8coreCalc5timerE(%rip)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	.cfi_restore_state
	movq	%rbx, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	(%rbx), %rax
	movl	$10, %esi
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rdx
	movq	48(%rax), %rax
	cmpq	%rdx, %rax
	je	.L6
	movq	%rbx, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	.L6
.L9:
	call	_ZSt16__throw_bad_castv@PLT
	.cfi_endproc
.LFE6826:
	.size	_ZN8coreCalc10startTimerEv, .-_ZN8coreCalc10startTimerEv
	.p2align 4
	.globl	_ZN8coreCalc8getTimerENSt6chrono10time_pointINS0_3_V212system_clockENS0_8durationIlSt5ratioILl1ELl1000000000EEEEEE
	.type	_ZN8coreCalc8getTimerENSt6chrono10time_pointINS0_3_V212system_clockENS0_8durationIlSt5ratioILl1ELl1000000000EEEEEE, @function
_ZN8coreCalc8getTimerENSt6chrono10time_pointINS0_3_V212system_clockENS0_8durationIlSt5ratioILl1ELl1000000000EEEEEE:
.LFB6827:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	movabsq	$2361183241434822607, %rdx
	subq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	movq	%rax, %rcx
	imulq	%rdx
	sarq	$63, %rcx
	sarq	$7, %rdx
	movq	%rdx, %rax
	subq	%rcx, %rax
	ret
	.cfi_endproc
.LFE6827:
	.size	_ZN8coreCalc8getTimerENSt6chrono10time_pointINS0_3_V212system_clockENS0_8durationIlSt5ratioILl1ELl1000000000EEEEEE, .-_ZN8coreCalc8getTimerENSt6chrono10time_pointINS0_3_V212system_clockENS0_8durationIlSt5ratioILl1ELl1000000000EEEEEE
	.p2align 4
	.globl	_ZN8coreCalc21getCurrentCardSegmentEv
	.type	_ZN8coreCalc21getCurrentCardSegmentEv, @function
_ZN8coreCalc21getCurrentCardSegmentEv:
.LFB6828:
	.cfi_startproc
	movq	_ZN8coreCalc6curSegE(%rip), %rax
	leaq	1(%rax), %rcx
	movabsq	$4835703278458516699, %rax
	imulq	%rcx
	movq	%rdx, %rax
	movq	%rcx, %rdx
	sarq	$63, %rdx
	sarq	$17, %rax
	subq	%rdx, %rax
	imulq	$500000, %rax, %rdx
	movq	%rcx, %rax
	subq	%rdx, %rax
	movq	%rax, _ZN8coreCalc6curSegE(%rip)
	ret
	.cfi_endproc
.LFE6828:
	.size	_ZN8coreCalc21getCurrentCardSegmentEv, .-_ZN8coreCalc21getCurrentCardSegmentEv
	.p2align 4
	.globl	_ZN8coreCalc6sinLutEd
	.type	_ZN8coreCalc6sinLutEd, @function
_ZN8coreCalc6sinLutEd:
.LFB6831:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movapd	%xmm0, %xmm2
	movapd	%xmm0, %xmm1
	leaq	_ZN8coreCalc9sinLut_tbE(%rip), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	divsd	.LC3(%rip), %xmm2
	cvttsd2siq	%xmm2, %rax
	pxor	%xmm2, %xmm2
	divsd	.LC1(%rip), %xmm1
	mulsd	.LC2(%rip), %xmm1
	cvtsi2sdq	%rax, %xmm2
	subsd	%xmm2, %xmm1
	mulsd	.LC4(%rip), %xmm1
	cvttsd2sil	%xmm1, %ebx
	movslq	%ebx, %rbx
	cmpb	$0, 0(%rbp,%rbx)
	je	.L14
	leaq	_ZN8coreCalc8sinLut_tE(%rip), %rax
	movsd	(%rax,%rbx,8), %xmm0
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L14:
	.cfi_restore_state
	call	sin@PLT
	leaq	_ZN8coreCalc8sinLut_tE(%rip), %rax
	movb	$1, 0(%rbp,%rbx)
	movsd	%xmm0, (%rax,%rbx,8)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6831:
	.size	_ZN8coreCalc6sinLutEd, .-_ZN8coreCalc6sinLutEd
	.p2align 4
	.globl	_ZN8coreCalc17calculate_tweezerER5Aom1DR5Aom2DilPs
	.type	_ZN8coreCalc17calculate_tweezerER5Aom1DR5Aom2DilPs, @function
_ZN8coreCalc17calculate_tweezerER5Aom1DR5Aom2DilPs:
.LFB6832:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movslq	%edx, %rdx
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%r8, %rbx
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	movq	(%rdi), %rax
	movq	(%rax,%rdx,8), %rax
	movsd	16(%rax), %xmm7
	movsd	(%rax), %xmm0
	mulsd	8(%rax), %xmm0
	movsd	%xmm7, 32(%rsp)
	movsd	24(%rax), %xmm7
	movsd	%xmm7, 40(%rsp)
	pxor	%xmm7, %xmm7
	cvtsi2sdl	8(%rdi), %xmm7
	movsd	%xmm0, (%rsp)
	movsd	%xmm7, 8(%rsp)
	movsd	16(%rdi), %xmm7
	movsd	%xmm7, 16(%rsp)
	movsd	24(%rdi), %xmm7
	movsd	%xmm7, 24(%rsp)
	.p2align 4,,10
	.p2align 3
.L18:
	pxor	%xmm0, %xmm0
	cvtsi2sdl	%ebp, %xmm0
	addsd	.LC5(%rip), %xmm0
	mulsd	32(%rsp), %xmm0
	divsd	.LC6(%rip), %xmm0
	addsd	40(%rsp), %xmm0
	call	sin@PLT
	mulsd	(%rsp), %xmm0
	movsd	8(%rsp), %xmm1
	mulsd	.LC7(%rip), %xmm0
	divsd	%xmm1, %xmm0
	mulsd	16(%rsp), %xmm0
	mulsd	24(%rsp), %xmm0
	divsd	%xmm1, %xmm0
	cvttsd2sil	%xmm0, %eax
	movw	%ax, (%rbx,%rbp,2)
	addq	$1, %rbp
	cmpq	$96, %rbp
	jne	.L18
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6832:
	.size	_ZN8coreCalc17calculate_tweezerER5Aom1DR5Aom2DilPs, .-_ZN8coreCalc17calculate_tweezerER5Aom1DR5Aom2DilPs
	.p2align 4
	.globl	_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs
	.type	_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs, @function
_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs:
.LFB6833:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rcx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movl	8(%rdi), %r13d
	testl	%r13d, %r13d
	jle	.L24
	pxor	%xmm4, %xmm4
	movq	(%rdi), %r14
	movslq	%r13d, %rax
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rdx, %xmm4
	leaq	(%r14,%rax,8), %rbx
	movq	%xmm4, %r15
	.p2align 4,,10
	.p2align 3
.L23:
	movq	(%r14), %rax
	movq	%r15, %xmm3
	movsd	%xmm1, 8(%rsp)
	addq	$8, %r14
	mulsd	16(%rax), %xmm3
	movsd	(%rax), %xmm0
	mulsd	8(%rax), %xmm0
	movsd	%xmm0, (%rsp)
	movapd	%xmm3, %xmm0
	divsd	.LC6(%rip), %xmm0
	addsd	24(%rax), %xmm0
	call	_ZN8coreCalc6sinLutEd
	mulsd	(%rsp), %xmm0
	movsd	8(%rsp), %xmm1
	addsd	%xmm0, %xmm1
	cmpq	%r14, %rbx
	jne	.L23
	mulsd	.LC7(%rip), %xmm1
.L22:
	pxor	%xmm0, %xmm0
	cvtsi2sdl	%r13d, %xmm0
	divsd	%xmm0, %xmm1
	mulsd	16(%rbp), %xmm1
	mulsd	24(%rbp), %xmm1
	divsd	%xmm0, %xmm1
	cvttsd2sil	%xmm1, %eax
	movw	%ax, (%r12)
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L24:
	.cfi_restore_state
	pxor	%xmm1, %xmm1
	jmp	.L22
	.cfi_endproc
.LFE6833:
	.size	_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs, .-_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs
	.p2align 4
	.globl	_ZN8coreCalc16calculateSegmentER9SchedulerR5Aom1DR5Aom2DlPs
	.type	_ZN8coreCalc16calculateSegmentER9SchedulerR5Aom1DR5Aom2DlPs, @function
_ZN8coreCalc16calculateSegmentER9SchedulerR5Aom1DR5Aom2DlPs:
.LFB6830:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rsi, %r13
	movq	%rcx, %rsi
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%r8, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rcx, %rbx
	leaq	96(%rbx), %r14
	call	_ZN9Scheduler13computeSampleEl@PLT
	.p2align 4,,10
	.p2align 3
.L28:
	movq	%rbp, %rcx
	movq	%rbx, %rdx
	movq	%r12, %rsi
	movq	%r13, %rdi
	call	_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs
	addq	$1, %rbx
	addq	$2, %rbp
	cmpq	%r14, %rbx
	jne	.L28
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6830:
	.size	_ZN8coreCalc16calculateSegmentER9SchedulerR5Aom1DR5Aom2DlPs, .-_ZN8coreCalc16calculateSegmentER9SchedulerR5Aom1DR5Aom2DlPs
	.section	.rodata.str1.1
.LC9:
	.string	"a "
.LC10:
	.string	"w"
.LC11:
	.string	"./res/cpp.txt"
.LC12:
	.string	"%d\n"
.LC13:
	.string	"surprise : "
.LC14:
	.string	"count "
	.section	.text.unlikely,"ax",@progbits
.LCOLDB15:
	.text
.LHOTB15:
	.p2align 4
	.globl	_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D
	.type	_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D, @function
_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D:
.LFB6829:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA6829
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rsi, %r15
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$312, %rsp
	.cfi_def_cfa_offset 368
	movq	%rdi, 24(%rsp)
	movq	%rdx, 88(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 296(%rsp)
	xorl	%eax, %eax
.LEHB0:
	call	_ZN8coreCalc10startTimerEv
	movl	$2, %edx
	leaq	.LC9(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	_ZN8coreCalc5timerE(%rip), %rbx
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	leaq	_ZSt4cout(%rip), %rdi
	movabsq	$2361183241434822607, %rdx
	subq	%rbx, %rax
	movq	%rax, %rcx
	imulq	%rdx
	sarq	$63, %rcx
	sarq	$7, %rdx
	movq	%rdx, %rsi
	subq	%rcx, %rsi
	call	_ZNSo9_M_insertIlEERSoT_@PLT
	movq	%rax, %rbx
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbx,%rax), %rbp
	testq	%rbp, %rbp
	je	.L72
	cmpb	$0, 56(%rbp)
	je	.L34
	movsbl	67(%rbp), %esi
.L35:
	movq	%rbx, %rdi
	leaq	.LC12(%rip), %r14
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	leaq	.LC10(%rip), %rsi
	leaq	.LC11(%rip), %rdi
	call	fopen@PLT
.LEHE0:
	movq	$0, 72(%rsp)
	movq	$0, 80(%rsp)
	movq	%rax, %r12
	movq	24(%rsp), %rax
	movq	$0, 8(%rsp)
	addq	$80048, %rax
	movq	%rax, 16(%rsp)
	jmp	.L54
	.p2align 4,,10
	.p2align 3
.L39:
	movq	_ZN8coreCalc5timerE(%rip), %rbx
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	subq	%rbx, %rax
	cmpq	$40000999, %rax
	jg	.L73
	movq	16(%rsp), %rdi
	call	pthread_mutex_unlock@PLT
.L54:
	movq	_ZN8coreCalc6curSegE(%rip), %rax
	movq	8(%rsp), %rsi
	leaq	1(%rax), %rcx
	movq	%rsi, %r8
	movabsq	$4835703278458516699, %rax
	imulq	%rcx
	movq	%rcx, %rax
	sarq	$63, %r8
	sarq	$63, %rax
	sarq	$17, %rdx
	movq	%rdx, %rdi
	subq	%rax, %rdi
	imulq	$500000, %rdi, %rax
	subq	%rax, %rcx
	movabsq	$-5551535331153507085, %rax
	imulq	%rsi
	movq	%rcx, %rdi
	movq	%rcx, _ZN8coreCalc6curSegE(%rip)
	movabsq	$3074457345618258603, %rax
	leaq	(%rdx,%rsi), %rcx
	imulq	%rsi
	sarq	$25, %rcx
	movabsq	$4835703278458516699, %rax
	subq	%r8, %rcx
	sarq	$4, %rdx
	movq	%rdx, %rsi
	subq	%r8, %rsi
	imulq	%rsi
	movq	%rsi, %rax
	sarq	$63, %rax
	sarq	$17, %rdx
	subq	%rax, %rdx
	imulq	$500000, %rdx, %rax
	subq	%rax, %rsi
	xorl	%eax, %eax
	cmpl	%edi, %esi
	setg	%al
	addq	%rax, %rcx
	imulq	$500000, %rcx, %rcx
	addq	%rdi, %rcx
	movq	16(%rsp), %rdi
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	movq	%rax, 40(%rsp)
	movq	%rax, 8(%rsp)
	call	pthread_mutex_lock@PLT
	testl	%eax, %eax
	jne	.L74
	movq	24(%rsp), %rax
	movq	8(%rsp), %rdi
	movq	80088(%rax), %rax
	leaq	300000(%rdi), %rcx
	cmpq	%rax, %rcx
	cmovl	%rax, %rcx
	movabsq	$3074457345618258603, %rax
	imulq	%rcx
	sarq	$63, %rcx
	sarq	$4, %rdx
	subq	%rcx, %rdx
	leal	(%rdx,%rdx,2), %eax
	sall	$5, %eax
	movslq	%eax, %rcx
	leaq	959999(%rdi), %rax
	movq	%rcx, 32(%rsp)
	cmpq	%rax, %rcx
	jg	.L39
	leaq	96(%rsp), %rbx
	movq	32(%rsp), %r13
	movl	$24, %ecx
	xorl	%eax, %eax
	movq	%rbx, %rdi
	movq	%rbx, 56(%rsp)
	rep stosq
	movq	24(%rsp), %rdi
	movq	%r13, %rsi
.LEHB1:
	call	_ZN9Scheduler13computeSampleEl@PLT
	movq	%rbx, 48(%rsp)
	leaq	288(%rsp), %rbp
	movq	%r12, 64(%rsp)
	movq	%r13, %r12
	movq	88(%rsp), %r13
	.p2align 4,,10
	.p2align 3
.L40:
	movq	%rbx, %rcx
	movq	%r12, %rdx
	movq	%r13, %rsi
	movq	%r15, %rdi
	call	_ZN8coreCalc14calculate_tickER5Aom1DR5Aom2DlRs
	addq	$2, %rbx
	addq	$1, %r12
	cmpq	%rbx, %rbp
	jne	.L40
	movq	32(%rsp), %rbx
	movq	24(%rsp), %rax
	movq	64(%rsp), %r12
	addq	$96, %rbx
	movq	%rbx, 80088(%rax)
	movq	56(%rsp), %rbx
	.p2align 4,,10
	.p2align 3
.L41:
	movswl	(%rbx), %edx
	movq	%r14, %rsi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf@PLT
	addq	$2, %rbx
	cmpq	%rbp, %rbx
	jne	.L41
	movq	%r12, %rdi
	call	fflush@PLT
	movq	48(%rsp), %rax
	pxor	%xmm0, %xmm0
	pxor	%xmm6, %xmm6
	pxor	%xmm5, %xmm5
	.p2align 4,,10
	.p2align 3
.L42:
	movdqa	(%rax), %xmm2
	movdqa	%xmm6, %xmm1
	movdqa	%xmm5, %xmm4
	addq	$16, %rax
	pcmpgtw	%xmm2, %xmm1
	movdqa	%xmm2, %xmm3
	punpcklwd	%xmm1, %xmm3
	punpckhwd	%xmm1, %xmm2
	pcmpgtd	%xmm3, %xmm4
	movdqa	%xmm3, %xmm1
	punpckldq	%xmm4, %xmm1
	punpckhdq	%xmm4, %xmm3
	paddq	%xmm1, %xmm0
	movdqa	%xmm5, %xmm1
	pcmpgtd	%xmm2, %xmm1
	paddq	%xmm3, %xmm0
	movdqa	%xmm2, %xmm3
	punpckldq	%xmm1, %xmm3
	punpckhdq	%xmm1, %xmm2
	paddq	%xmm3, %xmm0
	paddq	%xmm2, %xmm0
	cmpq	%rax, %rbp
	jne	.L42
	movdqa	%xmm0, %xmm1
	movq	40(%rsp), %rcx
	movabsq	$4781426665036160, %rdx
	addq	$96, 72(%rsp)
	psrldq	$8, %xmm1
	paddq	%xmm1, %xmm0
	movq	%xmm0, %rax
	addq	%rax, 80(%rsp)
	movabsq	$4150278345251397305, %rax
	imulq	%rax, %rcx
	movq	%rcx, %rax
	addq	%rdx, %rax
	movabsq	$149419583282380, %rdx
	rorq	$6, %rax
	cmpq	%rax, %rdx
	jb	.L39
	movl	$11, %edx
	leaq	.LC13(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	80(%rsp), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZNSo9_M_insertIlEERSoT_@PLT
	movq	%rax, %rbx
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbx,%rax), %rbp
	testq	%rbp, %rbp
	je	.L75
	cmpb	$0, 56(%rbp)
	je	.L46
	movsbl	67(%rbp), %esi
	jmp	.L47
.L73:
	call	_ZN8coreCalc10startTimerEv
	movl	$6, %edx
	leaq	.LC14(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	72(%rsp), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZNSo9_M_insertIlEERSoT_@PLT
	movq	%rax, %rbx
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbx,%rax), %rbp
	testq	%rbp, %rbp
	je	.L76
	cmpb	$0, 56(%rbp)
	je	.L51
	movsbl	67(%rbp), %esi
.L52:
	movq	%rbx, %rdi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	movq	296(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L71
	movq	16(%rsp), %rdi
	addq	$312, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	pthread_mutex_unlock@PLT
.L46:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	0(%rbp), %rax
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rdx
	movl	$10, %esi
	movq	48(%rax), %rax
	cmpq	%rdx, %rax
	jne	.L77
.L47:
	movq	%rbx, %rdi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
.LEHE1:
	jmp	.L39
.L34:
	movq	%rbp, %rdi
.LEHB2:
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	0(%rbp), %rax
	movl	$10, %esi
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rdx
	movq	48(%rax), %rax
	cmpq	%rdx, %rax
	je	.L35
	movq	%rbp, %rdi
	call	*%rax
.LEHE2:
	movsbl	%al, %esi
	jmp	.L35
.L51:
	movq	%rbp, %rdi
.LEHB3:
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	0(%rbp), %rax
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rdx
	movl	$10, %esi
	movq	48(%rax), %rax
	cmpq	%rdx, %rax
	je	.L52
	movq	%rbp, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	.L52
.L77:
	movq	%rbp, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	.L47
.L71:
	call	__stack_chk_fail@PLT
.L75:
	movq	296(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L71
	call	_ZSt16__throw_bad_castv@PLT
.LEHE3:
	.p2align 4,,10
	.p2align 3
.L74:
	movq	296(%rsp), %rdx
	subq	%fs:40, %rdx
	jne	.L71
	movl	%eax, %edi
.LEHB4:
	call	_ZSt20__throw_system_errori@PLT
.LEHE4:
.L76:
	movq	296(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L71
.LEHB5:
	call	_ZSt16__throw_bad_castv@PLT
.LEHE5:
.L72:
	movq	296(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L71
.LEHB6:
	call	_ZSt16__throw_bad_castv@PLT
.LEHE6:
.L62:
	movq	%rax, %rbx
	jmp	.L56
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA6829:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6829-.LLSDACSB6829
.LLSDACSB6829:
	.uleb128 .LEHB0-.LFB6829
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB6829
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L62-.LFB6829
	.uleb128 0
	.uleb128 .LEHB2-.LFB6829
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB3-.LFB6829
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L62-.LFB6829
	.uleb128 0
	.uleb128 .LEHB4-.LFB6829
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB5-.LFB6829
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L62-.LFB6829
	.uleb128 0
	.uleb128 .LEHB6-.LFB6829
	.uleb128 .LEHE6-.LEHB6
	.uleb128 0
	.uleb128 0
.LLSDACSE6829:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC6829
	.type	_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D.cold, @function
_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D.cold:
.LFSB6829:
.L56:
	.cfi_def_cfa_offset 368
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	16(%rsp), %rdi
	call	pthread_mutex_unlock@PLT
	movq	296(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L78
	movq	%rbx, %rdi
.LEHB7:
	call	_Unwind_Resume@PLT
.LEHE7:
.L78:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6829:
	.section	.gcc_except_table
.LLSDAC6829:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC6829-.LLSDACSBC6829
.LLSDACSBC6829:
	.uleb128 .LEHB7-.LCOLDB15
	.uleb128 .LEHE7-.LEHB7
	.uleb128 0
	.uleb128 0
.LLSDACSEC6829:
	.section	.text.unlikely
	.text
	.size	_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D, .-_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D
	.section	.text.unlikely
	.size	_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D.cold, .-_ZN8coreCalc9startCoreER9SchedulerR5Aom1DR5Aom2D.cold
.LCOLDE15:
	.text
.LHOTE15:
	.p2align 4
	.globl	_ZN8coreCalc27findTickAssociatedToSegmentEil
	.type	_ZN8coreCalc27findTickAssociatedToSegmentEil, @function
_ZN8coreCalc27findTickAssociatedToSegmentEil:
.LFB6835:
	.cfi_startproc
	movabsq	$-5551535331153507085, %rax
	movq	%rsi, %r8
	imulq	%rsi
	sarq	$63, %r8
	movabsq	$3074457345618258603, %rax
	leaq	(%rdx,%rsi), %rcx
	imulq	%rsi
	sarq	$25, %rcx
	movabsq	$4835703278458516699, %rax
	subq	%r8, %rcx
	movq	%rdx, %rsi
	sarq	$4, %rsi
	subq	%r8, %rsi
	imulq	%rsi
	movq	%rsi, %rax
	sarq	$63, %rax
	sarq	$17, %rdx
	subq	%rax, %rdx
	xorl	%eax, %eax
	imulq	$500000, %rdx, %rdx
	subq	%rdx, %rsi
	cmpl	%esi, %edi
	movslq	%edi, %rdi
	setl	%al
	addq	%rax, %rcx
	imulq	$500000, %rcx, %rcx
	addq	%rdi, %rcx
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	ret
	.cfi_endproc
.LFE6835:
	.size	_ZN8coreCalc27findTickAssociatedToSegmentEil, .-_ZN8coreCalc27findTickAssociatedToSegmentEil
	.globl	_ZN8coreCalc1cE
	.bss
	.align 4
	.type	_ZN8coreCalc1cE, @object
	.size	_ZN8coreCalc1cE, 4
_ZN8coreCalc1cE:
	.zero	4
	.globl	_ZN8coreCalc9sinLut_tbE
	.align 32
	.type	_ZN8coreCalc9sinLut_tbE, @object
	.size	_ZN8coreCalc9sinLut_tbE, 10000
_ZN8coreCalc9sinLut_tbE:
	.zero	10000
	.globl	_ZN8coreCalc8sinLut_tE
	.align 32
	.type	_ZN8coreCalc8sinLut_tE, @object
	.size	_ZN8coreCalc8sinLut_tE, 80000
_ZN8coreCalc8sinLut_tE:
	.zero	80000
	.globl	_ZN8coreCalc6curSegE
	.align 8
	.type	_ZN8coreCalc6curSegE, @object
	.size	_ZN8coreCalc6curSegE, 8
_ZN8coreCalc6curSegE:
	.zero	8
	.globl	_ZN8coreCalc5timerE
	.align 8
	.type	_ZN8coreCalc5timerE, @object
	.size	_ZN8coreCalc5timerE, 8
_ZN8coreCalc5timerE:
	.zero	8
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	1413754136
	.long	1074340347
	.align 8
.LC2:
	.long	0
	.long	1071644672
	.align 8
.LC3:
	.long	1413754136
	.long	1075388923
	.align 8
.LC4:
	.long	0
	.long	1086556160
	.align 8
.LC5:
	.long	0
	.long	1079508992
	.align 8
.LC6:
	.long	0
	.long	1103225251
	.align 8
.LC7:
	.long	0
	.long	1088421824
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (GNU) 13.1.1 20230429"
	.section	.note.GNU-stack,"",@progbits

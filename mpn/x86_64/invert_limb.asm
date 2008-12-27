dnl  AMD64 mpn_invert_limb -- Invert a normalized limb.

dnl  Copyright 2004, 2007, 2008 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.

dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 3 of the License, or (at
dnl  your option) any later version.

dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

include(`../config.m4')


C	     cycles/limb (approx)	div
C K8:		 40			 71
C P4:		141			161
C P6-15 (Core2): 63			116
C P6-28 (Atom): 130			191

C rax rcx rdx rdi rsi r8


ASM_START()
	TEXT
	ALIGN(16)
PROLOGUE(mpn_invert_limb)
	mov	%rdi, %rax
	shr	$55, %rax
ifdef(`PIC',`
	lea	-512+approx_tab(%rip), %r8
',`
	movabs	$-512+approx_tab, %r8
')
	movzwl	(%r8,%rax,2), R32(%rcx)
	mov	%rdi, %rsi
	mov	R32(%rcx), R32(%rax)
	imul	R32(%rcx), R32(%rcx)
	shr	$32, %rsi
	imul	%rsi, %rcx
	shr	$31, %rcx
	sal	$17, %rax
	sub	%rcx, %rax
	mov	%rax, %r8
	imul	%rax, %rax
	sal	$33, %r8
	mul	%rdi
	neg	%rdx
	lea	(%r8,%rdx,2), %rax
	mov	%rax, %r8
	mul	%rax
	mov	%rax, %rcx
	mov	%rdx, %rax
	mul	%rdi
	mov	%rax, %rsi
	mov	%rcx, %rax
	mov	%rdx, %rcx
	mul	%rdi
	add	%rdx, %rsi
	sbb	%rcx, %r8
	shr	$62, %rsi
	add	$1, %rsi
	sal	$2, %r8
	sub	%rsi, %r8
	mov	%rdi, %rax
	mul	%r8
	add	%rdi, %rax		C xl += d
	adc	%rdi, %rdx		C xh += d
	mov	%r8, %rax
	sub	%rdx, %rax		C return zh - xh
	ret
EPILOGUE()

	.section	.rodata
	.type	approx_tab, @object
	.size	approx_tab, 512
	ALIGN(2)
approx_tab:
	.value	0xffc0,0xfec0,0xfdc0,0xfcc0,0xfbc0,0xfac0,0xfa00,0xf900
	.value	0xf800,0xf700,0xf640,0xf540,0xf440,0xf380,0xf280,0xf180
	.value	0xf0c0,0xefc0,0xef00,0xee00,0xed40,0xec40,0xeb80,0xeac0
	.value	0xe9c0,0xe900,0xe840,0xe740,0xe680,0xe5c0,0xe500,0xe400
	.value	0xe340,0xe280,0xe1c0,0xe100,0xe040,0xdf80,0xdec0,0xde00
	.value	0xdd40,0xdc80,0xdbc0,0xdb00,0xda40,0xd980,0xd8c0,0xd800
	.value	0xd740,0xd680,0xd600,0xd540,0xd480,0xd3c0,0xd340,0xd280
	.value	0xd1c0,0xd140,0xd080,0xcfc0,0xcf40,0xce80,0xcdc0,0xcd40
	.value	0xcc80,0xcc00,0xcb40,0xcac0,0xca00,0xc980,0xc8c0,0xc840
	.value	0xc780,0xc700,0xc640,0xc5c0,0xc540,0xc480,0xc400,0xc380
	.value	0xc2c0,0xc240,0xc1c0,0xc100,0xc080,0xc000,0xbf80,0xbec0
	.value	0xbe40,0xbdc0,0xbd40,0xbc80,0xbc00,0xbb80,0xbb00,0xba80
	.value	0xba00,0xb980,0xb900,0xb840,0xb7c0,0xb740,0xb6c0,0xb640
	.value	0xb5c0,0xb540,0xb4c0,0xb440,0xb3c0,0xb340,0xb2c0,0xb240
	.value	0xb1c0,0xb140,0xb0c0,0xb080,0xb000,0xaf80,0xaf00,0xae80
	.value	0xae00,0xad80,0xad40,0xacc0,0xac40,0xabc0,0xab40,0xaac0
	.value	0xaa80,0xaa00,0xa980,0xa900,0xa8c0,0xa840,0xa7c0,0xa740
	.value	0xa700,0xa680,0xa600,0xa5c0,0xa540,0xa4c0,0xa480,0xa400
	.value	0xa380,0xa340,0xa2c0,0xa240,0xa200,0xa180,0xa140,0xa0c0
	.value	0xa080,0xa000,0x9f80,0x9f40,0x9ec0,0x9e80,0x9e00,0x9dc0
	.value	0x9d40,0x9d00,0x9c80,0x9c40,0x9bc0,0x9b80,0x9b00,0x9ac0
	.value	0x9a40,0x9a00,0x9980,0x9940,0x98c0,0x9880,0x9840,0x97c0
	.value	0x9780,0x9700,0x96c0,0x9680,0x9600,0x95c0,0x9580,0x9500
	.value	0x94c0,0x9440,0x9400,0x93c0,0x9340,0x9300,0x92c0,0x9240
	.value	0x9200,0x91c0,0x9180,0x9100,0x90c0,0x9080,0x9000,0x8fc0
	.value	0x8f80,0x8f40,0x8ec0,0x8e80,0x8e40,0x8e00,0x8d80,0x8d40
	.value	0x8d00,0x8cc0,0x8c80,0x8c00,0x8bc0,0x8b80,0x8b40,0x8b00
	.value	0x8a80,0x8a40,0x8a00,0x89c0,0x8980,0x8940,0x88c0,0x8880
	.value	0x8840,0x8800,0x87c0,0x8780,0x8740,0x8700,0x8680,0x8640
	.value	0x8600,0x85c0,0x8580,0x8540,0x8500,0x84c0,0x8480,0x8440
	.value	0x8400,0x8380,0x8340,0x8300,0x82c0,0x8280,0x8240,0x8200
	.value	0x81c0,0x8180,0x8140,0x8100,0x80c0,0x8080,0x8040,0x8000

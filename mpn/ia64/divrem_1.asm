dnl  IA-64 mpn_divrem_1 -- Divide an mpn number by an unnormalized limb.

dnl  Copyright 2002 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.

dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
dnl  the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
dnl  MA 02111-1307, USA.

include(`../config.m4')

C INPUT PARAMETERS
C rp  = r32
C qxn = r33
C up  = r34
C n   = r35
C vl  = r36

C         cycles/limb
C Itanium:    43
C Itanium 2:  31

C This was generated by gcc, then the loops were optimized.  Lots of things
C outside the loops could be streamlined.  It would probably be a good idea to
C merge the loops for normalized and unnormalized divisor, since the shifting
C stuff is done for free in parallel with other operations.  It would even be
C possible to merge all four loops, if the ld8 were made conditional.

ASM_START()

C HP's assembler requires these declarations for importing mpn_invert_limb
	.global	mpn_invert_limb
	.type	mpn_invert_limb,@function

PROLOGUE(mpn_divrem_1)
	.prologue
	.save	ar.pfs, r42
	alloc		r42 = ar.pfs, 5, 8, 1, 0
	.save	ar.lc, r44
	mov		r44 = ar.lc
	.save	rp, r41
	mov		r41 = b0
	.body
ifdef(`HAVE_ABI_32',
`	addp4		r32 = 0, r32
	sxt4		r33 = r33
	addp4		r34 = 0, r34
	sxt4		r35 = r35
	;;
')
	mov		r38 = r0
	add		r15 = r35, r33		;;
	cmp.ne		p6, p7 = 0, r15		;;
   (p7)	mov		r8 = r0
   (p7)	br.cond.dpnt	.L159
	shladd		r14 = r15, 3, r32
	cmp.le		p6, p7 = r0, r36	;;
	adds		r32 = -8, r14
   (p6)	br.cond.dpnt	.L177
	cmp.eq		p6, p7 = 0, r35
   (p6)	br.cond.dpnt	.L179
	shladd		r14 = r35, 3, r34	;;
	adds		r14 = -8, r14
	adds		r35 = -1, r35		;;
	ld8		r38 = [r14]		;;
	cmp.leu		p6, p7 = r36, r38	;;
   (p6)	addl		r15 = 1, r0
   (p7)	mov		r15 = r0		;;
	sub		r14 = r0, r15
	st8		[r32] = r15, -8		;;
	and		r14 = r14, r36		;;
	sub		r38 = r38, r14
.L179:
	mov		r45 = r36
	adds		r35 = -1, r35
	br.call.sptk.many b0 = mpn_invert_limb#
	;;
	mov		r23 = 1			;;
	setf.sig	f6 = r8
	setf.sig	f12 = r23
	cmp.le		p6, p7 = r0, r35
   (p7)	br.cond.dpnt	.L527
	shladd		r34 = r35, 3, r34
	setf.sig	f10 = r36
	mov		ar.lc = r35
	setf.sig	f7 = r38		;;
.Loop1:		C 00			C q=r18 nh=r38/f7
	ld8		r20 = [r34], -8
	xma.hu		f11 = f7, f6, f0
	;;	C 04
	xma.l		f8 = f11, f12, f7	C q = q + nh
	;;	C 08
	getf.sig	r18 = f8
	xma.hu		f9 = f8, f10, f0
	xma.l		f8 = f8, f10, f0
	;;	C 12
	getf.sig	r16 = f9
		C 13
	getf.sig	r15 = f8
	;;	C 18
	cmp.ltu		p6, p7 = r20, r15
	sub		r15 = r20, r15
	sub		r16 = r38, r16
	;;	C 19
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0?
   (p6)	add		r16 = -1, r16
   (p0)	cmp.ne.unc	p6, p7 = r0, r0
	;;	C 20
   (p8)	cmp.ltu		p6, p7 = r15, r36
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;	C 21
	.pred.rel "mutex",p6,p7
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0 still?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0 still?
   (p6)	add		r16 = -1, r16		C propagate carry into rH
	;;	C 22
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;	C 23
	cmp.ltu		p6, p7 = r15, r36
	;;	C 24
   (p7)	sub		r15 = r15, r36
   (p7)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;	C 25
	setf.sig	f7 = r15
	st8		[r32] = r18, -8
	mov		r38 = r15
	br.cloop.dptk	.Loop1
		C 31

.L527:
	adds		r35 = -1, r33
	;;
	cmp.le		p6, p7 = r0, r35
   (p7)	br.cond.dpnt	.L528
	setf.sig	f7 = r38
	setf.sig	f10 = r36
	mov		ar.lc = r35
	;;
.Loop2:
	xma.hu		f11 = f7, f6, f0
	;;
	xma.l		f8 = f11, f12, f7	C q = q + nh
	;;
	getf.sig	r18 = f8
	xma.hu		f9 = f8, f10, f0
	xma.l		f8 = f8, f10, f0
	;;
	getf.sig	r16 = f9
	getf.sig	r15 = f8
	;;
	cmp.ltu		p6, p7 = 0, r15
	sub		r15 = 0, r15
	sub		r16 = r38, r16
	;;
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0?
   (p6)	add		r16 = -1, r16
   (p0)	cmp.ne.unc	p6, p7 = r0, r0
	;;
   (p8)	cmp.ltu		p6, p7 = r15, r36
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	.pred.rel "mutex",p6,p7
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0 still?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0 still?
   (p6)	add		r16 = -1, r16		C propagate carry into rH
	;;
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	cmp.ltu		p6, p7 = r15, r36
	;;
   (p7)	sub		r15 = r15, r36
   (p7)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	setf.sig	f7 = r15
	st8		[r32] = r18, -8
	mov		r38 = r15
	br.cloop.dptk	.Loop2
	;;
.L528:
	mov		r8 = r38
	br		.L159
	;;
.L177:
	cmp.eq		p6, p7 = 0, r35
   (p6)	br.cond.dpnt	.L322
	shladd		r14 = r35, 3, r34	;;
	adds		r14 = -8, r14		;;
	ld8		r19 = [r14]		;;
	cmp.leu		p6, p7 = r36, r19
   (p6)	br.cond.dptk	.L322
	mov		r38 = r19		;;
	cmp.ne		p6, p7 = 1, r15
	st8		[r32] = r0, -8		;;
   (p7)	mov		r8 = r38
   (p7)	br.cond.dpnt	.L159
	adds		r35 = -1, r35
.L322:
	mux1		r15 = r36, @rev		;;
	sub		r14 = r0, r15		;;
	or		r14 = r15, r14		;;
	czx1.l		r14 = r14		;;
	adds		r14 = -1, r14		;;
	shladd		r16 = r14, 3, r0	;;
	shr.u		r14 = r36, r16		;;
	cmp.geu		p6, p7 = 15, r14	;;
   (p7)	shr.u		r14 = r14, 4
   (p7)	adds		r16 = 4, r16		;;
	cmp.geu		p6, p7 = 3, r14		;;
   (p7)	shr.u		r14 = r14, 2
   (p7)	adds		r16 = 2, r16		;;
	shr.u		r14 = r14, 1		;;
	add		r16 = r16, r14		;;
	sub		r40 = 63, r16		;;
	shl		r36 = r36, r40
	shl		r38 = r38, r40		;;
	mov		r45 = r36
	br.call.sptk.many b0 = mpn_invert_limb#
	;;
	mov		r23 = 1			;;
	setf.sig	f6 = r8
	setf.sig	f12 = r23
	cmp.eq		p6, p7 = 0, r35
   (p6)	br.cond.dpnt	.L435
	shladd		r15 = r35, 3, r34
	sub		r16 = 64, r40		;;
	adds		r15 = -8, r15
	adds		r35 = -2, r35		;;
	ld8		r19 = [r15]
	cmp.le		p6, p7 = r0, r35	;;
	shr.u		r14 = r19, r16		;;
	or		r38 = r14, r38
   (p7)	br.cond.dpnt	.L529
	mov		r22 = r16
	shladd		r34 = r35, 3, r34	;;
	setf.sig	f10 = r36
	setf.sig	f7 = r38
	mov		ar.lc = r35		;;
.Loop3:
	ld8		r14 = [r34], -8
	xma.hu		f11 = f7, f6, f0
	;;
	xma.l		f8 = f11, f12, f7	C q = q + nh
	;;
	getf.sig	r18 = f8
	xma.hu		f9 = f8, f10, f0
	shl		r20 = r19, r40
	xma.l		f8 = f8, f10, f0
	shr.u		r24 = r14, r22
	;;
	getf.sig	r16 = f9
	getf.sig	r15 = f8
	;;
	or		r20 = r24, r20
	;;
	cmp.ltu		p6, p7 = r20, r15
	sub		r15 = r20, r15
	sub		r16 = r38, r16
	;;
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0?
   (p6)	add		r16 = -1, r16
   (p0)	cmp.ne.unc	p6, p7 = r0, r0
	;;
   (p8)	cmp.ltu		p6, p7 = r15, r36
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	.pred.rel "mutex",p6,p7
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0 still?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0 still?
   (p6)	add		r16 = -1, r16		C propagate carry into rH
	;;
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	cmp.ltu		p6, p7 = r15, r36
	;;
   (p7)	sub		r15 = r15, r36
   (p7)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	setf.sig	f7 = r15
	st8		[r32] = r18, -8
	mov		r19 = r14
	mov		r38 = r15
	br.cloop.dptk	.Loop3
	;;
.L529:
	setf.sig	f10 = r36
	setf.sig	f7 = r38
	;;
	xma.hu		f11 = f7, f6, f0
	;;
	xma.l		f8 = f11, f12, f7	C q = q + nh
	;;
	getf.sig	r18 = f8
	xma.hu		f9 = f8, f10, f0
	shl		r20 = r19, r40
	xma.l		f8 = f8, f10, f0
	;;
	getf.sig	r16 = f9
	getf.sig	r15 = f8
	;;
	cmp.ltu		p6, p7 = r20, r15
	sub		r15 = r20, r15
	sub		r16 = r38, r16
	;;
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0?
   (p6)	add		r16 = -1, r16
   (p0)	cmp.ne.unc	p6, p7 = r0, r0
	;;
   (p8)	cmp.ltu		p6, p7 = r15, r36
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	.pred.rel "mutex",p6,p7
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0 still?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0 still?
   (p6)	add		r16 = -1, r16		C propagate carry into rH
	;;
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	cmp.ltu		p6, p7 = r15, r36
	;;
   (p7)	sub		r15 = r15, r36
   (p7)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	st8		[r32] = r18, -8
	mov		r38 = r15
.L435:
	adds		r35 = -1, r33
	;;
	cmp.le		p6, p7 = r0, r35
   (p7)	br.cond.dpnt	.L530
	setf.sig	f7 = r38
	setf.sig	f10 = r36
	mov		ar.lc = r35
	;;
.Loop4:
	xma.hu		f11 = f7, f6, f0
	;;
	xma.l		f8 = f11, f12, f7	C q = q + nh
	;;
	getf.sig	r18 = f8
	xma.hu		f9 = f8, f10, f0
	xma.l		f8 = f8, f10, f0
	;;
	getf.sig	r16 = f9
	getf.sig	r15 = f8
	;;
	cmp.ltu		p6, p7 = 0, r15
	sub		r15 = 0, r15
	sub		r16 = r38, r16
	;;
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0?
   (p6)	add		r16 = -1, r16
   (p0)	cmp.ne.unc	p6, p7 = r0, r0
	;;
   (p8)	cmp.ltu		p6, p7 = r15, r36
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	.pred.rel "mutex",p6,p7
   (p6)	cmp.ne		p8, p9 = 1, r16		C is rH != 0 still?
   (p7)	cmp.ne		p8, p9 = 0, r16		C is rH != 0 still?
   (p6)	add		r16 = -1, r16		C propagate carry into rH
	;;
   (p8)	sub		r15 = r15, r36
   (p8)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	cmp.ltu		p6, p7 = r15, r36
	;;
   (p7)	sub		r15 = r15, r36
   (p7)	add		r18 = 1, r18		C q = 1 + 1;	done if: rH > 0
	;;
	setf.sig	f7 = r15
	st8		[r32] = r18, -8
	mov		r38 = r15
	br.cloop.dptk	.Loop4
	;;
.L530:
	shr.u		r8 = r38, r40
.L159:
	mov		ar.pfs = r42
	mov		ar.lc = r44
	mov		b0 = r41
	br.ret.sptk.many b0
EPILOGUE(mpn_divrem_1)
ASM_END()

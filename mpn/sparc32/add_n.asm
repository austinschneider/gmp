! SPARC __mpn_add_n -- Add two limb vectors of the same length > 0 and store
! sum in a third limb vector.

! Copyright (C) 1995, 1996 Free Software Foundation, Inc.

! This file is part of the GNU MP Library.

! The GNU MP Library is free software; you can redistribute it and/or modify
! it under the terms of the GNU Library General Public License as published by
! the Free Software Foundation; either version 2 of the License, or (at your
! option) any later version.

! The GNU MP Library is distributed in the hope that it will be useful, but
! WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
! or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
! License for more details.

! You should have received a copy of the GNU Library General Public License
! along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
! the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
! MA 02111-1307, USA.


! INPUT PARAMETERS
define(res_ptr,%o0)
define(s1_ptr,%o1)
define(s2_ptr,%o2)
define(size,%o3)

include(`../config.m4')

ASM_START()
PROLOGUE(mpn_add_n)
	xor	s2_ptr,res_ptr,%g1
	andcc	%g1,4,%g0
	bne	L(1)			! branch if alignment differs
	nop
! **  V1a  **
L(0):	andcc	res_ptr,4,%g0		! res_ptr unaligned? Side effect: cy=0
	be	L(v1)			! if no, branch
	nop
/* Add least significant limb separately to align res_ptr and s2_ptr */
	ld	[s1_ptr],%g4
	add	s1_ptr,4,s1_ptr
	ld	[s2_ptr],%g2
	add	s2_ptr,4,s2_ptr
	add	size,-1,size
	addcc	%g4,%g2,%o4
	st	%o4,[res_ptr]
	add	res_ptr,4,res_ptr
L(v1):	addx	%g0,%g0,%o4		! save cy in register
	cmp	size,2			! if size < 2 ...
	bl	L(end2)			! ... branch to tail code
	subcc	%g0,%o4,%g0		! restore cy

	ld	[s1_ptr+0],%g4
	addcc	size,-10,size
	ld	[s1_ptr+4],%g1
	ldd	[s2_ptr+0],%g2
	blt	L(fin1)
	subcc	%g0,%o4,%g0		! restore cy
/* Add blocks of 8 limbs until less than 8 limbs remain */
L(loop1):
	addxcc	%g4,%g2,%o4
	ld	[s1_ptr+8],%g4
	addxcc	%g1,%g3,%o5
	ld	[s1_ptr+12],%g1
	ldd	[s2_ptr+8],%g2
	std	%o4,[res_ptr+0]
	addxcc	%g4,%g2,%o4
	ld	[s1_ptr+16],%g4
	addxcc	%g1,%g3,%o5
	ld	[s1_ptr+20],%g1
	ldd	[s2_ptr+16],%g2
	std	%o4,[res_ptr+8]
	addxcc	%g4,%g2,%o4
	ld	[s1_ptr+24],%g4
	addxcc	%g1,%g3,%o5
	ld	[s1_ptr+28],%g1
	ldd	[s2_ptr+24],%g2
	std	%o4,[res_ptr+16]
	addxcc	%g4,%g2,%o4
	ld	[s1_ptr+32],%g4
	addxcc	%g1,%g3,%o5
	ld	[s1_ptr+36],%g1
	ldd	[s2_ptr+32],%g2
	std	%o4,[res_ptr+24]
	addx	%g0,%g0,%o4		! save cy in register
	addcc	size,-8,size
	add	s1_ptr,32,s1_ptr
	add	s2_ptr,32,s2_ptr
	add	res_ptr,32,res_ptr
	bge	L(loop1)
	subcc	%g0,%o4,%g0		! restore cy

L(fin1):
	addcc	size,8-2,size
	blt	L(end1)
	subcc	%g0,%o4,%g0		! restore cy
/* Add blocks of 2 limbs until less than 2 limbs remain */
L(loope1):
	addxcc	%g4,%g2,%o4
	ld	[s1_ptr+8],%g4
	addxcc	%g1,%g3,%o5
	ld	[s1_ptr+12],%g1
	ldd	[s2_ptr+8],%g2
	std	%o4,[res_ptr+0]
	addx	%g0,%g0,%o4		! save cy in register
	addcc	size,-2,size
	add	s1_ptr,8,s1_ptr
	add	s2_ptr,8,s2_ptr
	add	res_ptr,8,res_ptr
	bge	L(loope1)
	subcc	%g0,%o4,%g0		! restore cy
L(end1):
	addxcc	%g4,%g2,%o4
	addxcc	%g1,%g3,%o5
	std	%o4,[res_ptr+0]
	addx	%g0,%g0,%o4		! save cy in register

	andcc	size,1,%g0
	be	L(ret1)
	subcc	%g0,%o4,%g0		! restore cy
/* Add last limb */
	ld	[s1_ptr+8],%g4
	ld	[s2_ptr+8],%g2
	addxcc	%g4,%g2,%o4
	st	%o4,[res_ptr+8]

L(ret1):
	retl
	addx	%g0,%g0,%o0	! return carry-out from most sign. limb

L(1):	xor	s1_ptr,res_ptr,%g1
	andcc	%g1,4,%g0
	bne	L(2)
	nop
! **  V1b  **
	mov	s2_ptr,%g1
	mov	s1_ptr,s2_ptr
	b	L(0)
	mov	%g1,s1_ptr

! **  V2  **
/* If we come here, the alignment of s1_ptr and res_ptr as well as the
   alignment of s2_ptr and res_ptr differ.  Since there are only two ways
   things can be aligned (that we care about) we now know that the alignment
   of s1_ptr and s2_ptr are the same.  */

L(2):	cmp	size,1
	be	L(jone)
	nop
	andcc	s1_ptr,4,%g0		! s1_ptr unaligned? Side effect: cy=0
	be	L(v2)			! if no, branch
	nop
/* Add least significant limb separately to align s1_ptr and s2_ptr */
	ld	[s1_ptr],%g4
	add	s1_ptr,4,s1_ptr
	ld	[s2_ptr],%g2
	add	s2_ptr,4,s2_ptr
	add	size,-1,size
	addcc	%g4,%g2,%o4
	st	%o4,[res_ptr]
	add	res_ptr,4,res_ptr

L(v2):	addx	%g0,%g0,%o4		! save cy in register
	addcc	size,-8,size
	blt	L(fin2)
	subcc	%g0,%o4,%g0		! restore cy
/* Add blocks of 8 limbs until less than 8 limbs remain */
L(loop2):
	ldd	[s1_ptr+0],%g2
	ldd	[s2_ptr+0],%o4
	addxcc	%g2,%o4,%g2
	st	%g2,[res_ptr+0]
	addxcc	%g3,%o5,%g3
	st	%g3,[res_ptr+4]
	ldd	[s1_ptr+8],%g2
	ldd	[s2_ptr+8],%o4
	addxcc	%g2,%o4,%g2
	st	%g2,[res_ptr+8]
	addxcc	%g3,%o5,%g3
	st	%g3,[res_ptr+12]
	ldd	[s1_ptr+16],%g2
	ldd	[s2_ptr+16],%o4
	addxcc	%g2,%o4,%g2
	st	%g2,[res_ptr+16]
	addxcc	%g3,%o5,%g3
	st	%g3,[res_ptr+20]
	ldd	[s1_ptr+24],%g2
	ldd	[s2_ptr+24],%o4
	addxcc	%g2,%o4,%g2
	st	%g2,[res_ptr+24]
	addxcc	%g3,%o5,%g3
	st	%g3,[res_ptr+28]
	addx	%g0,%g0,%o4		! save cy in register
	addcc	size,-8,size
	add	s1_ptr,32,s1_ptr
	add	s2_ptr,32,s2_ptr
	add	res_ptr,32,res_ptr
	bge	L(loop2)
	subcc	%g0,%o4,%g0		! restore cy

L(fin2):
	addcc	size,8-2,size
	blt	L(end2)
	subcc	%g0,%o4,%g0		! restore cy
L(loope2):
	ldd	[s1_ptr+0],%g2
	ldd	[s2_ptr+0],%o4
	addxcc	%g2,%o4,%g2
	st	%g2,[res_ptr+0]
	addxcc	%g3,%o5,%g3
	st	%g3,[res_ptr+4]
	addx	%g0,%g0,%o4		! save cy in register
	addcc	size,-2,size
	add	s1_ptr,8,s1_ptr
	add	s2_ptr,8,s2_ptr
	add	res_ptr,8,res_ptr
	bge	L(loope2)
	subcc	%g0,%o4,%g0		! restore cy
L(end2):
	andcc	size,1,%g0
	be	L(ret2)
	subcc	%g0,%o4,%g0		! restore cy
/* Add last limb */
L(jone):
	ld	[s1_ptr],%g4
	ld	[s2_ptr],%g2
	addxcc	%g4,%g2,%o4
	st	%o4,[res_ptr]

L(ret2):
	retl
	addx	%g0,%g0,%o0	! return carry-out from most sign. limb
EPILOGUE(mpn_add_n)

dnl  IA-64 time stamp counter access routine.

dnl  Copyright 2000, 2005 Free Software Foundation, Inc.
dnl
dnl  This file is part of the GNU MP Library.
dnl
dnl  The GNU MP Library is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public License as
dnl  published by the Free Software Foundation; either version 2.1 of the
dnl  License, or (at your option) any later version.
dnl
dnl  The GNU MP Library is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public
dnl  License along with the GNU MP Library; see the file COPYING.LIB.  If
dnl  not, write to the Free Software Foundation, Inc., 59 Temple Place -
dnl  Suite 330, Boston, MA 02111-1307, USA.

include(`../config.m4')


C void speed_cyclecounter (unsigned int p[2]);
C

ASM_START()
PROLOGUE(speed_cyclecounter)
	mov	r14 = ar.itc
	;;
	st4	[r32] = r14, 4
	shr.u	r14 = r14, 32
	;;
	st4	[r32] = r14
	br.ret.sptk.many b0
EPILOGUE(speed_cyclecounter)
ASM_END()

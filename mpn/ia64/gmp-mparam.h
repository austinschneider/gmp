/* gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 2000, 2001 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#define BITS_PER_MP_LIMB 64
#define BYTES_PER_MP_LIMB 8

/* Generated by tuneup.c, 2000-10-28. */

#ifndef KARATSUBA_MUL_THRESHOLD
#define KARATSUBA_MUL_THRESHOLD     20
#endif
#ifndef TOOM3_MUL_THRESHOLD
#define TOOM3_MUL_THRESHOLD        129
#endif

#ifndef KARATSUBA_SQR_THRESHOLD
#define KARATSUBA_SQR_THRESHOLD     50
#endif
#ifndef TOOM3_SQR_THRESHOLD
#define TOOM3_SQR_THRESHOLD        129
#endif

#ifndef DC_THRESHOLD
#define DC_THRESHOLD                38
#endif

#ifndef FIB_THRESHOLD
#define FIB_THRESHOLD              198
#endif

#ifndef POWM_THRESHOLD
#define POWM_THRESHOLD             184
#endif

#ifndef GCD_ACCEL_THRESHOLD
#define GCD_ACCEL_THRESHOLD          3
#endif
#ifndef GCDEXT_THRESHOLD
#define GCDEXT_THRESHOLD            34
#endif

#ifndef FFT_MUL_TABLE
#define FFT_MUL_TABLE  { 560, 1184, 2624, 4864, 11264, 20480, 0 }
#endif
#ifndef FFT_MODF_MUL_THRESHOLD
#define FFT_MODF_MUL_THRESHOLD     576
#endif
#ifndef FFT_MUL_THRESHOLD
#define FFT_MUL_THRESHOLD         4224
#endif

#ifndef FFT_SQR_TABLE
#define FFT_SQR_TABLE  { 496, 992, 2624, 4864, 11264, 28672, 0 }
#endif
#ifndef FFT_MODF_SQR_THRESHOLD
#define FFT_MODF_SQR_THRESHOLD     512
#endif
#ifndef FFT_SQR_THRESHOLD
#define FFT_SQR_THRESHOLD         3712
#endif

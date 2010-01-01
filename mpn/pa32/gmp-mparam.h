/* HP-PA 1.0 gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 1999, 2000, 2001, 2002 Free Software Foundation,
Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#define GMP_LIMB_BITS 32
#define BYTES_PER_MP_LIMB 4

/* These values are for the PA7100 using GCC.  */
/* Generated by tuneup.c, 2000-10-27. */

#ifndef MUL_TOOM22_THRESHOLD   
#define MUL_TOOM22_THRESHOLD      30
#endif
#ifndef MUL_TOOM33_THRESHOLD
#define MUL_TOOM33_THRESHOLD     141
#endif

#ifndef SQR_TOOM2_THRESHOLD    
#define SQR_TOOM2_THRESHOLD       59
#endif
#ifndef SQR_TOOM3_THRESHOLD
#define SQR_TOOM3_THRESHOLD      177
#endif

#ifndef DIV_DC_THRESHOLD
#define DIV_DC_THRESHOLD             108
#endif

#ifndef POWM_THRESHOLD
#define POWM_THRESHOLD            18
#endif

#ifndef GCD_ACCEL_THRESHOLD
#define GCD_ACCEL_THRESHOLD       46
#endif
#define GCDEXT_THRESHOLD          33
#endif

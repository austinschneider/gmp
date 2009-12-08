/* POWER3/PowerPC630 gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 2008, 2009 Free Software Foundation, Inc.

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

#define GMP_LIMB_BITS 64
#define BYTES_PER_MP_LIMB 8


/* Generated by tuneup.c, 2009-11-29, gcc 2.9 */

#define MUL_TOOM22_THRESHOLD             10
#define MUL_TOOM33_THRESHOLD             30
#define MUL_TOOM44_THRESHOLD             46

#define SQR_BASECASE_THRESHOLD            0  /* always */
#define SQR_TOOM2_THRESHOLD              16
#define SQR_TOOM3_THRESHOLD              45
#define SQR_TOOM4_THRESHOLD              64

#define MUL_FFT_TABLE  { 208, 480, 1088, 1792, 5120, 12288, 49152, 196608, 786432, 0 }
#define MUL_FFT_MODF_THRESHOLD          224
#define MUL_FFT_THRESHOLD              1408

#define SQR_FFT_TABLE  { 208, 416, 960, 1792, 5120, 12288, 49152, 196608, 786432, 0 }
#define SQR_FFT_MODF_THRESHOLD          184
#define SQR_FFT_THRESHOLD              1152

#define MULLO_BASECASE_THRESHOLD          3
#define MULLO_DC_THRESHOLD               28
#define MULLO_MUL_N_THRESHOLD          2350

#define MULMOD_BNM1_THRESHOLD             8

#define DC_DIV_QR_THRESHOLD              28
#define REDC_1_TO_REDC_N_THRESHOLD       39

#define MATRIX22_STRASSEN_THRESHOLD       8
#define HGCD_THRESHOLD                   53
#define GCD_DC_THRESHOLD                148
#define GCDEXT_DC_THRESHOLD             153
#define JACOBI_BASE_METHOD                1

#define MOD_1_NORM_THRESHOLD              0  /* always */
#define MOD_1_UNNORM_THRESHOLD            0  /* always */
#define MOD_1_1_THRESHOLD                 6
#define MOD_1_2_THRESHOLD             MP_SIZE_T_MAX  /* never */
#define MOD_1_4_THRESHOLD             MP_SIZE_T_MAX  /* never */
#define USE_PREINV_DIVREM_1               0
#define USE_PREINV_MOD_1                  1
#define DIVEXACT_1_THRESHOLD              0  /* always (native) */
#define MODEXACT_1_ODD_THRESHOLD          0  /* always */

#define GET_STR_DC_THRESHOLD             15
#define GET_STR_PRECOMPUTE_THRESHOLD     29
#define SET_STR_DC_THRESHOLD            357
#define SET_STR_PRECOMPUTE_THRESHOLD    803

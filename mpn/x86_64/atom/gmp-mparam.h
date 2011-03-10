/* Intel Atom/64 gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
2008, 2009, 2010 Free Software Foundation, Inc.

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

#define SHLD_SLOW 1
#define SHRD_SLOW 1

/* These routines exists for all x86_64 chips, but they are slower on Atom
   than separate add/sub and shift.  Make sure they are not really used.  */
#undef HAVE_NATIVE_mpn_rsh1add_n
#undef HAVE_NATIVE_mpn_rsh1sub_n

#define MOD_1_NORM_THRESHOLD                 0  /* always */
#define MOD_1_UNNORM_THRESHOLD               0  /* always */
#define MOD_1N_TO_MOD_1_1_THRESHOLD          4
#define MOD_1U_TO_MOD_1_1_THRESHOLD          3
#define MOD_1_1_TO_MOD_1_2_THRESHOLD     MP_SIZE_T_MAX
#define MOD_1_2_TO_MOD_1_4_THRESHOLD         0
#define PREINV_MOD_1_TO_MOD_1_THRESHOLD     10
#define USE_PREINV_DIVREM_1                  1  /* native */
#define DIVEXACT_1_THRESHOLD                 0  /* always (native) */
#define BMOD_1_TO_MOD_1_THRESHOLD           17

#define MUL_TOOM22_THRESHOLD                10
#define MUL_TOOM33_THRESHOLD                65
#define MUL_TOOM44_THRESHOLD               105
#define MUL_TOOM6H_THRESHOLD               157
#define MUL_TOOM8H_THRESHOLD               212

#define MUL_TOOM32_TO_TOOM43_THRESHOLD      65
#define MUL_TOOM32_TO_TOOM53_THRESHOLD      76
#define MUL_TOOM42_TO_TOOM53_THRESHOLD      71
#define MUL_TOOM42_TO_TOOM63_THRESHOLD      66

#define SQR_BASECASE_THRESHOLD               0  /* always (native) */
#define SQR_TOOM2_THRESHOLD                 16
#define SQR_TOOM3_THRESHOLD                 65
#define SQR_TOOM4_THRESHOLD                166
#define SQR_TOOM6_THRESHOLD                222
#define SQR_TOOM8_THRESHOLD                333

#define MULMOD_BNM1_THRESHOLD                7
#define SQRMOD_BNM1_THRESHOLD               12

#define MUL_FFT_MODF_THRESHOLD             220  /* k = 5 */
#define MUL_FFT_TABLE3                                      \
  { {    220, 5}, {      7, 4}, {     15, 5}, {     13, 6}, \
    {      7, 5}, {     15, 6}, {     13, 7}, {      7, 6}, \
    {     15, 7}, {      8, 6}, {     17, 7}, {     13, 8}, \
    {      7, 7}, {     17, 8}, {      9, 7}, {     19, 8}, \
    {     11, 7}, {     23, 8}, {     13, 9}, {      7, 8}, \
    {     15, 7}, {     31, 8}, {     19, 9}, {     11, 8}, \
    {     25,10}, {      7, 9}, {     15, 8}, {     33, 9}, \
    {     19, 8}, {     39, 9}, {     23,10}, {     15, 9}, \
    {     39,10}, {     23, 9}, {     47,11}, {     15,10}, \
    {     31, 9}, {     67,10}, {     39, 9}, {     79, 8}, \
    {    159,10}, {     47, 9}, {     95, 8}, {    191, 7}, \
    {    383,10}, {     55,11}, {     31,10}, {     63, 9}, \
    {    127, 8}, {    255,10}, {     71, 9}, {    143,10}, \
    {     79, 9}, {    159,11}, {     47,10}, {     95, 9}, \
    {    191, 8}, {    383,12}, {     31,11}, {     63, 9}, \
    {    255,10}, {    143, 9}, {    287,11}, {     79,10}, \
    {    159, 9}, {    319,10}, {    175, 9}, {    351, 8}, \
    {    703,11}, {     95,10}, {    191, 9}, {    383,10}, \
    {    207, 9}, {    415,11}, {    111,10}, {    223, 9}, \
    {    447,12}, {     63,11}, {    127,10}, {    255, 9}, \
    {    511,11}, {    143, 9}, {    575,10}, {    319,11}, \
    {    175,10}, {    351, 9}, {    703,11}, {    191,10}, \
    {    383,11}, {    223,13}, {   8192,14}, {  16384,15}, \
    {  32768,16}, {  65536,17}, { 131072,18}, { 262144,19}, \
    { 524288,20}, {1048576,21}, {2097152,22}, {4194304,23}, \
    {8388608,24} }
#define MUL_FFT_TABLE3_SIZE 101
#define MUL_FFT_THRESHOLD                 1728

#define SQR_FFT_MODF_THRESHOLD             188  /* k = 5 */
#define SQR_FFT_TABLE3                                      \
  { {    188, 5}, {      6, 4}, {     13, 5}, {      7, 4}, \
    {     15, 5}, {     11, 6}, {      6, 5}, {     13, 6}, \
    {      7, 5}, {     15, 6}, {     13, 7}, {      7, 6}, \
    {     15, 7}, {     13, 8}, {      7, 7}, {     17, 8}, \
    {      9, 7}, {     19, 8}, {     11, 7}, {     23, 8}, \
    {     13, 9}, {      7, 8}, {     19, 9}, {     11, 8}, \
    {     25,10}, {      7, 9}, {     15, 8}, {     33, 9}, \
    {     19, 8}, {     39, 9}, {     23,10}, {     15, 9}, \
    {     39,10}, {     23, 9}, {     47,11}, {     15,10}, \
    {     31, 9}, {     67,10}, {     39, 9}, {     79,10}, \
    {     47, 7}, {    383, 9}, {    103,11}, {     31,10}, \
    {     63, 9}, {    127, 8}, {    255, 7}, {    511, 8}, \
    {    287, 7}, {    575,10}, {     79, 9}, {    159, 8}, \
    {    319,11}, {     47, 9}, {    191,12}, {     31,11}, \
    {     63,10}, {    127, 9}, {    255,10}, {    143, 7}, \
    {   1151, 9}, {    351,11}, {     95,10}, {    191,11}, \
    {    111,10}, {    223,12}, {     63, 9}, {    511,11}, \
    {    143,10}, {    287, 9}, {    575,11}, {    159,10}, \
    {    319, 9}, {    639,11}, {    175,10}, {    351, 9}, \
    {    703,12}, {     95,11}, {    191,10}, {    383,11}, \
    {    207,10}, {    415,11}, {    223,10}, {    447,13}, \
    {   8192,14}, {  16384,15}, {  32768,16}, {  65536,17}, \
    { 131072,18}, { 262144,19}, { 524288,20}, {1048576,21}, \
    {2097152,22}, {4194304,23}, {8388608,24} }
#define SQR_FFT_TABLE3_SIZE 95
#define SQR_FFT_THRESHOLD                 1600

#define MULLO_BASECASE_THRESHOLD             3
#define MULLO_DC_THRESHOLD                  22
#define MULLO_MUL_N_THRESHOLD             3176

#define DC_DIV_QR_THRESHOLD                 27
#define DC_DIVAPPR_Q_THRESHOLD              92
#define DC_BDIV_QR_THRESHOLD                27
#define DC_BDIV_Q_THRESHOLD                 62

#define INV_MULMOD_BNM1_THRESHOLD           18
#define INV_NEWTON_THRESHOLD               131
#define INV_APPR_THRESHOLD                 103

#define BINV_NEWTON_THRESHOLD              163
#define REDC_1_TO_REDC_2_THRESHOLD          12
#define REDC_2_TO_REDC_N_THRESHOLD          36

#define MU_DIV_QR_THRESHOLD                792
#define MU_DIVAPPR_Q_THRESHOLD             807
#define MUPI_DIV_QR_THRESHOLD               67
#define MU_BDIV_QR_THRESHOLD               654
#define MU_BDIV_Q_THRESHOLD                748

#define MATRIX22_STRASSEN_THRESHOLD         13
#define HGCD_THRESHOLD                      82
#define GCD_DC_THRESHOLD                   186
#define GCDEXT_DC_THRESHOLD                186
#define JACOBI_BASE_METHOD                   4

#define GET_STR_DC_THRESHOLD                15
#define GET_STR_PRECOMPUTE_THRESHOLD        26
#define SET_STR_DC_THRESHOLD               266
#define SET_STR_PRECOMPUTE_THRESHOLD      1120

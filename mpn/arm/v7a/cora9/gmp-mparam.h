/* gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 1991, 1993, 1994, 1999-2003, 2009, 2010, 2012, 2013 Free Software
Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

#define GMP_LIMB_BITS 32
#define GMP_LIMB_BYTES 4

/* 1000MHz Cortex-A9 */

#define MOD_1_NORM_THRESHOLD                 0  /* always */
#define MOD_1_UNNORM_THRESHOLD               0  /* always */
#define MOD_1N_TO_MOD_1_1_THRESHOLD          3
#define MOD_1U_TO_MOD_1_1_THRESHOLD          3
#define MOD_1_1_TO_MOD_1_2_THRESHOLD        10
#define MOD_1_2_TO_MOD_1_4_THRESHOLD     MP_SIZE_T_MAX
#define PREINV_MOD_1_TO_MOD_1_THRESHOLD     12
#define USE_PREINV_DIVREM_1                  1  /* native */
#define DIV_QR_1N_PI1_METHOD                 1
#define DIV_QR_1_NORM_THRESHOLD          MP_SIZE_T_MAX  /* never */
#define DIV_QR_1_UNNORM_THRESHOLD        MP_SIZE_T_MAX  /* never */
#define DIV_QR_2_PI2_THRESHOLD           MP_SIZE_T_MAX  /* never */
#define DIVEXACT_1_THRESHOLD                 0  /* always (native) */
#define BMOD_1_TO_MOD_1_THRESHOLD           20

#define MUL_TOOM22_THRESHOLD                44
#define MUL_TOOM33_THRESHOLD               129
#define MUL_TOOM44_THRESHOLD               387
#define MUL_TOOM6H_THRESHOLD               517
#define MUL_TOOM8H_THRESHOLD               774

#define MUL_TOOM32_TO_TOOM43_THRESHOLD     133
#define MUL_TOOM32_TO_TOOM53_THRESHOLD     222
#define MUL_TOOM42_TO_TOOM53_THRESHOLD     136
#define MUL_TOOM42_TO_TOOM63_THRESHOLD     258
#define MUL_TOOM43_TO_TOOM54_THRESHOLD     193

#define SQR_BASECASE_THRESHOLD               0  /* always (native) */
#define SQR_TOOM2_THRESHOLD                 54
#define SQR_TOOM3_THRESHOLD                181
#define SQR_TOOM4_THRESHOLD                490
#define SQR_TOOM6_THRESHOLD                632
#define SQR_TOOM8_THRESHOLD                  0  /* always */

#define MULMID_TOOM42_THRESHOLD             66

#define MULMOD_BNM1_THRESHOLD               23
#define SQRMOD_BNM1_THRESHOLD               30

#define MUL_FFT_MODF_THRESHOLD             624  /* k = 5 */
#define MUL_FFT_TABLE3                                      \
  { {    624, 5}, {     29, 6}, {     15, 5}, {     35, 6}, \
    {     29, 7}, {     15, 6}, {     36, 7}, {     19, 6}, \
    {     40, 7}, {     21, 6}, {     43, 7}, {     23, 6}, \
    {     47, 7}, {     25, 6}, {     51, 7}, {     27, 6}, \
    {     55, 7}, {     29, 8}, {     15, 7}, {     35, 8}, \
    {     19, 7}, {     43, 8}, {     23, 7}, {     51, 8}, \
    {     27, 7}, {     57, 9}, {     15, 8}, {     31, 7}, \
    {     65, 8}, {     35, 7}, {     71, 8}, {     43, 9}, \
    {     23, 8}, {     55,10}, {     15, 9}, {     31, 8}, \
    {     71, 9}, {     39, 8}, {     83, 9}, {     47, 8}, \
    {     99, 9}, {     55,10}, {     31, 9}, {     79,10}, \
    {     47, 9}, {    103,11}, {     31,10}, {     63, 9}, \
    {    135,10}, {     79, 9}, {    167,10}, {     95, 9}, \
    {    191,10}, {    111,11}, {     63,10}, {    159,11}, \
    {     95,10}, {    191, 9}, {    383,12}, {     63,11}, \
    {    127,10}, {    255, 9}, {    511,10}, {    271,11}, \
    {    159,10}, {    319, 9}, {    639,10}, {    335, 9}, \
    {    671,11}, {    191,10}, {    383, 9}, {    767,10}, \
    {    399, 9}, {    799,10}, {    415,11}, {    223,12}, \
    {   4096,13}, {   8192,14}, {  16384,15}, {  32768,16} }
#define MUL_FFT_TABLE3_SIZE 80
#define MUL_FFT_THRESHOLD                 6784

#define SQR_FFT_MODF_THRESHOLD             560  /* k = 5 */
#define SQR_FFT_TABLE3                                      \
  { {    560, 5}, {     21, 4}, {     43, 5}, {     28, 6}, \
    {     15, 5}, {     32, 6}, {     17, 5}, {     35, 6}, \
    {     18, 5}, {     37, 6}, {     19, 5}, {     39, 6}, \
    {     37, 7}, {     19, 6}, {     40, 7}, {     21, 6}, \
    {     43, 7}, {     23, 6}, {     47, 7}, {     27, 6}, \
    {     55, 7}, {     29, 8}, {     15, 7}, {     37, 8}, \
    {     19, 7}, {     43, 8}, {     23, 7}, {     51, 8}, \
    {     27, 7}, {     55, 9}, {     15, 8}, {     31, 7}, \
    {     65, 8}, {     35, 7}, {     71, 8}, {     43, 9}, \
    {     23, 8}, {     55,10}, {     15, 9}, {     31, 8}, \
    {     71, 9}, {     39, 8}, {     83, 9}, {     47, 8}, \
    {     95, 9}, {     55,10}, {     31, 9}, {     79,10}, \
    {     47, 9}, {    103,11}, {     31,10}, {     63, 9}, \
    {    135,10}, {     79, 9}, {    159,10}, {     95, 9}, \
    {    191,10}, {    111,11}, {     63,10}, {    143, 9}, \
    {    287,10}, {    159,11}, {     95,10}, {    191, 9}, \
    {    383,10}, {    207,12}, {     63,11}, {    127,10}, \
    {    255, 9}, {    511, 8}, {   1023, 9}, {    543,10}, \
    {    287,11}, {    159,10}, {    319, 9}, {    639,10}, \
    {    335, 9}, {    671,10}, {    351,11}, {    191,10}, \
    {    383, 9}, {    767,10}, {    399, 9}, {    799,10}, \
    {    415, 9}, {    831,11}, {    223,12}, {   4096,13}, \
    {   8192,14}, {  16384,15}, {  32768,16} }
#define SQR_FFT_TABLE3_SIZE 91
#define SQR_FFT_THRESHOLD                 5312

#define MULLO_BASECASE_THRESHOLD             0  /* always */
#define MULLO_DC_THRESHOLD                  37
#define MULLO_MUL_N_THRESHOLD            13463

#define DC_DIV_QR_THRESHOLD                 42
#define DC_DIVAPPR_Q_THRESHOLD              92
#define DC_BDIV_QR_THRESHOLD                44
#define DC_BDIV_Q_THRESHOLD                 96

#define INV_MULMOD_BNM1_THRESHOLD           86
#define INV_NEWTON_THRESHOLD               134
#define INV_APPR_THRESHOLD                 118

#define BINV_NEWTON_THRESHOLD              327
#define REDC_1_TO_REDC_2_THRESHOLD           6
#define REDC_2_TO_REDC_N_THRESHOLD         142

#define MU_DIV_QR_THRESHOLD               2350
#define MU_DIVAPPR_Q_THRESHOLD            2172
#define MUPI_DIV_QR_THRESHOLD               43
#define MU_BDIV_QR_THRESHOLD              2089
#define MU_BDIV_Q_THRESHOLD               2172

#define POWM_SEC_TABLE  35,49,59,615,1867

#define MATRIX22_STRASSEN_THRESHOLD         24
#define HGCD_THRESHOLD                      64
#define HGCD_APPR_THRESHOLD                 53
#define HGCD_REDUCE_THRESHOLD             4120
#define GCD_DC_THRESHOLD                   416
#define GCDEXT_DC_THRESHOLD                298
#define JACOBI_BASE_METHOD                   4

#define GET_STR_DC_THRESHOLD                17
#define GET_STR_PRECOMPUTE_THRESHOLD        35
#define SET_STR_DC_THRESHOLD               210
#define SET_STR_PRECOMPUTE_THRESHOLD       752

#define FAC_DSC_THRESHOLD                  348
#define FAC_ODD_THRESHOLD                   29

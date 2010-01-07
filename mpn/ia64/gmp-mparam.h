/* gmp-mparam.h -- Compiler/machine parameter header file.

Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2009, 2010 Free Software
Foundation, Inc.

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

/* 1300MHz Itanium2 (babe.fsffrance.org) */


#define MOD_1_NORM_THRESHOLD                 0  /* always */
#define MOD_1_UNNORM_THRESHOLD               0  /* always */
#define MOD_1N_TO_MOD_1_1_THRESHOLD         14
#define MOD_1U_TO_MOD_1_1_THRESHOLD          8
#define MOD_1_1_TO_MOD_1_2_THRESHOLD         0
#define MOD_1_2_TO_MOD_1_4_THRESHOLD        14
#define PREINV_MOD_1_TO_MOD_1_THRESHOLD     22
#define USE_PREINV_DIVREM_1                  1  /* native */
#define DIVEXACT_1_THRESHOLD                 0  /* always (native) */
#define BMOD_1_TO_MOD_1_THRESHOLD        MP_SIZE_T_MAX  /* never */

#define MUL_TOOM22_THRESHOLD                44
#define MUL_TOOM33_THRESHOLD                89
#define MUL_TOOM44_THRESHOLD               232
#define MUL_TOOM6H_THRESHOLD               351
#define MUL_TOOM8H_THRESHOLD               454

#define MUL_TOOM32_TO_TOOM43_THRESHOLD     101
#define MUL_TOOM32_TO_TOOM53_THRESHOLD     160
#define MUL_TOOM42_TO_TOOM53_THRESHOLD     138
#define MUL_TOOM42_TO_TOOM63_THRESHOLD     159

#define SQR_BASECASE_THRESHOLD              26
#define SQR_TOOM2_THRESHOLD                119
#define SQR_TOOM3_THRESHOLD                141
#define SQR_TOOM4_THRESHOLD                282
#define SQR_TOOM6_THRESHOLD                375
#define SQR_TOOM8_THRESHOLD                527

#define MULMOD_BNM1_THRESHOLD               24
#define SQRMOD_BNM1_THRESHOLD               19

#define MUL_FFT_TABLE  { 560, 1184, 1856, 3840, 11264, 28672, 114688, 327680, 0 }
#define MUL_FFT_MODF_THRESHOLD             944
#define MUL_FFT_THRESHOLD                 9856

#define SQR_FFT_TABLE  { 592, 1248, 1856, 3840, 11264, 28672, 81920, 327680, 0 }
#define SQR_FFT_MODF_THRESHOLD             752
#define SQR_FFT_THRESHOLD                 7552

#define MULLO_BASECASE_THRESHOLD            17
#define MULLO_DC_THRESHOLD                  84
#define MULLO_MUL_N_THRESHOLD            19187

#define DC_DIV_QR_THRESHOLD                 68
#define DC_DIVAPPR_Q_THRESHOLD             252
#define DC_BDIV_QR_THRESHOLD               107
#define DC_BDIV_Q_THRESHOLD                292

#define INV_MULMOD_BNM1_THRESHOLD          123
#define INV_NEWTON_THRESHOLD               187
#define INV_APPR_THRESHOLD                 179

#define BINV_NEWTON_THRESHOLD              296
#define REDC_1_TO_REDC_2_THRESHOLD          10
#define REDC_2_TO_REDC_N_THRESHOLD         164

#define MU_DIV_QR_THRESHOLD               1752
#define MU_DIVAPPR_Q_THRESHOLD            1442
#define MUPI_DIV_QR_THRESHOLD                0  /* always */
#define MU_BDIV_QR_THRESHOLD              1895
#define MU_BDIV_Q_THRESHOLD               2089

#define MATRIX22_STRASSEN_THRESHOLD         23
#define HGCD_THRESHOLD                     120
#define GCD_DC_THRESHOLD                   630
#define GCDEXT_DC_THRESHOLD                440
#define JACOBI_BASE_METHOD                   2

#define GET_STR_DC_THRESHOLD                14
#define GET_STR_PRECOMPUTE_THRESHOLD        21
#define SET_STR_DC_THRESHOLD              1240
#define SET_STR_PRECOMPUTE_THRESHOLD      3566

#define MUL_FFT_TABLE2 {{1,4}, {401,5}, {993,6}, {2113,7}, {5249,8}, {11009,9}, {11777,8}, {13057,9}, {22017,10}, {23553,9}, {28161,10}, {31745,9}, {36353,10}, {39937,9}, {42497,10}, {48129,9}, {50689,10}, {56321,11}, {63489,10}, {89089,11}, {96257,10}, {113665,12}, {126977,11}, {129025,10}, {146433,11}, {161793,10}, {171009,11}, {194561,10}, {203777,11}, {227329,12}, {258049,11}, {326657,12}, {339969,11}, {350209,12}, {389121,11}, {423937,13}, {425985,11}, {450561,13}, {516097,12}, {520193,11}, {589825,12}, {651265,11}, {686081,12}, {784385,11}, {786433,12}, {796673,11}, {813057,12}, {827393,11}, {839681,12}, {905217,13}, {1040385,12}, {1044481,11}, {1046529,12}, {1050625,11}, {1052673,12}, {1060865,11}, {1071105,12}, {1095681,11}, {1101825,12}, {1220609,10}, {1224705,11}, {1233921,10}, {1236993,12}, {1308673,10}, {1309697,12}, {1318913,13}, {1343489,11}, {1351681,12}, {1388545,13}, {1564673,12}, {1577985,11}, {1581057,12}, {1699841,11}, {1701889,10}, {1708033,11}, {1717249,12}, {1720321,14}, {2080769,13}, {2088961,12}, {2357249,13}, {2613249,12}, {2879489,13}, {2883585,11}, {MP_SIZE_T_MAX, 0}}

#define SQR_FFT_TABLE2 {{1,4}, {529,5}, {1121,6}, {2305,7}, {5249,8}, {11009,9}, {11777,8}, {12545,9}, {22017,10}, {23553,9}, {28161,10}, {31745,9}, {35329,10}, {39937,9}, {42497,10}, {48641,9}, {50177,10}, {56321,11}, {63489,10}, {66561,9}, {67585,10}, {80897,11}, {96257,10}, {106497,12}, {126977,11}, {129025,10}, {139265,11}, {141313,10}, {143361,11}, {161793,10}, {167937,11}, {194561,10}, {204801,11}, {229377,12}, {258049,11}, {327681,12}, {389121,11}, {442369,13}, {516097,12}, {520193,11}, {529409,10}, {536577,11}, {538625,10}, {540161,11}, {548865,12}, {563201,11}, {573441,12}, {577537,11}, {583681,12}, {651265,11}, {658945,9}, {659457,10}, {660993,9}, {662017,11}, {663553,9}, {665089,10}, {668161,11}, {688129,12}, {704513,11}, {706561,10}, {707585,12}, {782337,11}, {794625,9}, {796673,11}, {823297,12}, {827393,11}, {843777,9}, {845825,11}, {849921,12}, {913409,13}, {1040385,12}, {1044481,11}, {1079297,12}, {1099777,11}, {1101825,12}, {1175553,11}, {1210369,12}, {1306625,11}, {1308673,10}, {1309697,11}, {1331201,10}, {1333249,11}, {1366017,13}, {1564673,12}, {1568769,11}, {1587201,12}, {1589249,11}, {1636353,12}, {1701889,14}, {2080769,13}, {2088961,12}, {2355201,11}, {2367489,13}, {2383873,11}, {2430977,12}, {2441217,13}, {2459649,11}, {2461697,13}, {2465793,11}, {2467841,12}, {2473985,13}, {2613249,12}, {2676737,11}, {2678785,12}, {2879489,11}, {2906113,12}, {2916353,11}, {2949121,10}, {2951169,11}, {MP_SIZE_T_MAX, 0}}

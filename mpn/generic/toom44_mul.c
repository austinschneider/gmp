/* mpn_toom44_mul -- Multiply {ap,an} and {bp,bn} where an and bn are close in
   size.  Or more accurately, bn <= an < (4/3)bn.

   Contributed to the GNU project by Torbjorn Granlund and Marco Bodrato.

   THE FUNCTION IN THIS FILE IS INTERNAL WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH IT THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT IT WILL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.

Copyright 2006, 2007, 2008 Free Software Foundation, Inc.

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


#include "gmp.h"
#include "gmp-impl.h"

/* Evaluate in: -1, -1/2, 0, +1/2, +1, +2, +inf

  <-s--><--n--><--n--><--n-->
   ____ ______ ______ ______
  |_a3_|___a2_|___a1_|___a0_|
   |b3_|___b2_|___b1_|___b0_|
   <-t-><--n--><--n--><--n-->

  v0  =   a0             *  b0              #    A(0)*B(0)
  v1  = ( a0+ a1+ a2+ a3)*( b0+ b1+ b2+ b3) #    A(1)*B(1)      ah  <= 3   bh  <= 3
  vm1 = ( a0- a1+ a2- a3)*( b0- b1+ b2- b3) #   A(-1)*B(-1)    |ah| <= 1  |bh| <= 1
  v2  = ( a0+2a1+4a2+8a3)*( b0+2b1+4b2+8b3) #    A(2)*B(2)      ah  <= 14  bh  <= 14
  vh  = (8a0+4a1+2a2+ a3)*(8b0+4b1+2b2+ b3) #  A(1/2)*B(1/2)    ah  <= 14  bh  <= 14
  vmh = (8a0-4a1+2a2- a3)*(8b0-4b1+2b2- b3) # A(-1/2)*B(-1/2)  -4<=ah<=9  -4<=bh<=9
  vinf=               a3 *          b2      #  A(inf)*B(inf)
*/

#if TUNE_PROGRAM_BUILD
#define MAYBE_mul_basecase 1
#define MAYBE_mul_toom22   1
#define MAYBE_mul_toom44   1
#else
#define MAYBE_mul_basecase						\
  (MUL_TOOM44_THRESHOLD < 4 * MUL_KARATSUBA_THRESHOLD)
#define MAYBE_mul_toom22						\
  (MUL_TOOM44_THRESHOLD < 4 * MUL_TOOM33_THRESHOLD)
#define MAYBE_mul_toom44						\
  (MUL_FFT_THRESHOLD >= 4 * MUL_TOOM44_THRESHOLD)
#endif

#define TOOM44_MUL_N_REC(p, a, b, n, ws)				\
  do {									\
    if (MAYBE_mul_basecase						\
	&& BELOW_THRESHOLD (n, MUL_KARATSUBA_THRESHOLD))		\
      mpn_mul_basecase (p, a, n, b, n);					\
    else if (MAYBE_mul_toom22						\
	     && BELOW_THRESHOLD (n, MUL_TOOM33_THRESHOLD))		\
      mpn_toom22_mul (p, a, n, b, n, ws);					\
    else if (! MAYBE_mul_toom44						\
	     || BELOW_THRESHOLD (n, MUL_TOOM44_THRESHOLD))		\
      mpn_toom33_mul (p, a, n, b, n, ws);					\
    else								\
      mpn_toom44_mul (p, a, n, b, n, ws);				\
  } while (0)

/* Use of scratch space. In the product area, we store

      ___________________
     |vinf|____|_vh_|_v0_|
      s+t  2n-1 2n+1  2n

   The other recursive products, v1, vm1, v2, vmh are stored in the
   scratch area. When computing them, we use the product area for
   intermediate values.

   Next, we compute vh. We can store the intermediate factors at v0
   and at vh + 2n + 2.

   Finally, for v0 and vinf, factors are parts of the input operands,
   and we need scratch space only for the recursive multiplication.

   In all, if S(an) is the scratch need, the needed space is bounded by

     S(an) <= 4 (2*ceil(an/4) + 1) + 1 + S(ceil(an/4) + 1)

   which should give S(n) = 8 n/3 + c log(n) for some constant c.
*/

void
mpn_toom44_mul (mp_ptr pp,
		mp_srcptr ap, mp_size_t an,
		mp_srcptr bp, mp_size_t bn,
		mp_ptr scratch)
{
  mp_size_t n, s, t;
  mp_limb_t cy;
  enum toom7_flags flags;

#define a0  ap
#define a1  (ap + n)
#define a2  (ap + 2*n)
#define a3  (ap + 3*n)
#define b0  bp
#define b1  (bp + n)
#define b2  (bp + 2*n)
#define b3  (bp + 3*n)

  ASSERT (an >= bn);

  n = (an + 3) >> 2;

  s = an - 3 * n;
  t = bn - 3 * n;

  ASSERT (0 < s && s <= n);
  ASSERT (0 < t && t <= n);
  ASSERT (s >= t);

  /* NOTE: The multiplications to v1, vm1, v2 and vmh overwrites the
   * following limb, so these must be computed in order, and we need a
   * one limb gap to tp. */
#define v0    pp				/* 2n */
#define v1    scratch				/* 2n+1 */
#define vm1   (scratch + 2 * n + 1)		/* 2n+1 */
#define v2    (scratch + 4 * n + 2)		/* 2n+1 */
#define vh    (pp + 2 * n)			/* 2n+1 */
#define vmh   (scratch + 6 * n + 3)		/* 2n+1 */
#define vinf  (pp + 6 * n)			/* s+t */
#define tp (scratch + 8*n + 5)

  /* apx and bpx must not overlap with vh */
#define apx   pp				/* n+1 */
#define amx   (pp + n + 1)			/* n+1 */
#define bmx   (pp + 2*n + 2)			/* n+1 */
#define bpx   (pp + 4*n + 2)			/* n+1 */

  /* Total scratch need: 8*n + 5 + scratch for recursive calls. This
     gives roughly 32 n/3 + log term. */

  /* Compute apx = a0 + a1 + a2 + a3 and amx = a0 - a1 + a2 - a3.  */
  if (mpn_toom_eval_dgr3_pm1 (apx, amx, ap, n, s, tp))
    flags = toom7_w3_neg;
  else
    flags = 0;

  /* Compute bpx = b0 + b1 + b2 + b3 bnd bmx = b0 - b1 + b2 - b3.  */
  if (mpn_toom_eval_dgr3_pm1 (bpx, bmx, bp, n, t, tp))
    flags ^= toom7_w3_neg;

  TOOM44_MUL_N_REC (v1, apx, bpx, n + 1, tp);	/* v1,  2n+1 limbs */
  TOOM44_MUL_N_REC (vm1, amx, bmx, n + 1, tp);	/* vm1,  2n+1 limbs */

  /* Compute apx = a0 + 2 a1 + 4 a2 + 8 a3 = a0 + 2 (a1 + 2 (a2 + 2 a3))  */
#if HAVE_NATIVE_mpn_addlsh1_n
  cy  = mpn_addlsh1_n (apx, a2, a3, s);
  if (s != n)
    cy = mpn_add_1 (apx + s, a2 + s, n - s, cy);
  cy = 2 * cy + mpn_addlsh1_n (apx, a1, apx, n);
  cy = 2 * cy + mpn_addlsh1_n (apx, a0, apx, n);
#else
  cy  = mpn_lshift (apx, a3, s, 1);
  cy += mpn_add_n (apx, a2, apx, s);
  if (s != n)
    cy = mpn_add_1 (apx + s, a2 + s, n - s, cy);
  cy = 2 * cy + mpn_lshift (apx, apx, n, 1);
  cy += mpn_add_n (apx, a1, apx, n);
  cy = 2 * cy + mpn_lshift (apx, apx, n, 1);
  cy += mpn_add_n (apx, a0, apx, n);
#endif
  apx[n] = cy;

  ASSERT (apx[n] <= 14);

  /* Compute bpx = b0 + 2 b1 + 4 b2 + 8 b3 = b0 + 2 (b1 + 2 (b2 + 2 b3))  */
#if HAVE_NATIVE_mpn_addlsh1_n
  cy  = mpn_addlsh1_n (bpx, b2, b3, t);
  if (t != n)
    cy = mpn_add_1 (bpx + t, b2 + t, n - t, cy);
  cy = 2 * cy + mpn_addlsh1_n (bpx, b1, bpx, n);
  cy = 2 * cy + mpn_addlsh1_n (bpx, b0, bpx, n);
#else
  cy  = mpn_lshift (bpx, b3, t, 1);
  cy += mpn_add_n (bpx, b2, bpx, t);
  if (t != n)
    cy = mpn_add_1 (bpx + t, b2 + t, n - t, cy);
  cy = 2 * cy + mpn_lshift (bpx, bpx, n, 1);
  cy += mpn_add_n (bpx, b1, bpx, n);
  cy = 2 * cy + mpn_lshift (bpx, bpx, n, 1);
  cy += mpn_add_n (bpx, b0, bpx, n);
#endif
  bpx[n] = cy;

  ASSERT (bpx[n] <= 14);

  TOOM44_MUL_N_REC (v2, apx, bpx, n + 1, tp);	/* v2,  2n+1 limbs */

  /* Compute apx = 8 a0 + 4 a1 + 2 a2 + a3 and amx = 8 a0 - 4 a1 + 2 a2 - a3 */
  cy  = mpn_lshift (apx, a0, n, 3);			/*  8a0             */
#if HAVE_NATIVE_mpn_addlsh1_n
  apx[n] = cy + mpn_addlsh1_n (apx, apx, a2, n);		/*  8a0 + 2a2       */
#else
  cy += mpn_lshift (tp, a2, n, 1);			/*        2a2       */
  apx[n] = cy + mpn_add_n (apx, apx, tp, n);		/*  8a0 + 2a2       */
#endif
  cy = mpn_lshift (tp, a1, n, 2);			/*  4a1             */
  tp[n] = cy + mpn_add (tp, tp, n, a3, s);		/*  4a1 +  a3       */
#if HAVE_NATIVE_mpn_addsub_n
  if (mpn_cmp (apx, tp, n + 1) < 0)
    {
      mpn_addsub_n (apx, amx, tp, apx, n + 1);
      flags |= toom7_w1_neg;
    }
  else
    {
      mpn_addsub_n (apx, amx, apx, tp, n + 1);
    }
#else
  if (mpn_cmp (apx, tp, n + 1) < 0)
    {
      mpn_sub_n (amx, tp, apx, n + 1);
      flags |= toom7_w1_neg;
    }
  else
    {
      mpn_sub_n (amx, apx, tp, n + 1);
    }
  mpn_add_n (apx, apx, tp, n + 1);
#endif

  ASSERT (apx[n] <= 14);
  ASSERT (amx[n] <= 9);

  /* Compute apx = 8 b0 + 4 b1 + 2 b2 + b3 and bmx = 8 b0 - 4 b1 + 2 b2 - b3 */
  cy  = mpn_lshift (bpx, b0, n, 3);			/*  8b0             */
#if HAVE_NATIVE_mpn_addlsh1_n
  bpx[n] = cy + mpn_addlsh1_n (bpx, bpx, b2, n);		/*  8b0 + 2b2       */
#else
  cy += mpn_lshift (tp, b2, n, 1);			/*        2b2       */
  bpx[n] = cy + mpn_add_n (bpx, bpx, tp, n);		/*  8b0 + 2b2       */
#endif
  cy = mpn_lshift (tp, b1, n, 2);			/*  4a1             */
  tp[n] = cy + mpn_add (tp, tp, n, b3, t);		/*  4a1 +  b3       */
#if HAVE_NATIVE_mpn_addsub_n
  if (mpn_cmp (bpx, tp, n + 1) < 0)
    {
      mpn_addsub_n (bpx, bmx, tp, bpx, n + 1);
      flags ^= toom7_w1_neg;
    }
  else
    {
      mpn_addsub_n (bpx, bmx, bpx, tp, n + 1);
    }
#else
  if (mpn_cmp (bpx, tp, n + 1) < 0)
    {
      mpn_sub_n (bmx, tp, bpx, n + 1);
      flags ^= toom7_w1_neg;
    }
  else
    {
      mpn_sub_n (bmx, bpx, tp, n + 1);
    }
  mpn_add_n (bpx, bpx, tp, n + 1);
#endif

  ASSERT (bpx[n] <= 14);
  ASSERT (bmx[n] <= 9);

  TOOM44_MUL_N_REC (vmh, amx, bmx, n + 1, tp);	/* vmh,  2n+1 limbs */
  TOOM44_MUL_N_REC (vh, apx, bpx, n + 1, tp);	/* vh,  2n+1 limbs */

  TOOM44_MUL_N_REC (v0, a0, b0, n, tp);
  if (s > t)
    mpn_mul (vinf, a3, s, b3, t);
  else
    TOOM44_MUL_N_REC (vinf, a3, b3, s, tp);	/* vinf, s+t limbs */

  mpn_toom_interpolate_7pts (pp, n, flags, vmh, vm1, v1, v2, s + t, tp);
}

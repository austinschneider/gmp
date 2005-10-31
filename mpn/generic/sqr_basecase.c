/* mpn_sqr_basecase -- Internal routine to square a natural number
   of length n.

   THIS IS AN INTERNAL FUNCTION WITH A MUTABLE INTERFACE.  IT IS ONLY
   SAFE TO REACH THIS FUNCTION THROUGH DOCUMENTED INTERFACES.


Copyright 1991, 1992, 1993, 1994, 1996, 1997, 2000, 2001, 2002, 2003, 2004,
2005 Free Software Foundation, Inc.

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

#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"


#if HAVE_NATIVE_mpn_sqr_diagonal
#define MPN_SQR_DIAGONAL(rp, up, n)					\
  mpn_sqr_diagonal (rp, up, n)
#else
#define MPN_SQR_DIAGONAL(rp, up, n)					\
  do {									\
    mp_size_t _i;							\
    for (_i = 0; _i < (n); _i++)					\
      {									\
	mp_limb_t ul, lpl;						\
	ul = (up)[_i];							\
	umul_ppmm ((rp)[2 * _i + 1], lpl, ul, ul << GMP_NAIL_BITS);	\
	(rp)[2 * _i] = lpl >> GMP_NAIL_BITS;				\
      }									\
  } while (0)
#endif


#undef READY_WITH_mpn_sqr_basecase


#if ! defined (READY_WITH_mpn_sqr_basecase) && HAVE_NATIVE_mpn_addmul_2s
void
mpn_sqr_basecase (mp_ptr rp, mp_srcptr up, mp_size_t n)
{
  mp_size_t i;
  mp_limb_t tarr[2 * SQR_KARATSUBA_THRESHOLD];
  mp_ptr tp = tarr;
  mp_limb_t cy;

  /* must fit 2*n limbs in tarr */
  ASSERT (n <= SQR_KARATSUBA_THRESHOLD);

  if ((n & 1) != 0)
    {
      if (n == 1)
	{
	  mp_limb_t ul, lpl;
	  ul = up[0];
	  umul_ppmm (rp[1], lpl, ul, ul << GMP_NAIL_BITS);
	  rp[0] = lpl >> GMP_NAIL_BITS;
	  return;
	}

      MPN_ZERO (tp, n);

      for (i = 0; i <= n - 2; i += 2)
	{
	  cy = mpn_addmul_2s (tp + 2 * i, up + i + 1, n - (i + 1), up + i);
	  tp[n + i] = cy;
	}
    }
  else
    {
      if (n == 2)
	{
	  rp[0] = 0;
	  rp[1] = 0;
	  rp[3] = mpn_addmul_2 (rp, up, 2, up);
	  return;
	}

      MPN_ZERO (tp, n);

      for (i = 0; i <= n - 4; i += 2)
	{
	  cy = mpn_addmul_2s (tp + 2 * i, up + i + 1, n - (i + 1), up + i);
	  tp[n + i] = cy;
	}
      cy = mpn_addmul_1 (tp + 2 * n - 4, up + n - 1, 1, up[n - 2]);
      tp[2 * n - 3] = cy;
    }

  MPN_SQR_DIAGONAL (rp, up, n);

#if HAVE_NATIVE_mpn_addlsh1_n
  cy = mpn_addlsh1_n (rp + 1, rp + 1, tp, 2 * n - 2);
#else
  cy = mpn_lshift (tp, tp, 2 * n - 2, 1);
  cy += mpn_add_n (rp + 1, rp + 1, tp, 2 * n - 2);
  rp[2 * n - 1] += cy;
#endif
}
#define READY_WITH_mpn_sqr_basecase
#endif


#if ! defined (READY_WITH_mpn_sqr_basecase) && HAVE_NATIVE_mpn_addmul_2

/* mpn_sqr_basecase using plain mpn_addmul_2.

   This is tricky, since we have to let mpn_addmul_2 make some undesirable
   multiplies, u[k]*u[k], that we would like to let mpn_sqr_diagonal handle.
   This forces us to conditionally add or subtract the mpn_sqr_diagonal
   results.  Examples of the product we form:

   n = 4              n = 5		n = 6
   u1u0 * u3u2u1      u1u0 * u4u3u2u1	u1u0 * u5u4u3u2u1
   u2 * u3	      u3u2 * u4u3	u3u2 * u5u4u3
					u4 * u5
   add: u0 u2 u3      add: u0 u2 u4	add: u0 u2 u4 u5
   sub: u1	      sub: u1 u3	sub: u1 u3
*/

void
mpn_sqr_basecase (mp_ptr rp, mp_srcptr up, mp_size_t n)
{
  mp_size_t i;
  mp_limb_t tarr[2 * SQR_KARATSUBA_THRESHOLD];
  mp_ptr tp = tarr;
  mp_limb_t cy;

  /* must fit 2*n limbs in tarr */
  ASSERT (n <= SQR_KARATSUBA_THRESHOLD);

  if ((n & 1) != 0)
    {
      mp_limb_t x0, x1;

      if (n == 1)
	{
	  mp_limb_t ul, lpl;
	  ul = up[0];
	  umul_ppmm (rp[1], lpl, ul, ul << GMP_NAIL_BITS);
	  rp[0] = lpl >> GMP_NAIL_BITS;
	  return;
	}

      /* The code below doesn't like unnormalized operands.  Since such
	 operands are unusual, handle them with a dumb recursion.  */
      if (up[n - 1] == 0)
	{
	  rp[2 * n - 2] = 0;
	  rp[2 * n - 1] = 0;
	  mpn_sqr_basecase (rp, up, n - 1);
	  return;
	}

      MPN_ZERO (tp, n);

      for (i = 0; i <= n - 2; i += 2)
	{
	  cy = mpn_addmul_2 (tp + 2 * i, up + i + 1, n - (i + 1), up + i);
	  tp[n + i] = cy;
	}

      MPN_SQR_DIAGONAL (rp, up, n);

      for (i = 2;; i += 4)
	{
	  x0 = rp[i + 0];
	  rp[i + 0] = (-x0) & GMP_NUMB_MASK;
	  x1 = rp[i + 1];
	  rp[i + 1] = (-x1 - (x0 != 0)) & GMP_NUMB_MASK;
	  __GMPN_SUB_1 (cy, rp + i + 2, rp + i + 2, 2, (x1 | x0) != 0);
	  if (i + 4 >= 2 * n)
	    break;
	  mpn_incr_u (rp + i + 4, cy);
	}
    }
  else
    {
      mp_limb_t x0, x1;

      if (n == 2)
	{
	  rp[0] = 0;
	  rp[1] = 0;
	  rp[3] = mpn_addmul_2 (rp, up, 2, up);
	  return;
	}

      /* The code below doesn't like unnormalized operands.  Since such
	 operands are unusual, handle them with a dumb recursion.  */
      if (up[n - 1] == 0)
	{
	  rp[2 * n - 2] = 0;
	  rp[2 * n - 1] = 0;
	  mpn_sqr_basecase (rp, up, n - 1);
	  return;
	}

      MPN_ZERO (tp, n);

      for (i = 0; i <= n - 4; i += 2)
	{
	  cy = mpn_addmul_2 (tp + 2 * i, up + i + 1, n - (i + 1), up + i);
	  tp[n + i] = cy;
	}
      cy = mpn_addmul_1 (tp + 2 * n - 4, up + n - 1, 1, up[n - 2]);
      tp[2 * n - 3] = cy;

      MPN_SQR_DIAGONAL (rp, up, n);

      for (i = 2;; i += 4)
	{
	  x0 = rp[i + 0];
	  rp[i + 0] = (-x0) & GMP_NUMB_MASK;
	  x1 = rp[i + 1];
	  rp[i + 1] = (-x1 - (x0 != 0)) & GMP_NUMB_MASK;
	  if (i + 6 >= 2 * n)
	    break;
	  __GMPN_SUB_1 (cy, rp + i + 2, rp + i + 2, 2, (x1 | x0) != 0);
	  mpn_incr_u (rp + i + 4, cy);
	}
      mpn_decr_u (rp + i + 2, (x1 | x0) != 0);
    }

#if HAVE_NATIVE_mpn_addlsh1_n
  cy = mpn_addlsh1_n (rp + 1, rp + 1, tp, 2 * n - 2);
#else
  cy = mpn_lshift (tp, tp, 2 * n - 2, 1);
  cy += mpn_add_n (rp + 1, rp + 1, tp, 2 * n - 2);
#endif
  rp[2 * n - 1] += cy;
}
#define READY_WITH_mpn_sqr_basecase
#endif


#if ! defined (READY_WITH_mpn_sqr_basecase)

/* Default mpn_sqr_basecase using mpn_addmul_1.  */

void
mpn_sqr_basecase (mp_ptr rp, mp_srcptr up, mp_size_t n)
{
  mp_size_t i;

  ASSERT (n >= 1);
  ASSERT (! MPN_OVERLAP_P (rp, 2*n, up, n));

  {
    mp_limb_t ul, lpl;
    ul = up[0];
    umul_ppmm (rp[1], lpl, ul, ul << GMP_NAIL_BITS);
    rp[0] = lpl >> GMP_NAIL_BITS;
  }
  if (n > 1)
    {
      mp_limb_t tarr[2 * SQR_KARATSUBA_THRESHOLD];
      mp_ptr tp = tarr;
      mp_limb_t cy;

      /* must fit 2*n limbs in tarr */
      ASSERT (n <= SQR_KARATSUBA_THRESHOLD);

      cy = mpn_mul_1 (tp, up + 1, n - 1, up[0]);
      tp[n - 1] = cy;
      for (i = 2; i < n; i++)
	{
	  mp_limb_t cy;
	  cy = mpn_addmul_1 (tp + 2 * i - 2, up + i, n - i, up[i - 1]);
	  tp[n + i - 2] = cy;
	}
      MPN_SQR_DIAGONAL (rp + 2, up + 1, n - 1);

      {
	mp_limb_t cy;
#if HAVE_NATIVE_mpn_addlsh1_n
	cy = mpn_addlsh1_n (rp + 1, rp + 1, tp, 2 * n - 2);
#else
	cy = mpn_lshift (tp, tp, 2 * n - 2, 1);
	cy += mpn_add_n (rp + 1, rp + 1, tp, 2 * n - 2);
#endif
	rp[2 * n - 1] += cy;
      }
    }
}
#endif

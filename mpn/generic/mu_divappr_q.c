/* mpn_mu_divappr_q, mpn_preinv_mu_divappr_q.

   Compute Q = floor(N / D) + e.  N is nn limbs, D is dn limbs and must be
   normalized, and Q must be nn-dn limbs, 0 <= e <= 4.  The requirement that Q
   is nn-dn limbs (and not nn-dn+1 limbs) was put in place in order to allow us
   to let N be unmodified during the operation.

   Contributed to the GNU project by Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH A MUTABLE INTERFACE.  IT IS
   ONLY SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS
   ALMOST GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP
   RELEASE.

Copyright 2005, 2006, 2007 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option) any
later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */


/* We use the "misunderstanding algorithm" (MUA), discovered by Paul Zimmermann
   and Torbjorn Granlund when Torbjorn misunderstood Paul's explanation of
   Jebelean's bidirectional exact division algorithm.

   The idea of this algorithm is to compute a smaller inverted value than used
   in the standard Barrett algorithm, and thus save time in the Newton
   iterations, and pay just a small price when using the inverted value for
   developing quotient bits.

   Written by Torbjorn Granlund.  Paul Zimmermann suggested the use of the
   "wrap around" trick.  Based on the GMP divexact code and inspired by code
   contributed to GMP by Karl Hasselstroem.
*/


/* CAUTION: This code and the code in mu_div_qr.c should be edited in lockstep.

 Things to work on:

  * Passing k isn't a great interface.  Either 'in' should be passed, or
    determined by the code.

  * The current mpn_mu_div_qr_itch isn't exactly scientifically written.
    Scratch space buffer overruns are not unlikely before some analysis is
    applied.  Since scratch requirements are expected to change, such an
    analysis will have to wait til things settle.

  * This isn't optimal when the remainder isn't needed, since the final
    multiplication could be made special and take O(1) time on average, in that
    case.  This is particularly bad when qn << dn.  At some level, code as in
    GMP 4 mpn_tdiv_qr should be used, effectively dividing the leading 2qn
    dividend limbs by the qn divisor limbs.

  * This isn't optimal when the quotient isn't needed, as it might take a lot
    of space.  The computation is always needed, though, so there is not time
    to save with special code.

  * The itch/scratch scheme isn't perhaps such a good idea as it once seemed,
    demonstrated by the fact that the mpn_inv function's scratch needs means
    that we need to keep a large allocation long after it is needed.  Things
    are worse as mpn_mul_fft does not accept any scratch parameter, which means
    we'll have a large memory hole while in mpn_mul_fft.  In general, a peak
    scratch need in the beginning of a function isn't well-handled by the
    itch/scratch scheme.

  * Some ideas from comments in divexact.c apply to this code too.
*/

/* the NOSTAT stuff handles properly the case where files are concatenated */
#ifdef NOSTAT
#undef STAT
#endif

#ifdef STAT
#undef STAT
#define STAT(x) x
#else
#define NOSTAT
#define STAT(x)
#endif

#include "gmp.h"
#include "gmp-impl.h"


/* In case k=0 (automatic choice), we distinguish 3 cases:
   (a) dn < qn:         in = ceil(qn / ceil(qn/dn))
   (b) dn/3 < qn <= dn: in = ceil(qn / 2)
   (c) qn < dn/3:       in = qn
   In all cases we have in <= dn.
 */
mp_size_t
mpn_mu_divappr_q_choose_in (mp_size_t qn, mp_size_t dn, int k)
{
  mp_size_t in;

  if (k == 0)
    {
      mp_size_t b;
      if (qn > dn)
	{
	  /* Compute an inverse size that is a nice partition of the quotient.  */
	  b = (qn - 1) / dn + 1;	/* ceil(qn/dn), number of blocks */
	  in = (qn - 1) / b + 1;	/* ceil(qn/b) = ceil(qn / ceil(qn/dn)) */
	}
      else if (3 * qn > dn)
	{
	  in = (qn - 1) / 2 + 1;	/* b = 2 */
	}
      else
	{
	  in = (qn - 1) / 1 + 1;	/* b = 1 */
	}
    }
  else
    {
      mp_size_t xn;
      xn = MIN (dn, qn);
      in = (xn - 1) / k + 1;
    }

  return in;
}

mp_limb_t
mpn_mu_divappr_q (mp_ptr qp,
		  mp_ptr np,
		  mp_size_t nn,
		  mp_srcptr dp,
		  mp_size_t dn,
		  mp_ptr scratch)
{
  mp_size_t qn, in;
  mp_limb_t cy;
  mp_ptr ip, tp;

  /* FIXME: We should probably not handle tiny operands, but do it for now.  */
  if (dn == 1)
    {
      mpn_divrem_1 (scratch, 0L, np, nn, dp[0]);
      MPN_COPY (qp, scratch, nn - 1);
      return scratch[nn - 1];
    }

  qn = nn - dn;

#if 1
  /* If Q is smaller than D, truncate operands. */
  if (qn + 1 < dn)
    {
      np += dn - (qn + 1);
      nn -= dn - (qn + 1);
      dp += dn - (qn + 1);
      dn = qn + 1;

      /* Since D is cut here, we can have a carry in N'/D' even if we don't
	 have it for N/D.  */
      if (mpn_cmp (np + nn - (qn + 1), dp, qn + 1) >= 0)
	{ /* quotient is 111...111 */
	  mp_size_t i;
	  for (i = 0; i <= qn; i ++)
	    qp[i] = ~ (mp_limb_t) 0;
	  return 0;
	}
    }
#endif

  /* Compute the inverse size.  */
  in = mpn_mu_divappr_q_choose_in (qn, dn, 0);
  ASSERT (in <= dn);

#if 1
  /* This alternative inverse computation method gets slightly more accurate
     results.  FIXME: Temp allocation needs not analysed, itch function not
     adapted.  */
  ip = scratch;
  tp = scratch + in + 1;

  /* compute an approximate inverse on (in+1) limbs */
  if (dn == in)
    {
      MPN_COPY (tp + 1, dp, in);
      tp[0] = 1;
      mpn_invert (ip, tp, in + 1);
      MPN_COPY_INCR (ip, ip + 1, in);
    }
  else
    {
      cy = mpn_add_1 (tp, dp + dn - (in + 1), in + 1, 1);
      if (UNLIKELY (cy != 0))
	MPN_ZERO (ip, in);
      else
	{
	  mpn_invert (ip, tp, in + 1);
	  MPN_COPY_INCR (ip, ip + 1, in);
	}
    }
#else
  /* This older inverse computation method gets slightly worse results than the
     one above.  */
  ip = scratch;
  tp = scratch + in;

  /* Compute inverse of D to in+1 limbs, then round to 'in' limbs.  Ideally the
     inversion function should do this automatically.  */
  if (dn == in)
    {
      tp[in + 1] = 0;
      MPN_COPY (tp + in + 2, dp, in);
      mpn_invert (tp, tp + in + 1, in + 1);
    }
  else
    {
      mpn_invert (tp, dp + dn - (in + 1), in + 1);
    }
  cy = mpn_sub_1 (tp, tp, in + 1, GMP_NUMB_HIGHBIT);
  if (UNLIKELY (cy != 0))
    MPN_ZERO (tp + 1, in);
  MPN_COPY (ip, tp + 1, in);
#endif

/* We can't really handle qh = 1 like this since we'd here clobber N, which is
   not allowed in the way we've defined this function's API.  */
#if 0
  qh = mpn_cmp (np + qn, dp, dn) >= 0;
  if (qh != 0)
    mpn_sub_n (np + qn, np + qn, dp, dn);
#endif

  mpn_preinv_mu_divappr_q (qp, np, nn, dp, dn, ip, in, scratch + in);

/*  return qh; */
  return 0;
}

void
mpn_preinv_mu_divappr_q (mp_ptr qp,
			 mp_ptr np,
			 mp_size_t nn,
			 mp_srcptr dp,
			 mp_size_t dn,
			 mp_srcptr ip,
			 mp_size_t in,
			 mp_ptr scratch)
{
  mp_ptr rp;
  mp_size_t qn;
  mp_limb_t cy;
  mp_ptr tp;
  mp_limb_t r;

  qn = nn - dn;

  if (qn == 0)
    return;

  rp = scratch;
  tp = scratch + dn;

  np += qn;
  qp += qn;

  MPN_COPY (rp, np, dn);

  while (qn > 0)
    {
      if (qn < in)
	{
	  ip += in - qn;
	  in = qn;
	}
      np -= in;
      qp -= in;

      /* Compute the next block of quotient limbs by multiplying the inverse I
	 by the upper part of the partial remainder R.  */
      mpn_mul_n (tp, rp + dn - in, ip, in);		/* mulhi  */
      cy = mpn_add_n (qp, tp + in, rp + dn - in, in);	/* I's msb implicit */
      ASSERT_ALWAYS (cy == 0);			/* FIXME */

      qn -= in;
      if (qn == 0)
	break;

      /* Compute the product of the quotient block and the divisor D, to be
	 subtracted from the partial remainder combined with new limbs from the
	 dividend N.  We only really need the low dn limbs.  */
#if WANT_FFT
      if (ABOVE_THRESHOLD (dn, MUL_FFT_MODF_THRESHOLD))
	{
	  /* Use the wrap-around trick.  */
	  mp_size_t m, wn;
	  int k;

	  k = mpn_fft_best_k (dn + 1, 0);
	  m = mpn_fft_next_size (dn + 1, k);
	  wn = dn + in - m;			/* number of wrapped limbs */

	  mpn_mul_fft (tp, m, dp, dn, qp, in, k);

	  if (wn > 0)
	    {
	      cy = mpn_add_n (tp, tp, rp + dn - wn, wn);
	      mpn_incr_u (tp + wn, cy);

	      cy = mpn_cmp (rp + dn - in, tp + dn, m - dn) < 0;
	      mpn_decr_u (tp, cy);
	    }
	}
      else
#endif
	mpn_mul (tp, dp, dn, qp, in);		/* dn+in limbs, high 'in' cancels */

      r = rp[dn - in] - tp[dn];

      /* Subtract the product from the partial remainder combined with new
	 limbs from the dividend N, generating a new partial remainder R.  */
      if (dn != in)
	{
	  cy = mpn_sub_n (tp, np, tp, in);	/* get next 'in' limbs from N */
	  cy = mpn_sub_nc (tp + in, rp, tp + in, dn - in, cy);
	  MPN_COPY (rp, tp, dn);		/* FIXME: try to avoid this */
	}
      else
	{
	  cy = mpn_sub_n (rp, np, tp, in);	/* get next 'in' limbs from N */
	}

      STAT (int i; int err = 0;
	    static int errarr[5]; static int err_rec; static int tot);

      /* Check the remainder R and adjust the quotient as needed.  */
      r -= cy;
      while (r != 0)
	{
	  /* We loop 0 times with about 69% probability, 1 time with about 31%
	     probability, 2 times with about 0.6% probability, if inverse is
	     computed as recommended.  */
	  mpn_incr_u (qp, 1);
	  cy = mpn_sub_n (rp, rp, dp, dn);
	  r -= cy;
	  STAT (err++);
	}
      if (mpn_cmp (rp, dp, dn) >= 0)
	{
	  /* This is executed with about 76% probability.  */
	  mpn_incr_u (qp, 1);
	  cy = mpn_sub_n (rp, rp, dp, dn);
	  STAT (err++);
	}

      STAT (
	    tot++;
	    errarr[err]++;
	    if (err > err_rec)
	      err_rec = err;
	    if (tot % 0x10000 == 0)
	      {
		for (i = 0; i <= err_rec; i++)
		  printf ("  %d(%.1f%%)", errarr[i], 100.0*errarr[i]/tot);
		printf ("\n");
	      }
	    );
    }

  /* FIXME: We should perhaps be somewhat more elegant in our rounding of the
     quotient.  For now, just make sure the returned quotient is >= the real
     quotient.  */
  qn = nn - dn;
  cy = mpn_add_1 (qp, qp, qn, 3);
  if (cy != 0)
    {
      MPN_ZERO (qp, qn);
      mpn_sub_1 (qp, qp, qn, 1);
    }
}

mp_size_t
mpn_mu_divappr_q_itch (mp_size_t nn, mp_size_t dn, int mua_k)
{
  mp_size_t qn, m;
  int k;

  /* FIXME: This isn't very carefully written, and might grossly overestimate
     the amount of scratch needed, and might perhaps also underestimate it,
     leading to potential buffer overruns.  In particular k=0 might lead to
     gross overestimates.  */

  if (dn == 1)
    return nn;

  qn = nn - dn;
  if (qn >= dn)
    {
      k = mpn_fft_best_k (dn + 1, 0);
      m = mpn_fft_next_size (dn + 1, k);
      return dn + (mua_k <= 1
		   ? 6 * dn
		   : m + 2 * dn);
    }
  else
    {
      k = mpn_fft_best_k (dn + 1, 0);
      m = mpn_fft_next_size (dn + 1, k);
      return dn + (mua_k <= 1
		   ? m + 4 * qn
		   : m + 2 * qn);
    }
}

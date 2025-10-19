/*
 * fft_r4_cmsis.c - Radix 4 FFT, kanged from CMSIS
 * 10-02-25 E. Brombaugh
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define PI (4.0F*atanf(1))

#define ARM_MATH_LOOPUNROLL

static float *twiddles;
static int *brtbl;
static int fftlen;

/**
  brief         Core function for the floating-point CFFT butterfly process.
  param[in,out] pSrc             points to the in-place buffer of floating-point data type
  param[in]     fftLen           length of the FFT
  param[in]     pCoef            points to the twiddle coefficient buffer
  param[in]     twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
  return        none
 */
void arm_radix4_butterfly_f32(
        float * pSrc,
        uint16_t fftLen,
  const float * pCoef,
        uint16_t twidCoefModifier)
{
        float co1, co2, co3, si1, si2, si3;
        uint32_t ia1, ia2, ia3;
        uint32_t i0, i1, i2, i3;
        uint32_t n1, n2, j, k;

#if defined (ARM_MATH_LOOPUNROLL)

        float xaIn, yaIn, xbIn, ybIn, xcIn, ycIn, xdIn, ydIn;
        float Xaplusc, Xbplusd, Yaplusc, Ybplusd, Xaminusc, Xbminusd, Yaminusc,
        Ybminusd;
        float Xb12C_out, Yb12C_out, Xc12C_out, Yc12C_out, Xd12C_out, Yd12C_out;
        float Xb12_out, Yb12_out, Xc12_out, Yc12_out, Xd12_out, Yd12_out;
        float *ptr1;
        float p0,p1,p2,p3,p4,p5;
        float a0,a1,a2,a3,a4,a5,a6,a7;

   /*  Initializations for the first stage */
   n2 = fftLen;
   n1 = n2;

   /* n2 = fftLen/4 */
   n2 >>= 2U;
   i0 = 0U;
   ia1 = 0U;

   j = n2;

   /*  Calculation of first stage */
   do
   {
      /*  index calculation for the input as, */
      /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
      i1 = i0 + n2;
      i2 = i1 + n2;
      i3 = i2 + n2;

      xaIn = pSrc[(2U * i0)];
      yaIn = pSrc[(2U * i0) + 1U];

      xbIn = pSrc[(2U * i1)];
      ybIn = pSrc[(2U * i1) + 1U];

      xcIn = pSrc[(2U * i2)];
      ycIn = pSrc[(2U * i2) + 1U];

      xdIn = pSrc[(2U * i3)];
      ydIn = pSrc[(2U * i3) + 1U];

      /* xa + xc */
      Xaplusc = xaIn + xcIn;
      /* xb + xd */
      Xbplusd = xbIn + xdIn;
      /* ya + yc */
      Yaplusc = yaIn + ycIn;
      /* yb + yd */
      Ybplusd = ybIn + ydIn;

      /*  index calculation for the coefficients */
      ia2 = ia1 + ia1;
      co2 = pCoef[ia2 * 2U];
      si2 = pCoef[(ia2 * 2U) + 1U];

      /* xa - xc */
      Xaminusc = xaIn - xcIn;
      /* xb - xd */
      Xbminusd = xbIn - xdIn;
      /* ya - yc */
      Yaminusc = yaIn - ycIn;
      /* yb - yd */
      Ybminusd = ybIn - ydIn;

      /* xa' = xa + xb + xc + xd */
      pSrc[(2U * i0)] = Xaplusc + Xbplusd;
      /* ya' = ya + yb + yc + yd */
      pSrc[(2U * i0) + 1U] = Yaplusc + Ybplusd;

      /* (xa - xc) + (yb - yd) */
      Xb12C_out = (Xaminusc + Ybminusd);
      /* (ya - yc) + (xb - xd) */
      Yb12C_out = (Yaminusc - Xbminusd);
      /* (xa + xc) - (xb + xd) */
      Xc12C_out = (Xaplusc - Xbplusd);
      /* (ya + yc) - (yb + yd) */
      Yc12C_out = (Yaplusc - Ybplusd);
      /* (xa - xc) - (yb - yd) */
      Xd12C_out = (Xaminusc - Ybminusd);
      /* (ya - yc) + (xb - xd) */
      Yd12C_out = (Xbminusd + Yaminusc);

      co1 = pCoef[ia1 * 2U];
      si1 = pCoef[(ia1 * 2U) + 1U];

      /*  index calculation for the coefficients */
      ia3 = ia2 + ia1;
      co3 = pCoef[ia3 * 2U];
      si3 = pCoef[(ia3 * 2U) + 1U];

      Xb12_out = Xb12C_out * co1;
      Yb12_out = Yb12C_out * co1;
      Xc12_out = Xc12C_out * co2;
      Yc12_out = Yc12C_out * co2;
      Xd12_out = Xd12C_out * co3;
      Yd12_out = Yd12C_out * co3;

      /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
      //Xb12_out -= Yb12C_out * si1;
      p0 = Yb12C_out * si1;
      /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
      //Yb12_out += Xb12C_out * si1;
      p1 = Xb12C_out * si1;
      /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
      //Xc12_out -= Yc12C_out * si2;
      p2 = Yc12C_out * si2;
      /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
      //Yc12_out += Xc12C_out * si2;
      p3 = Xc12C_out * si2;
      /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
      //Xd12_out -= Yd12C_out * si3;
      p4 = Yd12C_out * si3;
      /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
      //Yd12_out += Xd12C_out * si3;
      p5 = Xd12C_out * si3;

      Xb12_out += p0;
      Yb12_out -= p1;
      Xc12_out += p2;
      Yc12_out -= p3;
      Xd12_out += p4;
      Yd12_out -= p5;

      /* xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
      pSrc[2U * i1] = Xc12_out;

      /* yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
      pSrc[(2U * i1) + 1U] = Yc12_out;

      /* xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
      pSrc[2U * i2] = Xb12_out;

      /* yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
      pSrc[(2U * i2) + 1U] = Yb12_out;

      /* xd' = (xa-yb-xc+yd)co3 + (ya+xb-yc-xd)(si3) */
      pSrc[2U * i3] = Xd12_out;

      /* yd' = (ya+xb-yc-xd)co3 - (xa-yb-xc+yd)(si3) */
      pSrc[(2U * i3) + 1U] = Yd12_out;

      /*  Twiddle coefficients index modifier */
      ia1 += twidCoefModifier;

      /*  Updating input index */
      i0++;

   }
   while (--j);

   twidCoefModifier <<= 2U;

   /*  Calculation of second stage to excluding last stage */
   for (k = fftLen >> 2U; k > 4U; k >>= 2U)
   {
      /*  Initializations for the first stage */
      n1 = n2;
      n2 >>= 2U;
      ia1 = 0U;

      /*  Calculation of first stage */
      j = 0;
      do
      {
         /*  index calculation for the coefficients */
         ia2 = ia1 + ia1;
         ia3 = ia2 + ia1;
         co1 = pCoef[(ia1 * 2U)];
         si1 = pCoef[(ia1 * 2U) + 1U];
         co2 = pCoef[(ia2 * 2U)];
         si2 = pCoef[(ia2 * 2U) + 1U];
         co3 = pCoef[(ia3 * 2U)];
         si3 = pCoef[(ia3 * 2U) + 1U];

         /*  Twiddle coefficients index modifier */
         ia1 += twidCoefModifier;

         i0 = j;
         do
         {
            /*  index calculation for the input as, */
            /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
            i1 = i0 + n2;
            i2 = i1 + n2;
            i3 = i2 + n2;

            xaIn = pSrc[(2U * i0)];
            yaIn = pSrc[(2U * i0) + 1U];

            xbIn = pSrc[(2U * i1)];
            ybIn = pSrc[(2U * i1) + 1U];

            xcIn = pSrc[(2U * i2)];
            ycIn = pSrc[(2U * i2) + 1U];

            xdIn = pSrc[(2U * i3)];
            ydIn = pSrc[(2U * i3) + 1U];

            /* xa - xc */
            Xaminusc = xaIn - xcIn;
            /* (xb - xd) */
            Xbminusd = xbIn - xdIn;
            /* ya - yc */
            Yaminusc = yaIn - ycIn;
            /* (yb - yd) */
            Ybminusd = ybIn - ydIn;

            /* xa + xc */
            Xaplusc = xaIn + xcIn;
            /* xb + xd */
            Xbplusd = xbIn + xdIn;
            /* ya + yc */
            Yaplusc = yaIn + ycIn;
            /* yb + yd */
            Ybplusd = ybIn + ydIn;

            /* (xa - xc) + (yb - yd) */
            Xb12C_out = (Xaminusc + Ybminusd);
            /* (ya - yc) -  (xb - xd) */
            Yb12C_out = (Yaminusc - Xbminusd);
            /* xa + xc -(xb + xd) */
            Xc12C_out = (Xaplusc - Xbplusd);
            /* (ya + yc) - (yb + yd) */
            Yc12C_out = (Yaplusc - Ybplusd);
            /* (xa - xc) - (yb - yd) */
            Xd12C_out = (Xaminusc - Ybminusd);
            /* (ya - yc) +  (xb - xd) */
            Yd12C_out = (Xbminusd + Yaminusc);

            pSrc[(2U * i0)] = Xaplusc + Xbplusd;
            pSrc[(2U * i0) + 1U] = Yaplusc + Ybplusd;

            Xb12_out = Xb12C_out * co1;
            Yb12_out = Yb12C_out * co1;
            Xc12_out = Xc12C_out * co2;
            Yc12_out = Yc12C_out * co2;
            Xd12_out = Xd12C_out * co3;
            Yd12_out = Yd12C_out * co3;

            /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
            //Xb12_out -= Yb12C_out * si1;
            p0 = Yb12C_out * si1;
            /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
            //Yb12_out += Xb12C_out * si1;
            p1 = Xb12C_out * si1;
            /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
            //Xc12_out -= Yc12C_out * si2;
            p2 = Yc12C_out * si2;
            /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
            //Yc12_out += Xc12C_out * si2;
            p3 = Xc12C_out * si2;
            /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
            //Xd12_out -= Yd12C_out * si3;
            p4 = Yd12C_out * si3;
            /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
            //Yd12_out += Xd12C_out * si3;
            p5 = Xd12C_out * si3;

            Xb12_out += p0;
            Yb12_out -= p1;
            Xc12_out += p2;
            Yc12_out -= p3;
            Xd12_out += p4;
            Yd12_out -= p5;

            /* xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
            pSrc[2U * i1] = Xc12_out;

            /* yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
            pSrc[(2U * i1) + 1U] = Yc12_out;

            /* xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
            pSrc[2U * i2] = Xb12_out;

            /* yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
            pSrc[(2U * i2) + 1U] = Yb12_out;

            /* xd' = (xa-yb-xc+yd)co3 + (ya+xb-yc-xd)(si3) */
            pSrc[2U * i3] = Xd12_out;

            /* yd' = (ya+xb-yc-xd)co3 - (xa-yb-xc+yd)(si3) */
            pSrc[(2U * i3) + 1U] = Yd12_out;

            i0 += n1;
         } while (i0 < fftLen);
         j++;
      } while (j <= (n2 - 1U));
      twidCoefModifier <<= 2U;
   }

   j = fftLen >> 2;
   ptr1 = &pSrc[0];

   /*  Calculations of last stage */
   do
   {
      xaIn = ptr1[0];
      yaIn = ptr1[1];
      xbIn = ptr1[2];
      ybIn = ptr1[3];
      xcIn = ptr1[4];
      ycIn = ptr1[5];
      xdIn = ptr1[6];
      ydIn = ptr1[7];

      /* xa + xc */
      Xaplusc = xaIn + xcIn;

      /* xa - xc */
      Xaminusc = xaIn - xcIn;

      /* ya + yc */
      Yaplusc = yaIn + ycIn;

      /* ya - yc */
      Yaminusc = yaIn - ycIn;

      /* xb + xd */
      Xbplusd = xbIn + xdIn;

      /* yb + yd */
      Ybplusd = ybIn + ydIn;

      /* (xb-xd) */
      Xbminusd = xbIn - xdIn;

      /* (yb-yd) */
      Ybminusd = ybIn - ydIn;

      /* xa' = xa + xb + xc + xd */
      a0 = (Xaplusc + Xbplusd);
      /* ya' = ya + yb + yc + yd */
      a1 = (Yaplusc + Ybplusd);
      /* xc' = (xa-xb+xc-xd) */
      a2 = (Xaplusc - Xbplusd);
      /* yc' = (ya-yb+yc-yd) */
      a3 = (Yaplusc - Ybplusd);
      /* xb' = (xa+yb-xc-yd) */
      a4 = (Xaminusc + Ybminusd);
      /* yb' = (ya-xb-yc+xd) */
      a5 = (Yaminusc - Xbminusd);
      /* xd' = (xa-yb-xc+yd)) */
      a6 = (Xaminusc - Ybminusd);
      /* yd' = (ya+xb-yc-xd) */
      a7 = (Xbminusd + Yaminusc);

      ptr1[0] = a0;
      ptr1[1] = a1;
      ptr1[2] = a2;
      ptr1[3] = a3;
      ptr1[4] = a4;
      ptr1[5] = a5;
      ptr1[6] = a6;
      ptr1[7] = a7;

      /* increment pointer by 8 */
      ptr1 += 8U;
   } while (--j);

#else

	float t1, t2, r1, r2, s1, s2;

	/* Initializations for the fft calculation */
	n2 = fftLen;
	n1 = n2;
	for (k = fftLen; k > 1U; k >>= 2U)
	{
		/*  Initializations for the fft calculation */
		n1 = n2;
		n2 >>= 2U;
		ia1 = 0U;

		/*  FFT Calculation */
		j = 0;
		do
		{
			/*  index calculation for the coefficients */
			ia2 = ia1 + ia1;
			ia3 = ia2 + ia1;
			co1 = pCoef[ia1 * 2U];
			si1 = pCoef[(ia1 * 2U) + 1U];
			co2 = pCoef[ia2 * 2U];
			si2 = pCoef[(ia2 * 2U) + 1U];
			co3 = pCoef[ia3 * 2U];
			si3 = pCoef[(ia3 * 2U) + 1U];

			/*  Twiddle coefficients index modifier */
			ia1 = ia1 + twidCoefModifier;

			i0 = j;
			do
			{
				/*  index calculation for the input as, */
				/*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
				i1 = i0 + n2;
				i2 = i1 + n2;
				i3 = i2 + n2;

				/* xa + xc */
				r1 = pSrc[(2U * i0)] + pSrc[(2U * i2)];

				/* xa - xc */
				r2 = pSrc[(2U * i0)] - pSrc[(2U * i2)];

				/* ya + yc */
				s1 = pSrc[(2U * i0) + 1U] + pSrc[(2U * i2) + 1U];

				/* ya - yc */
				s2 = pSrc[(2U * i0) + 1U] - pSrc[(2U * i2) + 1U];

				/* xb + xd */
				t1 = pSrc[2U * i1] + pSrc[2U * i3];

				/* xa' = xa + xb + xc + xd */
				pSrc[2U * i0] = r1 + t1;

				/* yb + yd */
				t2 = pSrc[(2U * i1) + 1U] + pSrc[(2U * i3) + 1U];

				/* ya' = ya + yb + yc + yd */
				pSrc[(2U * i0) + 1U] = s1 + t2;

				/* xa + xc -(xb + xd) */
				r1 = r1 - t1;

				/* (ya + yc) - (yb + yd) */
				s1 = s1 - t2;

				/* (yb - yd) */
				t1 = pSrc[(2U * i1) + 1U] - pSrc[(2U * i3) + 1U];

				/* (xb - xd) */
				t2 = pSrc[2U * i1] - pSrc[2U * i3];

				/* xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
				pSrc[2U * i1] = (r1 * co2) + (s1 * si2);

				/* yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
				pSrc[(2U * i1) + 1U] = (s1 * co2) - (r1 * si2);

				/* (xa - xc) + (yb - yd) */
				r1 = r2 + t1;

				/* (xa - xc) - (yb - yd) */
				r2 = r2 - t1;

				/* (ya - yc) -  (xb - xd) */
				s1 = s2 - t2;

				/* (ya - yc) +  (xb - xd) */
				s2 = s2 + t2;

				/* xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
				pSrc[2U * i2] = (r1 * co1) + (s1 * si1);

				/* yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
				pSrc[(2U * i2) + 1U] = (s1 * co1) - (r1 * si1);

				/* xd' = (xa-yb-xc+yd)co3 + (ya+xb-yc-xd)(si3) */
				pSrc[2U * i3] = (r2 * co3) + (s2 * si3);

				/* yd' = (ya+xb-yc-xd)co3 - (xa-yb-xc+yd)(si3) */
				pSrc[(2U * i3) + 1U] = (s2 * co3) - (r2 * si3);

				i0 += n1;
			} while ( i0 < fftLen);
			j++;
		} while (j <= (n2 - 1U));
		twidCoefModifier <<= 2U;
	}
#endif /* #if defined (ARM_MATH_LOOPUNROLL) */
}

/**
  brief         Core function for the floating-point CIFFT butterfly process.
  param[in,out] pSrc             points to the in-place buffer of floating-point data type
  param[in]     fftLen           length of the FFT
  param[in]     pCoef            points to twiddle coefficient buffer
  param[in]     twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.
  param[in]     onebyfftLen      value of 1/fftLen
  return        none
 */
void arm_radix4_butterfly_inverse_f32(
        float * pSrc,
        uint16_t fftLen,
  const float * pCoef,
        uint16_t twidCoefModifier,
        float onebyfftLen)
{
        float co1, co2, co3, si1, si2, si3;
        uint32_t ia1, ia2, ia3;
        uint32_t i0, i1, i2, i3;
        uint32_t n1, n2, j, k;

#if defined (ARM_MATH_LOOPUNROLL)

        float xaIn, yaIn, xbIn, ybIn, xcIn, ycIn, xdIn, ydIn;
        float Xaplusc, Xbplusd, Yaplusc, Ybplusd, Xaminusc, Xbminusd, Yaminusc,
        Ybminusd;
        float Xb12C_out, Yb12C_out, Xc12C_out, Yc12C_out, Xd12C_out, Yd12C_out;
        float Xb12_out, Yb12_out, Xc12_out, Yc12_out, Xd12_out, Yd12_out;
        float *ptr1;
        float p0,p1,p2,p3,p4,p5,p6,p7;
        float a0,a1,a2,a3,a4,a5,a6,a7;


   /*  Initializations for the first stage */
   n2 = fftLen;
   n1 = n2;

   /* n2 = fftLen/4 */
   n2 >>= 2U;
   i0 = 0U;
   ia1 = 0U;

   j = n2;

   /*  Calculation of first stage */
   do
   {
      /*  index calculation for the input as, */
      /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
      i1 = i0 + n2;
      i2 = i1 + n2;
      i3 = i2 + n2;

      /*  Butterfly implementation */
      xaIn = pSrc[(2U * i0)];
      yaIn = pSrc[(2U * i0) + 1U];

      xcIn = pSrc[(2U * i2)];
      ycIn = pSrc[(2U * i2) + 1U];

      xbIn = pSrc[(2U * i1)];
      ybIn = pSrc[(2U * i1) + 1U];

      xdIn = pSrc[(2U * i3)];
      ydIn = pSrc[(2U * i3) + 1U];

      /* xa + xc */
      Xaplusc = xaIn + xcIn;
      /* xb + xd */
      Xbplusd = xbIn + xdIn;
      /* ya + yc */
      Yaplusc = yaIn + ycIn;
      /* yb + yd */
      Ybplusd = ybIn + ydIn;

      /*  index calculation for the coefficients */
      ia2 = ia1 + ia1;
      co2 = pCoef[ia2 * 2U];
      si2 = pCoef[(ia2 * 2U) + 1U];

      /* xa - xc */
      Xaminusc = xaIn - xcIn;
      /* xb - xd */
      Xbminusd = xbIn - xdIn;
      /* ya - yc */
      Yaminusc = yaIn - ycIn;
      /* yb - yd */
      Ybminusd = ybIn - ydIn;

      /* xa' = xa + xb + xc + xd */
      pSrc[(2U * i0)] = Xaplusc + Xbplusd;

      /* ya' = ya + yb + yc + yd */
      pSrc[(2U * i0) + 1U] = Yaplusc + Ybplusd;

      /* (xa - xc) - (yb - yd) */
      Xb12C_out = (Xaminusc - Ybminusd);
      /* (ya - yc) + (xb - xd) */
      Yb12C_out = (Yaminusc + Xbminusd);
      /* (xa + xc) - (xb + xd) */
      Xc12C_out = (Xaplusc - Xbplusd);
      /* (ya + yc) - (yb + yd) */
      Yc12C_out = (Yaplusc - Ybplusd);
      /* (xa - xc) + (yb - yd) */
      Xd12C_out = (Xaminusc + Ybminusd);
      /* (ya - yc) - (xb - xd) */
      Yd12C_out = (Yaminusc - Xbminusd);

      co1 = pCoef[ia1 * 2U];
      si1 = pCoef[(ia1 * 2U) + 1U];

      /*  index calculation for the coefficients */
      ia3 = ia2 + ia1;
      co3 = pCoef[ia3 * 2U];
      si3 = pCoef[(ia3 * 2U) + 1U];

      Xb12_out = Xb12C_out * co1;
      Yb12_out = Yb12C_out * co1;
      Xc12_out = Xc12C_out * co2;
      Yc12_out = Yc12C_out * co2;
      Xd12_out = Xd12C_out * co3;
      Yd12_out = Yd12C_out * co3;

      /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
      //Xb12_out -= Yb12C_out * si1;
      p0 = Yb12C_out * si1;
      /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
      //Yb12_out += Xb12C_out * si1;
      p1 = Xb12C_out * si1;
      /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
      //Xc12_out -= Yc12C_out * si2;
      p2 = Yc12C_out * si2;
      /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
      //Yc12_out += Xc12C_out * si2;
      p3 = Xc12C_out * si2;
      /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
      //Xd12_out -= Yd12C_out * si3;
      p4 = Yd12C_out * si3;
      /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
      //Yd12_out += Xd12C_out * si3;
      p5 = Xd12C_out * si3;

      Xb12_out -= p0;
      Yb12_out += p1;
      Xc12_out -= p2;
      Yc12_out += p3;
      Xd12_out -= p4;
      Yd12_out += p5;

      /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
      pSrc[2U * i1] = Xc12_out;

      /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
      pSrc[(2U * i1) + 1U] = Yc12_out;

      /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
      pSrc[2U * i2] = Xb12_out;

      /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
      pSrc[(2U * i2) + 1U] = Yb12_out;

      /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
      pSrc[2U * i3] = Xd12_out;

      /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
      pSrc[(2U * i3) + 1U] = Yd12_out;

      /*  Twiddle coefficients index modifier */
      ia1 = ia1 + twidCoefModifier;

      /*  Updating input index */
      i0 = i0 + 1U;

   } while (--j);

   twidCoefModifier <<= 2U;

   /*  Calculation of second stage to excluding last stage */
   for (k = fftLen >> 2U; k > 4U; k >>= 2U)
   {
      /*  Initializations for the first stage */
      n1 = n2;
      n2 >>= 2U;
      ia1 = 0U;

      /*  Calculation of first stage */
      j = 0;
      do
      {
         /*  index calculation for the coefficients */
         ia2 = ia1 + ia1;
         ia3 = ia2 + ia1;
         co1 = pCoef[ia1 * 2U];
         si1 = pCoef[(ia1 * 2U) + 1U];
         co2 = pCoef[ia2 * 2U];
         si2 = pCoef[(ia2 * 2U) + 1U];
         co3 = pCoef[ia3 * 2U];
         si3 = pCoef[(ia3 * 2U) + 1U];

         /*  Twiddle coefficients index modifier */
         ia1 = ia1 + twidCoefModifier;

         i0 = j;
         do
         {
            /*  index calculation for the input as, */
            /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
            i1 = i0 + n2;
            i2 = i1 + n2;
            i3 = i2 + n2;

            xaIn = pSrc[(2U * i0)];
            yaIn = pSrc[(2U * i0) + 1U];

            xbIn = pSrc[(2U * i1)];
            ybIn = pSrc[(2U * i1) + 1U];

            xcIn = pSrc[(2U * i2)];
            ycIn = pSrc[(2U * i2) + 1U];

            xdIn = pSrc[(2U * i3)];
            ydIn = pSrc[(2U * i3) + 1U];

            /* xa - xc */
            Xaminusc = xaIn - xcIn;
            /* (xb - xd) */
            Xbminusd = xbIn - xdIn;
            /* ya - yc */
            Yaminusc = yaIn - ycIn;
            /* (yb - yd) */
            Ybminusd = ybIn - ydIn;

            /* xa + xc */
            Xaplusc = xaIn + xcIn;
            /* xb + xd */
            Xbplusd = xbIn + xdIn;
            /* ya + yc */
            Yaplusc = yaIn + ycIn;
            /* yb + yd */
            Ybplusd = ybIn + ydIn;

            /* (xa - xc) - (yb - yd) */
            Xb12C_out = (Xaminusc - Ybminusd);
            /* (ya - yc) +  (xb - xd) */
            Yb12C_out = (Yaminusc + Xbminusd);
            /* xa + xc -(xb + xd) */
            Xc12C_out = (Xaplusc - Xbplusd);
            /* (ya + yc) - (yb + yd) */
            Yc12C_out = (Yaplusc - Ybplusd);
            /* (xa - xc) + (yb - yd) */
            Xd12C_out = (Xaminusc + Ybminusd);
            /* (ya - yc) -  (xb - xd) */
            Yd12C_out = (Yaminusc - Xbminusd);

            pSrc[(2U * i0)] = Xaplusc + Xbplusd;
            pSrc[(2U * i0) + 1U] = Yaplusc + Ybplusd;

            Xb12_out = Xb12C_out * co1;
            Yb12_out = Yb12C_out * co1;
            Xc12_out = Xc12C_out * co2;
            Yc12_out = Yc12C_out * co2;
            Xd12_out = Xd12C_out * co3;
            Yd12_out = Yd12C_out * co3;

            /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
            //Xb12_out -= Yb12C_out * si1;
            p0 = Yb12C_out * si1;
            /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
            //Yb12_out += Xb12C_out * si1;
            p1 = Xb12C_out * si1;
            /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
            //Xc12_out -= Yc12C_out * si2;
            p2 = Yc12C_out * si2;
            /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
            //Yc12_out += Xc12C_out * si2;
            p3 = Xc12C_out * si2;
            /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
            //Xd12_out -= Yd12C_out * si3;
            p4 = Yd12C_out * si3;
            /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
            //Yd12_out += Xd12C_out * si3;
            p5 = Xd12C_out * si3;

            Xb12_out -= p0;
            Yb12_out += p1;
            Xc12_out -= p2;
            Yc12_out += p3;
            Xd12_out -= p4;
            Yd12_out += p5;

            /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
            pSrc[2U * i1] = Xc12_out;

            /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
            pSrc[(2U * i1) + 1U] = Yc12_out;

            /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
            pSrc[2U * i2] = Xb12_out;

            /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
            pSrc[(2U * i2) + 1U] = Yb12_out;

            /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
            pSrc[2U * i3] = Xd12_out;

            /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
            pSrc[(2U * i3) + 1U] = Yd12_out;

            i0 += n1;
         } while (i0 < fftLen);
         j++;
      } while (j <= (n2 - 1U));
      twidCoefModifier <<= 2U;
   }
   /*  Initializations of last stage */

   j = fftLen >> 2;
   ptr1 = &pSrc[0];

   /*  Calculations of last stage */
   do
   {
      xaIn = ptr1[0];
      yaIn = ptr1[1];
      xbIn = ptr1[2];
      ybIn = ptr1[3];
      xcIn = ptr1[4];
      ycIn = ptr1[5];
      xdIn = ptr1[6];
      ydIn = ptr1[7];

      /*  Butterfly implementation */
      /* xa + xc */
      Xaplusc = xaIn + xcIn;

      /* xa - xc */
      Xaminusc = xaIn - xcIn;

      /* ya + yc */
      Yaplusc = yaIn + ycIn;

      /* ya - yc */
      Yaminusc = yaIn - ycIn;

      /* xb + xd */
      Xbplusd = xbIn + xdIn;

      /* yb + yd */
      Ybplusd = ybIn + ydIn;

      /* (xb-xd) */
      Xbminusd = xbIn - xdIn;

      /* (yb-yd) */
      Ybminusd = ybIn - ydIn;

      /* xa' = (xa+xb+xc+xd) * onebyfftLen */
      a0 = (Xaplusc + Xbplusd);
      /* ya' = (ya+yb+yc+yd) * onebyfftLen */
      a1 = (Yaplusc + Ybplusd);
      /* xc' = (xa-xb+xc-xd) * onebyfftLen */
      a2 = (Xaplusc - Xbplusd);
      /* yc' = (ya-yb+yc-yd) * onebyfftLen  */
      a3 = (Yaplusc - Ybplusd);
      /* xb' = (xa-yb-xc+yd) * onebyfftLen */
      a4 = (Xaminusc - Ybminusd);
      /* yb' = (ya+xb-yc-xd) * onebyfftLen */
      a5 = (Yaminusc + Xbminusd);
      /* xd' = (xa-yb-xc+yd) * onebyfftLen */
      a6 = (Xaminusc + Ybminusd);
      /* yd' = (ya-xb-yc+xd) * onebyfftLen */
      a7 = (Yaminusc - Xbminusd);

      p0 = a0 * onebyfftLen;
      p1 = a1 * onebyfftLen;
      p2 = a2 * onebyfftLen;
      p3 = a3 * onebyfftLen;
      p4 = a4 * onebyfftLen;
      p5 = a5 * onebyfftLen;
      p6 = a6 * onebyfftLen;
      p7 = a7 * onebyfftLen;

      /* xa' = (xa+xb+xc+xd) * onebyfftLen */
      ptr1[0] = p0;
      /* ya' = (ya+yb+yc+yd) * onebyfftLen */
      ptr1[1] = p1;
      /* xc' = (xa-xb+xc-xd) * onebyfftLen */
      ptr1[2] = p2;
      /* yc' = (ya-yb+yc-yd) * onebyfftLen  */
      ptr1[3] = p3;
      /* xb' = (xa-yb-xc+yd) * onebyfftLen */
      ptr1[4] = p4;
      /* yb' = (ya+xb-yc-xd) * onebyfftLen */
      ptr1[5] = p5;
      /* xd' = (xa-yb-xc+yd) * onebyfftLen */
      ptr1[6] = p6;
      /* yd' = (ya-xb-yc+xd) * onebyfftLen */
      ptr1[7] = p7;

      /* increment source pointer by 8 for next calculations */
      ptr1 = ptr1 + 8U;

   } while (--j);

#else

        float t1, t2, r1, r2, s1, s2;

   /*  Initializations for the first stage */
   n2 = fftLen;
   n1 = n2;

   /*  Calculation of first stage */
   for (k = fftLen; k > 4U; k >>= 2U)
   {
      /*  Initializations for the first stage */
      n1 = n2;
      n2 >>= 2U;
      ia1 = 0U;

      /*  Calculation of first stage */
      j = 0;
      do
      {
         /*  index calculation for the coefficients */
         ia2 = ia1 + ia1;
         ia3 = ia2 + ia1;
         co1 = pCoef[ia1 * 2U];
         si1 = pCoef[(ia1 * 2U) + 1U];
         co2 = pCoef[ia2 * 2U];
         si2 = pCoef[(ia2 * 2U) + 1U];
         co3 = pCoef[ia3 * 2U];
         si3 = pCoef[(ia3 * 2U) + 1U];

         /*  Twiddle coefficients index modifier */
         ia1 = ia1 + twidCoefModifier;

         i0 = j;
         do
         {
            /*  index calculation for the input as, */
            /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
            i1 = i0 + n2;
            i2 = i1 + n2;
            i3 = i2 + n2;

            /* xa + xc */
            r1 = pSrc[(2U * i0)] + pSrc[(2U * i2)];

            /* xa - xc */
            r2 = pSrc[(2U * i0)] - pSrc[(2U * i2)];

            /* ya + yc */
            s1 = pSrc[(2U * i0) + 1U] + pSrc[(2U * i2) + 1U];

            /* ya - yc */
            s2 = pSrc[(2U * i0) + 1U] - pSrc[(2U * i2) + 1U];

            /* xb + xd */
            t1 = pSrc[2U * i1] + pSrc[2U * i3];

            /* xa' = xa + xb + xc + xd */
            pSrc[2U * i0] = r1 + t1;

            /* xa + xc -(xb + xd) */
            r1 = r1 - t1;

            /* yb + yd */
            t2 = pSrc[(2U * i1) + 1U] + pSrc[(2U * i3) + 1U];

            /* ya' = ya + yb + yc + yd */
            pSrc[(2U * i0) + 1U] = s1 + t2;

            /* (ya + yc) - (yb + yd) */
            s1 = s1 - t2;

            /* (yb - yd) */
            t1 = pSrc[(2U * i1) + 1U] - pSrc[(2U * i3) + 1U];

            /* (xb - xd) */
            t2 = pSrc[2U * i1] - pSrc[2U * i3];

            /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
            pSrc[2U * i1] = (r1 * co2) - (s1 * si2);

            /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
            pSrc[(2U * i1) + 1U] = (s1 * co2) + (r1 * si2);

            /* (xa - xc) - (yb - yd) */
            r1 = r2 - t1;

            /* (xa - xc) + (yb - yd) */
            r2 = r2 + t1;

            /* (ya - yc) +  (xb - xd) */
            s1 = s2 + t2;

            /* (ya - yc) -  (xb - xd) */
            s2 = s2 - t2;

            /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
            pSrc[2U * i2] = (r1 * co1) - (s1 * si1);

            /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
            pSrc[(2U * i2) + 1U] = (s1 * co1) + (r1 * si1);

            /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
            pSrc[2U * i3] = (r2 * co3) - (s2 * si3);

            /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
            pSrc[(2U * i3) + 1U] = (s2 * co3) + (r2 * si3);

            i0 += n1;
         } while ( i0 < fftLen);
         j++;
      } while (j <= (n2 - 1U));
      twidCoefModifier <<= 2U;
   }
   /*  Initializations of last stage */
   n1 = n2;
   n2 >>= 2U;

   /*  Calculations of last stage */
   for (i0 = 0U; i0 <= (fftLen - n1); i0 += n1)
   {
      /*  index calculation for the input as, */
      /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
      i1 = i0 + n2;
      i2 = i1 + n2;
      i3 = i2 + n2;

      /*  Butterfly implementation */
      /* xa + xc */
      r1 = pSrc[2U * i0] + pSrc[2U * i2];

      /* xa - xc */
      r2 = pSrc[2U * i0] - pSrc[2U * i2];

      /* ya + yc */
      s1 = pSrc[(2U * i0) + 1U] + pSrc[(2U * i2) + 1U];

      /* ya - yc */
      s2 = pSrc[(2U * i0) + 1U] - pSrc[(2U * i2) + 1U];

      /* xc + xd */
      t1 = pSrc[2U * i1] + pSrc[2U * i3];

      /* xa' = xa + xb + xc + xd */
      pSrc[2U * i0] = (r1 + t1) * onebyfftLen;

      /* (xa + xb) - (xc + xd) */
      r1 = r1 - t1;

      /* yb + yd */
      t2 = pSrc[(2U * i1) + 1U] + pSrc[(2U * i3) + 1U];

      /* ya' = ya + yb + yc + yd */
      pSrc[(2U * i0) + 1U] = (s1 + t2) * onebyfftLen;

      /* (ya + yc) - (yb + yd) */
      s1 = s1 - t2;

      /* (yb-yd) */
      t1 = pSrc[(2U * i1) + 1U] - pSrc[(2U * i3) + 1U];

      /* (xb-xd) */
      t2 = pSrc[2U * i1] - pSrc[2U * i3];

      /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
      pSrc[2U * i1] = r1 * onebyfftLen;

      /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
      pSrc[(2U * i1) + 1U] = s1 * onebyfftLen;

      /* (xa - xc) - (yb-yd) */
      r1 = r2 - t1;

      /* (xa - xc) + (yb-yd) */
      r2 = r2 + t1;

      /* (ya - yc) + (xb-xd) */
      s1 = s2 + t2;

      /* (ya - yc) - (xb-xd) */
      s2 = s2 - t2;

      /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
      pSrc[2U * i2] = r1 * onebyfftLen;

      /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
      pSrc[(2U * i2) + 1U] = s1 * onebyfftLen;

      /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
      pSrc[2U * i3] = r2 * onebyfftLen;

      /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
      pSrc[(2U * i3) + 1U] = s2 * onebyfftLen;
   }

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */
}


/*
 * reverse bits for given bitwidth
 */
static int reverse_bits(int val, int width) {
	int result = 0;
	for (int i = 0; i < width; i++, val >>= 1)
		result = (result << 1) | (val & 1U);
	return result;
}

// set up the FFT
int fft_r4_cmsis_setup(int len)
{
	if((len != 4) && (len != 16) && (len != 64) &&
		(len != 256) && (len != 1024) && (len != 4096))
		return 1;
	
	fftlen = len;
	
	if(!(twiddles = malloc(2*len*sizeof(float))))
		return 1;
	
	if(!(brtbl = malloc(len*sizeof(int))))
	{
		free(twiddles);
		return 1;
	}
	
	// Length variables
	int width = 0;  // Compute levels = floor(log2(n))
	for (int temp = len; temp > 1U; temp >>= 1)
		width++;
	
	for(int i=0;i<len;i++)
	{
		twiddles[2*i] = cosf((float)i * PI * 2.0F / (float)len);
		twiddles[2*i+1] = sinf((float)i * PI * 2.0F / (float)len);
		
		int j = reverse_bits(i, width);
		brtbl[i] = j;
	}
	
	return 0;
}

// free resources (unused for now)
void fft_r4_cmsis_destroy(void)
{
	free(brtbl);
	free(twiddles);
}

// compute the FFT
void fft_r4_cmsis(float *inout, uint8_t ifftFlag, uint8_t bitReverseFlag)
{
	if(!ifftFlag)
	{
		// foward xfrm
		arm_radix4_butterfly_f32(
			inout,
			fftlen,
			twiddles,
			1);
	}
	else
	{
		float onebyfftLen = 1.0F; // scaling done elsewhere /(float)fftlen;
		
		arm_radix4_butterfly_inverse_f32(
				inout,
				fftlen,
				twiddles,
				1,
				onebyfftLen);
	}
	
	if(bitReverseFlag)
	{
		// Bit-reversed addressing permutation
		for (int i = 0; i < fftlen; i++) {
			int j = brtbl[i];
			if (j > i) {
				float temp = inout[2*i];
				inout[2*i] = inout[2*j];
				inout[2*j] = temp;
				temp = inout[2*i+1];
				inout[2*i+1] = inout[2*j+1];
				inout[2*j+1] = temp;
			}
		}
	}
}
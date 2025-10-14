/*
 * dsp_lib.h - miscellaneous DSP stuff for RP2040 Audio
 * 03-27-22 E. Brombaugh
 */

#ifndef __dsp_lib__
#define __dsp_lib__

#include "main.h"

uint8_t dsp_gethyst(int16_t *oldval, int16_t newval);
uint8_t dsp_ratio_hyst_arb(uint16_t *old, uint16_t in, uint8_t range);

/*
 * stuff that CMSIS provides but the RPi Pico SDK does not
 */

#ifndef   __ASM
  #define __ASM                                  __asm
#endif

/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  ARG1  Value to be saturated
  \param [in]  ARG2  Bit position to saturate to (1..32)
  \return             Saturated value
 */
#define __SSAT(ARG1,ARG2) \
__extension__ \
({                          \
  int32_t __RES, __ARG1 = (ARG1); \
  __ASM ("ssat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })


/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  ARG1  Value to be saturated
  \param [in]  ARG2  Bit position to saturate to (0..31)
  \return             Saturated value
 */
#define __USAT(ARG1,ARG2) \
 __extension__ \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  __ASM ("usat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })

/*
 * signed saturation to 16-bit
 */
#if 0
/* as regular code */
inline int16_t dsp_ssat16(int32_t in)
{
	in = in > 32767 ? 32767 : in;
	in = in < -32768 ? -32768 : in;
	return in;
}
#else
/* as intrinsic */
inline int16_t dsp_ssat16(int32_t in)
{
	return __SSAT(in, 16);
}
#endif

#endif


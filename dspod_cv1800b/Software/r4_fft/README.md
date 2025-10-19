# Radix 4 FFT

This is a stand-alone Radix 4 FFT that's been extracted from CMSIS for use in POSIX userspace programs. The original code targeting embedded devices can be found here: [CMSIS DSP Software Library](https://arm-software.github.io/CMSIS_5/DSP/html/index.html)

## Use

There are three functions:

```
int fft_r4_cmsis_setup(int len);
```

Sets up internal data structures for a given length of transform. Supported transform lengths are powers of 4 up to 4096: 4, 16, 64, 256, 1024, 4096. Note that this uses `malloc()` to create the twiddle and bitreverse arrays.

```

void fft_r4_cmsis_destroy(void);

```

Frees up resources allocated by the `setup()` call.

```
void fft_r4_cmsis(float *inout, uint8_t ifftFlag, uint8_t bitReverseFlag);



```

Invokes the transform in-place on the array `inout` which is complex with interleaved Real, Imag data. `ifftFlag` is 0 for the forward transform and 1 for the inverse transform. The inverse transform is not scaled by 1/length. `bitReverseFlag` is 0 when no index addressing correction is required on the output and 1 when correction is needed.





#include "arm_math.h"

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 44100 Hz

fixed point precision: 16 bits

* 0 Hz - 400 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 800 Hz - 22000 Hz
  gain = 0
  desired attenuation = -24 dB
  actual attenuation = n/a

*/

int32_t dsp_LPF_400Hz_filter_kernel[TAP_NUMBER_1_LPF] = // very slow with high tap number... fix it!
{
	-807, -32, -33, -29, -28, -21, -18, -9, -2, 9, 18, 33, 44, 61, 75, 95, 112, 134, 153, 177, 198, 225,
	248, 276, 301, 330, 356, 386, 413, 444, 471, 501, 528, 558, 584, 612,  638, 666, 688, 713, 733, 756,
	773, 793, 808, 824, 835, 848, 855, 864, 867, 873, 871, 873, 867, 864, 855, 848, 835, 824, 808, 793,
	773, 756, 733, 713, 688, 666, 638, 612, 584, 558, 528, 501, 471, 444, 413, 386, 356, 330, 301, 276,
	248, 225, 198, 177, 153, 134, 112, 95, 75, 61, 44, 33, 18, 9, -2, -9, -18, -21, -28, -29, -33, -32,
	-807
};

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 44100 Hz

fixed point precision: 16 bits

* 0 Hz - 800 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 1600 Hz - 22000 Hz
  gain = 0
  desired attenuation = -24 dB
  actual attenuation = n/a

*/

int32_t dsp_LPF_800Hz_filter_kernel[TAP_NUMBER_2_LPF] =
{
	-884, 41, 59, 92, 135, 193, 259, 337, 423, 519, 620, 728, 837, 950, 1061, 1171, 1274, 1373, 1462, 1540,
	1607, 1661, 1699, 1724, 1731, 1724, 1699, 1661, 1607, 1540, 1462, 1373, 1274, 1171, 1061, 950, 837, 728,
	620, 519, 423, 337, 259, 193, 135, 92, 59, 41, -884
};

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 44100 Hz

* 0 Hz - 2000 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 4.172224263175958 dB

* 4000 Hz - 22000 Hz
  gain = 0
  desired attenuation = -24 dB
  actual attenuation = -24.007477218706352 dB
*/

int32_t dsp_LPF_2kHz_filter_kernel[TAP_NUMBER_3_LPF] =
{
	-450, 1178, 1434, 2006, 2665, 3297, 3817, 4159, 4278, 4159, 3817, 3297, 2665, 2006, 1434, 1178, -450
};


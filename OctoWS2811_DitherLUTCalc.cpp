#include <Arduino.h>

#include "OctoWS2811_Dither.h"

byte OctoWS2811_Dither::ditherBits;
uint8_t OctoWS2811_Dither::ditherCycle;
uint8_t OctoWS2811_Dither::gammaTable[256 << DITHER_BITS];

void OctoWS2811_Dither::ditherLUTCalc(double bri) {
  if (bri > 8)
    bri = pow(((bri + 16) / 116), 3);
  else
    bri = bri / 903.3;

  for (int dither = 0; dither < 1 << DITHER_BITS; dither++) {
    int ditherValue = 0;
    int dread = 1 << DITHER_BITS;
    int dout = 1;
    for (int d = 0; d < DITHER_BITS; d++) {
      dread >>= 1;
      if (dither & dread)
        ditherValue |= dout;
      dout <<= 1;
    }
    ditherValue = (ditherValue << (8 - DITHER_BITS));

    for (int n = 0; n < 256; n++) {
      double brightness = n / 2.55;
      double value;
      int pwmValue;
      if (brightness > 8)
        value = pow(((brightness + 16) / 116), 3);
      else
        value = brightness / 903.3;
      value *= 255 * bri;

      pwmValue = value * 256;
      pwmValue += ditherValue;
      pwmValue = pwmValue >> 8;
      gammaTable[n + (dither << 8)] = pwmValue;
    }
  }
}
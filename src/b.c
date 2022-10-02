#include <stdio.h>

signed main() {
  int   i1 = 3.141592654;
  int   i2 = 0.499999;
  int   i3 = 0.500000;
  int   i4 = 0.500001;
  int   i5 = -0.499999;
  int   i6 = -0.500000;
  int   i7 = -0.500001;
  float f1 = 3141592654;
  float f2 = -314;
  int   i8 = f1 / f2;
  float f3 = i1 + i8;
}
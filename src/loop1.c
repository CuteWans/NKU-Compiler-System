#include <stdio.h>
#include <time.h>

#define N 1000000000

int a[N];

struct timespec get_steady_timespec() {
  struct timespec res;
  clock_gettime(CLOCK_MONOTONIC, &res);
  return res;
}

long long timespec_sub(struct timespec tx, struct timespec ty) {
  long long ans;
  ans = (tx.tv_sec - ty.tv_sec) * 1000000000LL + (tx.tv_nsec - ty.tv_nsec);
  return ans;
}

signed main() {
  struct timespec t1 = get_steady_timespec();

  int i;
  for (i = 0; i < N; ++i) {
    a[i] *= 2000;
    a[i] /= 10000;
  }

  struct timespec t2 = get_steady_timespec();
  printf("%lld ns\n", timespec_sub(t2, t1));
}
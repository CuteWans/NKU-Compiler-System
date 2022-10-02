#include <stdio.h>
signed main() {
  int a = 0;
  int b = 1;
  int i = 1;
  int n;
  scanf("%d%d%d", &n, &a, &b);
  while (i < n) {
    int t = b;
    b     = a + b;
    printf("%d\n", b);
    a = t;
    i = i + 1;
  }
}
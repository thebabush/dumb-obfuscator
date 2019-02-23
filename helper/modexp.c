
__attribute__((always_inline)) static unsigned int _slow_mod(unsigned int a, unsigned int b) {
  unsigned int x = b;

  while (x <= (a >> 1)) {
    x <<= 1;
  }

  while (a >= b) {
    if (a >= x) {
      a -= x;
    }
    x >>= 1;
  }

  return a;
}

__attribute__((always_inline)) unsigned int _modexp(unsigned int base, unsigned int exponent, unsigned int mod) {
  unsigned int result = 1;

  base = _slow_mod(base, mod);

  while (exponent > 0) {
    if (exponent & 1) {
      result = _slow_mod(result * base, mod);
    }

    exponent >>= 1;
    base = _slow_mod(base * base, mod);
  }

  return result;
}


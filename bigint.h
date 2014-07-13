#ifndef hBIGINT

#define hBIGINT

#define BASE 100000000 /* 10^8 */

typedef struct bigint {
  unsigned long long *V;
  int tam;
  char sinal;
} bigint;

void inicialize_bigint(bigint *I);

void libere_bigint(bigint *I);

int leia_bigint(bigint *I);

void imprima_bigint(bigint *I);

int compare_bigint(bigint *I1, bigint *I2);

int some_bigint(bigint *I1, bigint *I2, bigint *I3);

int subtraia_bigint(bigint *I1, bigint *I2, bigint *I3);

int multiplique_bigint(bigint *I1, bigint *I2, bigint *I3);

int divida_bigint(bigint *I1, bigint *I2, bigint *I3, bigint *I4);

#endif

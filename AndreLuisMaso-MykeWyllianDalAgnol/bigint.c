/* Prof. Leandro M. Zatesko */

#include"bigint.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/* Rotinas de finalização da função de divisão */

#define ABORTE_DIVISAO(v, w, x, y, z)  \
  { libere_bigint(v); libere_bigint(w); libere_bigint(x); \
    libere_bigint(y); libere_bigint(z); return 0; }

#define FINALIZE_DIVISAO(x, y, z)  \
  { libere_bigint(x); libere_bigint(y); libere_bigint(z); return 1; }

/* Macro que testa se um bigint é zero */

#define ZERO(I) ((I).tam == 1 && (I).V[0] == 0)

/* Cabeçalhos das funções não declaradas em bigint.h acompanhados das
   respectivas especificações. */

int compare_abs_bigint(bigint *I1, bigint *I2);
  /* Compara o valor absoluto do inteiro i1 representado pelo bigint
     apontado por I1 com o valor absoluto do inteiro i2 representado
     pelo bigint apontado por I2. Assume que ambos os bigint's são
     válidos e não-vazios no momento da chamada da função e mantém-nos
     intactos. Devolve 1, 0 ou -1 se, respectivamente, |i1| > |i2|,
     |i1| = |i2| ou |i1| < |i2|. */

int superssome_bigint(bigint *I1, bigint *I2, bigint *I3, int cmp);
  /* Se cmp = 1, faz o mesmo que some_bigint(I1, I2, I3);
     Se cmp = 0, faz o mesmo que subtraia_bigint(I1, I2, I3); */

int some_abs_bigint(bigint *I1, bigint *I2, bigint *I3);
  /* Tenta somar o valor absoluto do inteiro i1 representado pelo bigint
     I1 com o valor absoluto do inteiro i2 representado pelo bigint
     apontado por I2 e garante que o bigint apontado por I3 represente o
     resultado da soma. Assume que os três bigint's são válidos, podendo
     apenas o terceiro ser vazio, no momento da chamada da função e
     garante que todos sejam válidos no momento em que a função se
     encerra. Não assume que os três ponteiros são distintos. Devolve 1
     em caso de sucesso ou 0 caso contrário. */

int subtraia_abs_bigint(bigint *I1, bigint *I2, bigint *I3);
  /* Tenta subtrair o valor absoluto do inteiro i2 representado pelo
     bigint I2 com o valor absoluto do inteiro i1 representado pelo
     bigint apontado por I1 e garante que o bigint apontado por I3
     represente o resultado da subtração. Assume que os três bigint's
     são válidos, podendo apenas o terceiro ser vazio, no momento da
     chamada da função e garante que todos sejam válidos no momento em
     que a função se encerra. Assume adicionalmente que |i1| >= |i2|.
     Não assume que os três ponteiros são distintos. Devolve 1 em caso
     de sucesso ou 0 caso contrário. */

int conserte_zeros(bigint *I);
  /* Tenta remover os eventuais zeros à esquerda do bigint apontado por
     I. Assume que o bigint é válido e não-vazio. Devolve 1 em caso de
     sucesso ou 0 caso contrário. */

int multiplique_bigint_por_escalar(bigint *I1,
                                   unsigned long long a, bigint *I2);
  /* Tenta multiplicar o valor absoluto do inteiro i1 representado pelo
     bigint I1 pelo inteiro não-negativo a e fazer com que o bigint
     apontado por I2 represente o resultado da multiplicação. Assume que
     os bigint's são válidos, sendo o primeiro deles não-vazio, no
     momento da chamada da função e garante que ambos sejam válidos no
     momento em que a função se encerra. Assume que 0 <= a < BASE e que
     os ponteiros são distintos. Devolve 1 em caso de sucesso ou 0 caso
     contrário. */

int desloque_bigint_para_a_esquerda(bigint *I, int i);
  /* Tenta realizar sobre o bigint apontado por I uma operação de
     deslocamento de i casas para a esquerda. Assume que o bigint é
     válido e não-vazio e que o inteiro i é não-negativo. Devolve 1 em
     caso de sucesso ou 0 caso contrário. */

int abs_bigint(bigint *I1, bigint *I2);
  /* Tenta fazer com que o bigint apontado por I2 represente o valor
     absoluto do inteiro representado pelo bigint apontado por I1.
     Assume que ambos os bigint's são válidos, que o primeiro é
     não-vazio e que os ponteiros são distintos. Devolve 1 em caso de
     sucesso ou 0 caso contrário. */

int busque_bin_adic(bigint *I1, bigint *I2, bigint *adic, bigint *mult);
  /* Tenta encontrar, via busca binária, o maior inteiro X tal que
     apenas o primeiro dígito de X na base BASE é não-nulo e tal que
     i2 * X é máximo e não excede i1, sendo i1 o inteiro representado
     por I1 e i2 o inteiro representado por I2. Faz com que adic
     represente X e com que mult represente i2 * X. Assume que todos os
     bigints são válidos, podendo adic e mult ser vazios. Assume ainda
     que i1 >= i2 > 0. Devolve 1 em caso de sucesso ou 0 em caso de
     fracasso. */

/* Implementações das funções */

void inicialize_bigint(bigint *I) {
  I->V = NULL; I->tam = 0; I->sinal = 0;
}

void libere_bigint(bigint *I) {
  if (I->V) free(I->V);
  inicialize_bigint(I);
}

int leia_bigint_da_string(bigint *I, char *S) {
  int i, inic, j, pot;
  libere_bigint(I);
  i = strlen(S) - 1;
  if (S[0] >= '0' && S[0] <= '9') inic = 0;
  else if (S[0] == '+' || S[0] == '-') inic = 1;
  else return 0;
  if (inic > i) return 0;
  if (S[0] == '-') I->sinal = 0;
  else I->sinal = 1;
  for (j = 1; j <= i; j++)
    if (S[j] < '0' || S[j] > '9')
      return 0;
  I->tam = 0;
  while (S[inic] == '0' && inic < i) inic++;
  if (inic == i && S[inic] == '0') I->sinal = 1;
  while (i >= inic) {
    pot = 1;
    if ((I->V = (unsigned long long*)realloc
         (I->V, (I->tam + 1) * sizeof(unsigned long long))) == NULL)
      { libere_bigint(I); return 0; }
    I->V[I->tam] = 0;
    for (j = 1; j <= 8 && i >= inic; j++, i--) {
      I->V[I->tam] = I->V[I->tam] + pot*(S[i] - '0');
      pot *= 10;
    }
    I->tam++;
  }
  return 1;
}

int leia_bigint(bigint *I) {
  char *S = NULL;
  size_t n;
  ssize_t tmp;
  int ret;
  if ((tmp = getline(&S, &n, stdin)) == -1) ret = 0;
  else if (tmp < 1) ret = 0;
  else {
    S[tmp - 1] = '\0';
    ret = leia_bigint_da_string(I, S);
  }
  free(S);
  return ret;
}

void imprima_bigint(bigint *I) {
  int j;
  if (I->tam == 0) return;
  if (I->sinal == 0) printf("-");
  printf("%llu", I->V[I->tam - 1]);
  for (j = I->tam - 2; j >= 0; j--)
    printf("%08llu", I->V[j]);
  printf("\n");
}

int compare_abs_bigint(bigint *I1, bigint *I2) {
  int i = I1->tam - 1;
  if (I1->tam > I2->tam) return 1;
  if (I1->tam < I2->tam) return -1;
  while (i >= 0 && I1->V[i] == I2->V[i]) i--;
  if (i < 0) return 0;
  if (I1->V[i] > I2->V[i]) return 1;
  return -1;
}

int compare_bigint(bigint *I1, bigint *I2) {
  int spin = I1->sinal ? 1 : -1;
  if (I1->sinal != I2->sinal) return I1->sinal - I2->sinal;
  return spin * compare_abs_bigint(I1, I2);
}

int conserte_zeros(bigint *I) {
  int i;
  for (i = I->tam - 1; i >= 0 && I->V[i] == 0; i--);
  if (i == -1) I->tam = 1;
  else I->tam = i + 1;
  if ((I->V = (unsigned long long*)
       realloc(I->V, (I->tam) * sizeof(unsigned long long))) == NULL) {
    libere_bigint(I);
    return 0;
  }
  return 1;
}

int some_abs_bigint(bigint *I1, bigint *I2, bigint *I3) {
  int i;
  unsigned long long *V, tam, carry = 0, valor1, valor2;
  tam = (I1->tam > I2->tam ? I1->tam : I2->tam) + 1;
  if ((V = (unsigned long long*)
       malloc(tam * sizeof(unsigned long long))) == NULL)
    return 0;
  for (i = 0; i < tam; i++) {
    valor1 = i < I1->tam ? I1->V[i] : 0;
    valor2 = i < I2->tam ? I2->V[i] : 0;
    V[i] = valor1 + valor2 + carry;
    carry = V[i] / BASE;
    V[i] = V[i] % BASE;
  }
  libere_bigint(I3);
  I3->V = V;
  I3->tam = tam;
  I3->sinal = 1;
  if (!conserte_zeros(I3)) {
    libere_bigint(I3);
    return 0;
  }
  return 1;
}

int subtraia_abs_bigint(bigint *I1, bigint *I2, bigint *I3) {
  int i;
  unsigned long long *V, tam, carry = 0, valor2;
  tam = I1->tam;
  if ((V = (unsigned long long*)
       malloc(tam * sizeof(unsigned long long))) == NULL)
    return 0;
  for (i = 0; i < tam; i++) {
    valor2 = i < I2->tam ? I2->V[i] : 0;
    if (I1->V[i] < valor2 + carry) {
      V[i] = BASE + I1->V[i] - valor2 - carry;
      carry = 1;
    } else {
      V[i] = I1->V[i] - valor2 - carry;
      carry = 0;
    }
  }
  libere_bigint(I3);
  I3->V = V;
  I3->tam = tam;
  I3->sinal = 1;
  if (!conserte_zeros(I3)) {
    libere_bigint(I3);
    return 0;
  }
  return 1;
}

int superssome_bigint(bigint *I1, bigint *I2, bigint *I3, int cmp) {
  int sinal = I1->sinal;
  if ((sinal == I2->sinal) == cmp) {
    if (!some_abs_bigint(I1, I2, I3)) return 0;
    I3->sinal = sinal;
    return 1;
  }
  switch (compare_abs_bigint(I1, I2)) {
  case 0:
    leia_bigint_da_string(I3, "0");
    break;
  case 1:
    if (!subtraia_abs_bigint(I1, I2, I3)) return 0;
    I3->sinal = sinal;
    break;
  case -1:
    if (!subtraia_abs_bigint(I2, I1, I3)) return 0;
    I3->sinal = sinal ? 0 : 1;
    break;
  }
  return 1;
}

int some_bigint(bigint *I1, bigint *I2, bigint *I3) {
  return superssome_bigint(I1, I2, I3, 1);
}

int subtraia_bigint(bigint *I1, bigint *I2, bigint *I3) {
  return superssome_bigint(I1, I2, I3, 0);
}

int desloque_bigint_para_a_esquerda(bigint *I, int i) {
  int j;
  if ((I->V = (unsigned long long*)realloc
       (I->V, (I->tam + i) * sizeof(unsigned long long))) == NULL) {
    libere_bigint(I);
    return 0;
  }
  for (j = I->tam - 1 + i; j >= i; j--)
    I->V[j] = I->V[j-i];
  for (; j >= 0; j--) I->V[j] = 0;
  I->tam += i;
  return conserte_zeros(I);
}

int multiplique_bigint_por_escalar(bigint *I1,
                                   unsigned long long a, bigint *I2) {
  int i;
  unsigned long long carry = 0;
  libere_bigint(I2);
  I2->sinal = 1;
  I2->tam = I1->tam + 1;
  if ((I2->V = (unsigned long long*)
       malloc((I2->tam) * sizeof(unsigned long long))) == NULL) {
    libere_bigint(I2);
    return 0;
  }
    for (i = 0; i < I1->tam; i++) {
    I2->V[i] = I1->V[i] * a + carry;
    carry = I2->V[i] / BASE;
    I2->V[i] = I2->V[i] % BASE;
  }
  I2->V[i] = carry;
  I2->tam = i + 1;
  return conserte_zeros(I2);
}

int multiplique_bigint(bigint *I1, bigint *I2, bigint *I3) {
  bigint tmp, soma;
  int i, sinal = (I1->sinal == I2->sinal);
  inicialize_bigint(&tmp);
  inicialize_bigint(&soma);
  if (!leia_bigint_da_string(&soma, "0")) return 0;
  for (i = 0; i < I1->tam; i++) {
    if (!multiplique_bigint_por_escalar(I2, I1->V[i], &tmp) ||
        !desloque_bigint_para_a_esquerda(&tmp, i) ||
        !some_bigint(&soma, &tmp, &soma)) {
      libere_bigint(&soma);
      libere_bigint(&tmp);
      return 0;
    }
  }
  libere_bigint(&tmp);
  libere_bigint(I3);
  I3->V = soma.V;
  I3->tam = soma.tam;
  I3->sinal = ZERO(*I3) ? 1 : sinal;
  return 1;
}

int abs_bigint(bigint *I1, bigint *I2) {
  int i;
  libere_bigint(I2);
  if ((I2->V = (unsigned long long*)malloc
       ((I1->tam) * sizeof(unsigned long long))) == NULL) return 0;
  I2->sinal = 1; I2->tam = I1->tam;
  for (i = 0; i < I2->tam; i++) I2->V[i] = I1->V[i];
  return 1;
}

int busque_bin_adic(bigint *I1, bigint *I2, bigint *adic, bigint *mult) {
  unsigned long long dmin = 1, dmax = BASE, d;
  int shift, cmp;
  libere_bigint(adic); libere_bigint(mult);
  while (dmin + 1 < dmax) {
    d = dmin + (dmax - dmin) / 2;
    if (!multiplique_bigint_por_escalar(I2, d, mult)) return 0;
    shift = I1->tam > mult->tam ? I1->tam - mult->tam : 0;
    if (!desloque_bigint_para_a_esquerda(mult, shift)) return 0;
    if ((cmp = compare_bigint(mult, I1)) > 0) dmax = d;
    else if (cmp == 0) break;
    else dmin = d;
  }
  d = dmin + 1;
  if (!multiplique_bigint_por_escalar(I2, d, mult)) return 0;
  shift = I1->tam > mult->tam ? I1->tam - mult->tam : 0;
  if (!desloque_bigint_para_a_esquerda(mult, shift)) return 0;
  if ((cmp = compare_bigint(mult, I1)) > 0) {
    d = dmin;
    if (!multiplique_bigint_por_escalar(I2, d, mult)) return 0;
    shift = I1->tam > mult->tam ? I1->tam - mult->tam : 0;
    if (!desloque_bigint_para_a_esquerda(mult, shift)) return 0;
  }
  if (!leia_bigint_da_string(adic, "0")) return 0;
  adic->V[0] = d;
  if (!desloque_bigint_para_a_esquerda(adic, shift)) return 0;
  return 1;
}

int divida_bigint(bigint *I1, bigint *I2, bigint *I3, bigint *I4) {
  bigint I2abs, quoc, mod, adic, mult;
  inicialize_bigint(&I2abs); inicialize_bigint(&quoc);
  inicialize_bigint(&mod); inicialize_bigint(&adic);
  inicialize_bigint(&mult);
  if (ZERO(*I2)) ABORTE_DIVISAO(&I2abs, &quoc, &mod, &adic, &mult);
  if (!abs_bigint(I1, &mod) || !abs_bigint(I2, &I2abs) ||
      !leia_bigint_da_string(&quoc, "0"))
    ABORTE_DIVISAO(&I2abs, &quoc, &mod, &adic, &mult);
  while (compare_bigint(&I2abs, &mod) <= 0) {
    if (!busque_bin_adic(&mod, &I2abs, &adic, &mult))
      ABORTE_DIVISAO(&I2abs, &quoc, &mod, &adic, &mult);
    if (!subtraia_bigint(&mod, &mult, &mod) ||
        !some_bigint(&quoc, &adic, &quoc))
      ABORTE_DIVISAO(&I2abs, &quoc, &mod, &adic, &mult);
  }
  if (I1->sinal && !(I2->sinal))
    quoc.sinal = 0;
  else if (!(I1->sinal)) {
    if (!leia_bigint_da_string(&adic, "1") ||
        !some_bigint(&quoc, &adic, &quoc) ||
        !subtraia_bigint(&I2abs, &mod, &mod))
      ABORTE_DIVISAO(&I2abs, &quoc, &mod, &adic, &mult);
    if (I2->sinal) quoc.sinal = 0;
  }
  libere_bigint(I3);
  I3->sinal = quoc.sinal; I3->tam = quoc.tam; I3->V = quoc.V;
  libere_bigint(I4);
  I4->sinal = mod.sinal; I4->tam = mod.tam; I4->V = mod.V;
  FINALIZE_DIVISAO(&I2abs, &adic, &mult);
}

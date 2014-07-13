/*-------------------------------------------------------------------------------------------
-----------------TRABALHO DE ESTRUTURA DE DADOS I--------------------------------------------
---------------------------------------------------------------------------------------------
-----------------ACADEMICOS: ANDRE LUIZ MASO & MYKE WYLLIAN DAL'AGNOL------------------------
---------------------------------------------------------------------------------------------
-----------------CALCULADORA USANDO O TIPO ABSTRATO DE DADO: BIGINT--------------------------
-------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigint.h"
#include "llstack.h"

/*-------------------------------------------------------------------------------------------
-------funcao que libera o bigint, usada como parametro na funcao de free_llstack -----------
-------* recebe de parametro um ponteiro void para o bigint e entao libera numero------------
-------------------------------------------------------------------------------------------*/

void liberebigint(void *x){
	libere_bigint((bigint*)x);
	free((bigint*)x);
}

/*-------------------------------------------------------------------------------------------
-------funcao que verifica se o bigint eh 0--------------------------------------------------
-------* recebe como parametro um ponteiro para um bigint, soma com ele mesmo e -------------
-------  verifica se o resultado eh igual ao bigint passado, se for o numero eh 0------------
-------------------------------------------------------------------------------------------*/

int zero(bigint *k){
	bigint s;
	inicialize_bigint(&s);
	int ret=0;
	//char *d;
	//d =(char*)malloc(3*sizeof(char));
	//d="0\n\0";
	//leia_bigint_da_string(&s,d);
	//free(d);
	if(some_bigint(k,k,&s)==0) ret=1;
	if(compare_bigint(k,&s)==0) ret=1;
	libere_bigint(&s);
	return ret;
}

/*-------------------------------------------------------------------------------------------
------funcao que imprime a linha de erro de comando invalido---------------------------------
-------------------------------------------------------------------------------------------*/

void erro(int a){
	fprintf(stderr,"ERROR! %d-th command is invalid.\n",a);
}

/*-------------------------------------------------------------------------------------------
-------funcao que imprime a linha de erro de locacao de memoria pelo malloc------------------
-------* recebe de parametro a pilha de bigint e a libera------------------------------------
-------------------------------------------------------------------------------------------*/

void erro_malloc(llstack_t *L){
	free_llstack(L,&liberebigint);
	free(L);
	fprintf(stderr,"ERROR while calling malloc(3)\n");
}

/*-------------------------------------------------------------------------------------------
-------funcao que imprime a linha de erro de locacao de memoria pelo malloc------------------
-------* recebe de parametro a pilha de bigint e dois bigint que ja foram--------------------
-------  desempilhados e os libera-----------------------------------------------------------
-------------------------------------------------------------------------------------------*/

void erro_(llstack_t *L, bigint *I, bigint *I2){
	push_llstack(L,I2);
	push_llstack(L,I);
}

/*-------------------------------------------------------------------------------------------
-------funcao que verifica se existe algum caracter que nao seja um numero-------------------
-------* recebe de parametro um ponteiro para a string a ser verificada----------------------
-------------------------------------------------------------------------------------------*/

int verifique_string(char *S){
	int ret=1, i, y = strlen(S);
	for(i=(S[0]=='+'||S[0]=='-')?1:0; i < y; i++){
		if(S[i]>'9'||S[i]<'0') {ret = 0; break;}
	}
	return ret;
}

/*-------------------------------------------------------------------------------------------
-------funcao que converte o bigint da string e o empilha------------------------------------
-------* recebe de parametro a pilha, a string com o bigint e o numero da linha de comando---
-------------------------------------------------------------------------------------------*/

int empilhe(llstack_t *L,char *S, int a){
	if(verifique_string(S)==0) {erro(a);free(S); return 1;}
	bigint *I=NULL;
	if((I=(bigint*) malloc (sizeof(bigint)))==NULL) {erro_malloc(L); return 0;}
		inicialize_bigint(I);
		leia_bigint_da_string(I,S);
		push_llstack(L,I);
		free(S);
		return 1;
}

/*-------------------------------------------------------------------------------------------
-------funcao que resolve as operacoes matematicas da calculadora ---------------------------
-------* recebe de parametro a pilha de bigints, a linha de comando e o codigo da operacao---
-------------------------------------------------------------------------------------------*/

int resolva(llstack_t *L, int caso){
	bigint *I=NULL,*I2=NULL;
	if(get_size(L)<2) {return 0;}
	else{
		I=(bigint*)pop_llstack(L);
		I2=(bigint*)pop_llstack(L);
		if(caso==5||caso==6) {if(zero(I2)==1) {erro_(L,I,I2); return 0;}}
		switch (caso){
			case 2: if(some_bigint(I,I2,I)==0) {erro_(L,I,I2); return 0;} break;
			case 3: if(subtraia_bigint(I,I2,I)==0) {erro_(L,I,I2); return 0;} break;
			case 4: if(multiplique_bigint(I,I2,I)==0) {erro_(L,I,I2); return 0;} break;
			case 5: if(divida_bigint(I,I2,I,I2)==0) {erro_(L,I,I2); return 0;} break;
			case 6: if(divida_bigint(I,I2,I2,I)==0) {erro_(L,I,I2); return 0;} break;
		}
		libere_bigint(I2);
		free(I2);
		I2=NULL;
		push_llstack(L,I);
		return 1;
	}
}

/*-------------------------------------------------------------------------------------------
-------funcao que faz as impressoes dos bigint na tela --------------------------------------
-------* recebe de parametro a pilha de bigint, a linha de comando e o codigo da operacao----
-------------------------------------------------------------------------------------------*/

int imprima(llstack_t *L, int caso){
	bigint *I=NULL;
	if(test_emptiness(L)) return 0;
	else{
		I=(bigint*)pop_llstack(L);
		imprima_bigint(I);
		switch (caso){
			case 7: {push_llstack(L,I); break;}
			case 8: {libere_bigint(I); free(I); break;}
		}
	}
	return 1;
}

/*-------------------------------------------------------------------------------------------
-------funcao main da calculadora------------------------------------------------------------
-------* contem necessariamente uma pilha e uma string---------------------------------------
-------------------------------------------------------------------------------------------*/

int main(void){
	llstack_t *L=NULL;      
	char *S=NULL;      
	size_t n=0;        
	ssize_t tam;       
	if((L=(llstack_t*) malloc (sizeof(llstack_t)))==NULL){ erro_malloc(L); return 2;}
	create_llstack(L);
	int a=0,caso,tm;
	
/*-------o laco se repete ate dar erro no getline ou encontrar o EOF------------------------*/

	while(tam = getline(&(S), &n, stdin) != -1){
		tm=strlen(S);
		S[tm - 1] = '\0';
		a++;
		n=0;
		
/*-------as operacoes a serem feitas sao codificadas para facilitar o ciclo do programa----*/

		if((S[0]>='0'&&S[0]<='9')||((S[0]=='+'||S[0]=='-')&&(S[1]>='0'&&S[1]<='9'))) caso = 1;
		else if(S[0]=='+'&&tm==2) caso = 2;
		else if(S[0]=='-'&&tm==2) caso = 3;
		else if(S[0]=='*'&&tm==2) caso = 4;
		else if(S[0]=='/'&&tm==2) caso = 5;
		else if(S[0]=='%'&&tm==2) caso = 6;
		else if(S[0]=='p'&&tm==2) caso = 7;
		else if(S[0]=='P'&&tm==2) caso = 8;
		else if(S[0]=='q'&&tm==2) caso = 9;
		else caso = 10;
		if(caso != 1) free(S);
		
/*--------dependendo da origem da operacao, uma funcao mais especifica eh chamada-----------*/
		
		if(caso==1)	{if(empilhe(L,S,a)==0){ return 2;}}
		else if(caso>1&&caso<7)	{if(resolva(L,caso)==0){erro(a);}}
		else if(caso==7||caso==8) {if(imprima(L,caso)==0) {erro(a);}}
		else if(caso==9) {free_llstack(L,&liberebigint); free(L);return 0;}
		else erro(a);
	}
	
/*-------quando sair do while testa se deu fim do arqivo ou se deu um erro inesperado--------*/
	
	free_llstack(L,&liberebigint);
	free(L);
	free(S);
	char q;
	if(scanf("%c",&q)==EOF){
		fprintf(stderr,"ERROR while reading from stdin.\n"); return 1;
	}
	fprintf(stderr,"ERROR! Aborted for an unknown reason.\n"); return 3;
}

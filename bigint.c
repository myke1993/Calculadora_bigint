#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

void inicialize_bigint(bigint *I){
	I->V=NULL;
	I->tam=0;
	I->sinal=0;
}

void libere_bigint(bigint *I){
	free(I->V);
	inicialize_bigint(I);
}

int leia_bigint(bigint *I){
	if(I->V!=NULL){
		libere_bigint(I);
	}
	size_t n=0;
	ssize_t tam;
	char *tem=NULL;
	int inicio,i,j,k,x;
	if ((tam = getline(&(tem), &n, stdin)) == -1){
		return 0;
	}
	tem[--tam] = '\0';
	if (tem[0]=='-'){
		I->sinal=1;
	}
	if(tem[0]=='+'||tem[0]=='-'){
		for(inicio=1;tem[inicio]=='0'&&inicio<=tam;inicio++);
	}
	else{
		for(inicio=0;tem[inicio]=='0'&&inicio<=tam;inicio++);
	}
	if(inicio==tam){
		I->tam=1;
		I->sinal=0;
			if((I->V = (unsigned long long *) malloc(I->tam*sizeof(unsigned long long)))==NULL){
			free(tem);
			inicialize_bigint(I);
			return 0;
		}
		I->V[0]=0; 
		return 1;
	}
	I->tam=((tam-inicio)/8);
	if((tam-inicio)%8!=0){
		I->tam++;
	}
	if((I->V = (unsigned long long *) malloc(I->tam*sizeof(unsigned long long)))==NULL){
		free(tem);
		inicialize_bigint(I);
		return 0;
	} 
	for(i=0;i<I->tam;i++){
		I->V[i]=0;
	}
	for(i=0;i<I->tam;i++){
		for(j=0;j<=7&&tam!=inicio;j++){
			x=1;
			for(k=1;k<=j;k++){
				x*=10;
			}
			I->V[i]+=((tem[tam-1]-'0')*x);
			tam--;
		}
	}	
	free(tem);
		return 1;
}

void imprima_bigint(bigint *I){
	int a;
	if(I->sinal==1){
		printf("-");
	}
	printf("%llu",I->V[I->tam-1]);
	if(I->tam-2>-1){
		for(a=I->tam-2;a>=0;a--){
			printf("%08llu",I->V[a]);
		}
	}
	printf("\n");
}

int compare_bigint(bigint *I1, bigint *I2){
	int a;
	if(I1->sinal>I2->sinal){
	return -1;
	}
	if(I1->sinal<I2->sinal){
	return 1;
	}
	if(I1->tam>I2->tam){
		if(I1->sinal==0){
			return 1;
		}
		return -1;
	}
	if(I1->tam<I2->tam){
		if(I1->sinal==0){
			return -1;
		}
		return 1;
	}
	if(I1->sinal==0){
		for(a=I1->tam-1;a>=0;a--){
			if(I1->V[a]>I2->V[a]){
				return 1;				}
			else if(I1->V[a]<I2->V[a]){
				return -1;
			}
		}
	}
	for(a=a<I1->tam-1;a>=0;a++){
		if(I1->V[a]<I2->V[a]){
			return 1;				}
		else if(I1->V[a]>I2->V[a]){
			return -1;
		}
		return 0;
	}
}

int some_bigint(bigint *I1, bigint *I2, bigint *I3){
	int c;
	if(I1->sinal==0&&I2->sinal==1){
		I2->sinal=0;
		if(subtraia_bigint(I1,I2,I3)==0){
		return 0;
		}
		I2->sinal=1;
		return 1;
	}
	else if(I1->sinal==1&&I2->sinal==0){
		I2->sinal=1;
		if(subtraia_bigint(I1,I2,I3)==0){
		return 0;
		}
		I2->sinal=0;
		return 1;
	}
	else if(I1->sinal==0&&I2->sinal==0){
		c=0;
	}
	else{
		c=1;
	}
	int a,b;
	bigint *tmp;
	if((tmp=(bigint*) malloc (sizeof(bigint)))==NULL){
		return 0;
	}
	inicialize_bigint(tmp);
	tmp->sinal=c;
	if(compare_bigint(I1,I2)==1){
		tmp->tam=I1->tam;
		if((tmp->V = (unsigned long long *) malloc(tmp->tam*sizeof(unsigned long long)))==NULL){
			libere_bigint(tmp);
			free(tmp);
			return 0;
		}
	}
	else{
		tmp->tam=I2->tam;
		if((tmp->V = (unsigned long long *) malloc(tmp->tam*sizeof(unsigned long long)))==NULL){
			libere_bigint(tmp);
			free(tmp);
			return 0;
		}
	}
	for(a=0;a<tmp->tam;a++){
		tmp->V[a]=0;
	}
	for(a=0;a<tmp->tam;a++){
		if(a<I1->tam){
			tmp->V[a]+=I1->V[a];
		}
		if(a<I2->tam){
			tmp->V[a]+=I2->V[a];
		}
		if(a+1==tmp->tam){
			if(tmp->V[a]/BASE>=1){
				tmp->tam++;
				if((tmp->V = (unsigned long long *) realloc(tmp->V,tmp->tam*sizeof(unsigned long long)))==NULL){
					libere_bigint(tmp);
					free(tmp);
					return 0;
				}
			}
		}
		if(tmp->V[a]>=BASE){
			tmp->V[a+1]=tmp->V[a]/BASE;
			tmp->V[a]%=BASE;
		}
	}
	if(I3->V!=NULL){
		libere_bigint(I3);
	}
	*I3=*tmp;
	return 1;
}

int subtraia_bigint(bigint *I1, bigint *I2, bigint *I3){	
	int c,x=0;;	
	if(I1->sinal==0&&I2->sinal==1){
		I2->sinal=0;
		if(some_bigint(I1,I2,I3)==0){
			return 0;
		}
		I2->sinal=1;
		return 1;
	}
	else if(I1->sinal==1&&I2->sinal==0){
		I2->sinal=1;
		if(some_bigint(I1,I2,I3)==0){
			return 0;
		}
		I2->sinal=0;
		return 1;
	}
	else if(I1->sinal==0&&I2->sinal==0){
		c=0;
	}
	else{
		c=1;
	}
	int a,b;
	bigint *tmp;
	if((tmp=(bigint*) malloc (sizeof(bigint)))==NULL){
		return 0;
	}
	inicialize_bigint(tmp);
	if(c==1){
		if((compare_bigint(I1,I2))==-1){
			tmp->sinal=1;
			tmp->tam=I1->tam;
		}
		else{
			tmp->sinal=0;
			tmp->tam=I2->tam;
		}
	}
	else{
		if((compare_bigint(I1,I2))==-1){
			tmp->sinal=1;
			tmp->tam=I2->tam;
		}
		else{
			tmp->sinal=0;
			tmp->tam=I1->tam;
		}
	}
		if((tmp->V = (unsigned long long *) malloc(tmp->tam*sizeof(unsigned long long)))==NULL){
			libere_bigint(tmp);
			free(tmp);
			return 0;
		}
		for(a=0;a<tmp->tam;a++){
			tmp->V[a]=0;
		}
	if(tmp->sinal==0&&c==0){
		for(a=0;a<tmp->tam;a++){
			tmp->V[a]+=I1->V[a];
			if(I2->tam>a){
				tmp->V[a]-=I2->V[a];
			}
			if(tmp->V[a]>BASE){
				tmp->V[a]+=BASE;
				tmp->V[a+1]--;
			}
		}
	}
	else if (tmp->sinal==1&&c==0){
		for(a=0;a<tmp->tam;a++){
			tmp->V[a]+=I2->V[a];
			if(I1->tam>a){
				tmp->V[a]-=I1->V[a];
			}
			if(tmp->V[a]>BASE){
				tmp->V[a]+=BASE;
				tmp->V[a+1]--;
			}
			
		}
	}
	else if (tmp->sinal==0&&c==1){
		for(a=0;a<tmp->tam;a++){
			tmp->V[a]+=I2->V[a];
			if(I1->tam>a){
				tmp->V[a]-=I1->V[a];
			}
			if(tmp->V[a]>BASE){
				tmp->V[a]+=BASE;
				tmp->V[a+1]--;
			}
			
		}
	}
	else {
		for(a=0;a<tmp->tam;a++){
			tmp->V[a]+=I1->V[a];
			if(I2->tam>a){
				tmp->V[a]-=I2->V[a];
			}
			if(tmp->V[a]>BASE){
				tmp->V[a]+=BASE;
				tmp->V[a+1]--;
			}
			
		}
	}
	for(a=tmp->tam-1;a>0;a--){
		if(tmp->V[a]==0){
			tmp->tam--;
			if((tmp->V = (unsigned long long *) realloc(tmp->V,tmp->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tmp);
				free(tmp);
				return 0;
			}
		}
		else{
			break;
		}
	}
	if(I3->V!=NULL){
		libere_bigint(I3);
	}
	*I3 = *tmp;
	return 1;
}

int multiplique_bigint(bigint *I1, bigint *I2, bigint *I3){
	bigint *tem;
	bigint *Ttem;
	if((tem = (bigint*) malloc (sizeof(bigint)))==NULL){
		return 0;
	}
	if((Ttem = (bigint*) malloc (sizeof(bigint)))==NULL){
		return 0;
	}
	inicialize_bigint(tem);
	inicialize_bigint(Ttem);
	tem->tam=I1->tam+I2->tam+1;
	Ttem->tam=I1->tam+I2->tam+1;
	if((tem->V = (unsigned long long *) malloc(tem->tam*sizeof(unsigned long long)))==NULL){
		libere_bigint(tem);
		free(tem);
		libere_bigint(Ttem);
		free(Ttem);
		return 0;
	}
	if((Ttem->V = (unsigned long long *) malloc(Ttem->tam*sizeof(unsigned long long)))==NULL){
		libere_bigint(tem);
		free(tem);
		libere_bigint(Ttem);
		free(Ttem);
		return 0;
	}
	int a,b,c;
	for(b=0;b<Ttem->tam;b++){
		Ttem->V[b]=0;
	}
	for(a=0;a<I2->tam;a++){
		for(b=0;b<tem->tam;b++){
			tem->V[b]=0;
		}
		for(b=a,c=0;c<I1->tam;b++,c++){
			tem->V[b]+=(I1->V[c]*I2->V[a]);
			if(tem->V[b]>=BASE){
				tem->V[b+1]=tem->V[b]/BASE;
				tem->V[b]%=BASE;
			}
		}
		some_bigint(Ttem,tem,Ttem);
		Ttem->tam=I1->tam+I2->tam+1;
		if((Ttem->V = (unsigned long long *) realloc(Ttem->V,Ttem->tam*sizeof(unsigned long long)))==NULL){
			libere_bigint(tem);
			free(tem);
			libere_bigint(Ttem);
			free(Ttem);
			return 0;
		}
	}
		for(a=Ttem->tam-1;a>0;a--){
			if(Ttem->V[a]==0){
				Ttem->tam-=1;
				if((Ttem->V = (unsigned long long *) realloc(Ttem->V,Ttem->tam*sizeof(unsigned long long)))==NULL){
					libere_bigint(tem);
					free(tem);
					libere_bigint(Ttem);
					free(Ttem);
					return 0;
				}
			}
		else{
			break;
		}
	}
	if(I1->sinal!=I2->sinal){
		Ttem->sinal=1;
	}
	else{
		Ttem->sinal=0;
	}
	if(I3->V!=NULL){
		libere_bigint(I3);
	}
	*I3=*Ttem;
	libere_bigint(tem);
	free(tem);
	return 1;
}


int divida_bigint(bigint *I1, bigint *I2, bigint *I3, bigint *I4){
	if(I2->V[0]==0&&I2->tam==1){
		return 0;
	}
	bigint *tem;
	bigint *Ttem;
	if((tem = (bigint*) malloc (sizeof(bigint)))==NULL){
		return 0;
	}
	if((Ttem = (bigint*) malloc (sizeof(bigint)))==NULL){
		free(tem);
		return 0;
	}
	inicialize_bigint(tem);
	inicialize_bigint(Ttem);
	if((compare_bigint(I1,I2))==0){
		tem->sinal=0;
		tem->tam=1;
		if((tem->V = (unsigned long long *) malloc(tem->tam*sizeof(unsigned long long)))==NULL){
			libere_bigint(tem);
			free(tem);
			libere_bigint(Ttem);
			free(Ttem);
			return 0;
		}
		tem->V[0]=1;
		Ttem->sinal=0;
		Ttem->tam=1;
		if((Ttem->V = (unsigned long long *) malloc(Ttem->tam*sizeof(unsigned long long)))==NULL){
			libere_bigint(tem);
			free(tem);
			libere_bigint(Ttem);
			free(Ttem);
			return 0;
		}
		Ttem->V[0]=0;
		if(I3->V!=NULL){
			libere_bigint(I3);
		}
		if(I4->V!=NULL){
			libere_bigint(I4);
		}
		*I3=*tem;
		*I4=*Ttem;
		return 1;
	}
	int a;
	if(I1->sinal==0&&I2->sinal==0){
		if((compare_bigint(I1,I2))==-1){
			tem->tam=I1->tam;
			tem->sinal=I1->sinal;
			if((tem->V = (unsigned long long *) malloc(tem->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tem);
				free(tem);
				libere_bigint(Ttem);
				free(Ttem);
				return 0;
			}
			for(a=0;a<tem->tam;a++){
				tem->V[a]=I1->V[a];
			}
			Ttem->sinal=0;
			Ttem->tam=1;
			if((Ttem->V = (unsigned long long *) malloc(Ttem->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tem);
				free(tem);
				libere_bigint(Ttem);
				free(Ttem);
				return 0;
			}
			Ttem->V[0]=0;
			if(I3->V!=NULL){
				libere_bigint(I3);
			}
			if(I4->V!=NULL){
				libere_bigint(I4);
			}
			*I4=*tem;
			*I3=*Ttem;
			return 1;
		}
	}
	if(I1->sinal==1&&I2->sinal==1){
		if((compare_bigint(I1,I2))==1){
			tem->tam=I1->tam;
			tem->sinal=I1->sinal;
			if((tem->V = (unsigned long long *) malloc(tem->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tem);
				free(tem);
				libere_bigint(Ttem);
				free(Ttem);
				return 0;
			}
			for(a=0;a<tem->tam;a++){
				tem->V[a]=I1->V[a];
			}
			I2->sinal=0;
			some_bigint(tem,I2,tem);
			I2->sinal=1;
			Ttem->sinal=0;
			Ttem->tam=1;
			if((Ttem->V = (unsigned long long *) malloc(Ttem->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tem);
				free(tem);
				libere_bigint(Ttem);
				free(Ttem);
				return 0;
			}
			Ttem->V[0]=1;
			if(I3->V!=NULL){
				libere_bigint(I3);
			}
			if(I4->V!=NULL){
				libere_bigint(I4);
			}
			*I4=*tem;
			*I3=*Ttem;
			return 1;
		}
	}
	int i1s,i2s;
	i1s=I1->sinal;
	i2s=I2->sinal;
	I1->sinal=0;
	I2->sinal=0;
	bigint *dois;
	bigint *tot;
	if((dois = (bigint*) malloc (sizeof(bigint)))==NULL){
		free(tem);
		free(Ttem);
		return 0;
	}
	if((tot = (bigint*) malloc (sizeof(bigint)))==NULL){
		free(tem);
		free(Ttem);
		free(dois);
		return 0;
	}
	inicialize_bigint(dois);
	inicialize_bigint(tot);
	dois->tam=1;
	dois->sinal=0;
	if((dois->V = (unsigned long long *) malloc(dois->tam*sizeof(unsigned long long)))==NULL){
		libere_bigint(tem);
		free(tem);
		libere_bigint(Ttem);
		free(Ttem);
		libere_bigint(dois);
		free(dois);
		libere_bigint(tot);
		free(tot);
		return 0;
	}
	dois->V[0]=2;
	tem->sinal=0;
	tem->tam=1;
	if((tem->V = (unsigned long long *) malloc(tem->tam*sizeof(unsigned long long)))==NULL){
		libere_bigint(tem);
		free(tem);
		libere_bigint(Ttem);
		free(Ttem);
		libere_bigint(dois);
		free(dois);
		libere_bigint(tot);
		free(tot);
		return 0;
	}
	tem->V[0]=1;
	Ttem->sinal=0;
	Ttem->tam=1;
	if((Ttem->V = (unsigned long long *) malloc(Ttem->tam*sizeof(unsigned long long)))==NULL){
		libere_bigint(tem);
		free(tem);
		libere_bigint(Ttem);
		free(Ttem);
		libere_bigint(dois);
		free(dois);
		libere_bigint(tot);
		free(tot);
		return 0;
	}
	Ttem->V[0]=0;
	int r=0;
	while(r!=1){
		some_bigint(Ttem,tem,Ttem);
		multiplique_bigint(Ttem,I2,tot);
		if(compare_bigint(tot,I1)==0){	
			if(i1s==i2s){
				Ttem->sinal=0;
			}
			else{
				Ttem->sinal=1;
			}
			tem->sinal=0;
			tem->tam=1;
			if((tem->V = (unsigned long long *) realloc(tem->V,tem->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tem);
				free(tem);
				libere_bigint(Ttem);
				free(Ttem);
				libere_bigint(dois);
				free(dois);
				libere_bigint(tot);
				free(tot);
				return 0;
			}
			tem->V[0]=0;
			if(I3->V!=NULL){
				libere_bigint(I3);
			}
			if(I4->V!=NULL){
				libere_bigint(I4);
			}
			I1->sinal=i1s;
			I2->sinal=i2s;
			*I3=*Ttem;
			*I4=*tem;
			libere_bigint(dois);
			free(dois);
			libere_bigint(tot);
			free(tot);
			return 1;
		}
		else if(compare_bigint(tot,I1)==-1){
			multiplique_bigint(tem,dois,tem);
		}
		else{
			subtraia_bigint(Ttem,tem,Ttem);
			tem->sinal=0;
			tem->tam=1;
			if((tem->V = (unsigned long long *) realloc(tem->V,tem->tam*sizeof(unsigned long long)))==NULL){
				libere_bigint(tem);
				free(tem);
				libere_bigint(Ttem);
				free(Ttem);
				libere_bigint(dois);
				free(dois);
				libere_bigint(tot);
				free(tot);
				return 0;
			}
			tem->V[0]=1;
			some_bigint(Ttem,tem,tot);
			multiplique_bigint(tot,I2,tot);
			if(compare_bigint(tot,I1)==1){
				multiplique_bigint(Ttem,I2,tot);
				subtraia_bigint(I1,tot,tot);
				if(i1s==1){
					some_bigint(Ttem,tem,Ttem);
					subtraia_bigint(I2,tot,tot);
				}
				if(I3->V!=NULL){
					libere_bigint(I3);
				}
				if(I4->V!=NULL){
					libere_bigint(I4);
				}
				if(i1s==i2s){
					Ttem->sinal=0;
				}
				else{
					Ttem->sinal=1;
				}
				I1->sinal=i1s;
				I2->sinal=i2s;
				*I3=*Ttem;
				*I4=*tot;
				return 1;
			}
		}
		
	}
	
	return 1;
}

















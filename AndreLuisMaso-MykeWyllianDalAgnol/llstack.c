#include<stdio.h>
#include<stdlib.h>
#include "llstack.h"

void free_llstack(llstack_t *S, void (*free_function)(void*)){
	if(S->top == NULL){
		create_llstack(S);
		return;
	}
	void *X;
	X=pop_llstack(S);
	(*free_function)(X);
	free_llstack(S,free_function);
}

int push_llstack(llstack_t *S, void *k){
	llsnode_t *X; 
	X = (llsnode_t*)malloc(sizeof(llsnode_t));
	if (X == NULL) return 0;
	X->next = S->top;
	X->key = k;
	S->top = X;
	S->nmemb++;
	return 1;
}
  
  
void* pop_llstack(llstack_t *S){
	void *tmp;
	llsnode_t *X;
	X = S->top;
	tmp = S->top->key;
	S->top = S->top->next;
	free(X);
	S->nmemb--;
	return tmp;
}

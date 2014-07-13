#ifndef _LLSTACK_H
#define _LLSTACK_H
#include<stdlib.h>

/* Node of a linked list stack */
typedef struct llsnode {
  void *key; /* pointer to the key of the node */
  struct llsnode *next; /* pointer to the next node in the stack */
} llsnode_t;

/* Linked list stack */
typedef struct lls {
  llsnode_t *top; /* pointer to the node in the top of the stack */
  size_t nmemb; /* number of nodes in the stack */
} llstack_t;

/* Create an empty linked list stack.
 * S must be a stack, allocated but never used. */
#define create_llstack(S)  S->nmemb=0;S->top=NULL 

/* Return the number of nodes in the stack.
 * S must be a valid linked list stack. */
#define get_size(S) S->nmemb

/* Return 1 if the stack S is empty or 0 otherwise.
 * S must be a valid linked list stack. */
#define test_emptiness(S) (S->nmemb==0 ? 1:0)

/* Free all the memory used by a valid linked list stack,
   setting top of the stack to NULL.
 * S must be a pointer to the stack.
 * free_function must be a pointer to a function which frees
   the key according to its data type. */
void free_llstack(llstack_t *S, void (*free_function)(void*));

/* Push a key into a linked list stack.
 * S must be a pointer to the stack.
 * k must be a pointer to the key to be pushed.
 * Returns 1 on success or 0 otherwise. */
int push_llstack(llstack_t *S, void *k);

/* Pop a key from a linked list stack.
 * S must be a pointer to the stack.
 * Returns on sucess a pointer to the key poped or NULL otherwise. */
void* pop_llstack(llstack_t *S);

#endif /* _LLSTACK_H */

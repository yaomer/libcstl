#ifndef _STACK_H
#define _STACK_H

#include "../queue_array.h"

typedef struct {
    Queue *push;
    Queue *pop;
} Stack;

Stack   *stack_init(void);
void    stack_push(Stack *, int);
int     stack_pop(Stack *);
int     is_stack_full(Stack *);
int     is_stack_empty(Stack *);
void    stack_clear(Stack **);

#endif  /* _STACK_H */

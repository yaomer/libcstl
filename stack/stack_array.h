#ifndef _STACK_ARRAY_H
#define _STACK_ARRAY_H

#define MAXVAL 100

typedef struct {
    int val[MAXVAL];
    int sp;
} Stack;

Stack   *init(void);
void    push(Stack *, int);
int     pop(Stack *);
int     top(Stack *);
void    clear(Stack **);
int     isfull(Stack *);
int     isempty(Stack *);

#endif  /* _STACK_ARRAY_H */

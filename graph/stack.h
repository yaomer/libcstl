#ifndef _STACK_H
#define _STACK_H

typedef struct node {
    char *s;
    struct node *next;
} Stack;

void    stack_push(Stack **, char *);
void    stack_free(Stack *);
Stack   *stack_pop(Stack **);
void    stack_clear(Stack **);
int     is_stack_empty(Stack *);

#endif  /* _STACK_H */

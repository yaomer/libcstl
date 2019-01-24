#ifndef _STACK_LIST_H
#define _STACK_LIST_H

typedef struct node {
    int val;
    struct node *next;
} Stack;

void    push(Stack **, int);
void    pop(Stack **);
int     top(Stack *);
void    clear(Stack **);
int     isfull(Stack *);
int     isempty(Stack *);

#endif  /* _STACK_LIST_H */

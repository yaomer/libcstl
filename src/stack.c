#include <string.h>

#include "alloc.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"

#define S_VECTOR 1
#define S_LIST   2
#define S_DEQUE  3

typedef struct __stack {
    void *stack;
    int adaptor;
} Stack;

typedef void (*__stack_free_handler)(void *);

Stack *stack_init(const char *name, __stack_free_handler sfree)
{
    Stack *s = Malloc(sizeof(Stack));
    if (name && strcmp(name, "vector") == 0) {
        s->adaptor = S_VECTOR;
        s->stack = vector_init_p();
        vector_set_free_handler(s->stack, sfree);
    } else if (name && strcmp(name, "list") == 0) {
        s->adaptor = S_LIST;
        s->stack = list_init(NULL);
        list_set_free_handler(s->stack, sfree);
    } else {
        s->adaptor = S_DEQUE;
        s->stack = deque_init_p();
        deque_set_free_handler(s->stack, sfree);
    }
    return s;
}

#define __do_stack(s, x, y, z) \
    switch ((s)->adaptor) { \
    case S_VECTOR: x; break; \
    case S_LIST: y; break;\
    case S_DEQUE: z; break; \
    }

bool stack_empty(Stack *s)
{
    __do_stack(s, return vector_empty(s->stack),
                  return list_empty(s->stack),
                  return deque_empty(s->stack));
}

size_t stack_size(Stack *s)
{
    __do_stack(s, return vector_size(s->stack),
                  return list_size(s->stack),
                  return deque_size(s->stack));
}

void *stack_top(Stack *s)
{
    __do_stack(s, return vector_back_p(s->stack),
                  return list_back(s->stack),
                  return deque_back_p(s->stack));
}

void stack_push(Stack *s, void *data)
{
    __do_stack(s, vector_push_back(s->stack, data),
                  list_push_back(s->stack, data),
                  deque_push_back(s->stack, data));
}

void stack_pop(Stack *s)
{
    __do_stack(s, vector_pop_back(s->stack),
                  list_pop_back(s->stack),
                  deque_pop_back(s->stack));
}

void stack_clear(Stack *s)
{
    __do_stack(s, vector_clear(s->stack),
                  list_clear(s->stack),
                  deque_clear(s->stack));
}

void stack_free(Stack *s)
{
    __do_stack(s, vector_free(s->stack),
                  list_free(s->stack),
                  deque_free(s->stack));
    free(s);
}

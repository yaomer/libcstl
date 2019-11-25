#ifndef _LIBCSTL_STACK_H
#define _LIBCSTL_STACK_H

#include <stddef.h>
#include <stdbool.h>

/* 因为stack_t与系统的定义冲突，所以不得不改用Stack了 */
typedef struct __stack Stack;

typedef void (*__stack_free_handler)(void *);

/* 初始化一个栈
 * stack是一个适配器，name提供其底层所使用的容器，默认为deque
 * name = [vector, list, deque] */
Stack *stack_init(const char *name/*=deque*/, __stack_free_handler sfree);
/* 返回栈是否为空 */
bool stack_empty(Stack *s);
/* 返回栈中存储的元素数 */
size_t stack_size(Stack *s);
/* 返回栈顶元素 */
void *stack_top(Stack *s);
/* 向栈中压入一个元素 */
void stack_push(Stack *s, void *data);
/* 弹出栈顶元素 */
void stack_pop(Stack *s);
/* 清空栈 */
void stack_clear(Stack *s);
/* 释放一个栈 */
void stack_free(Stack *s);

#endif // _LIBCSTL_STACK_H

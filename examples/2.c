#include <libcstl/vector.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * deque的用法与vector类似
 */

void f1(void)
{
    vector_t *v = vector_init(sizeof(int));
    for (int i = 0; i < 10; i++)
        vector_push_back(v, &i);
    /* 通过下标遍历 */
    for (int i = 0; i < 10; i++)
        printf("%d ", *(int*)vector_entry(v, i));
    printf("\n");
    /* 通过迭代器遍历 */
    vector_iterator iter = vector_begin(v);
    do { printf("%d ", *(int*)vector_get(iter)); } while (vector_next(iter));
    printf("\n");
    vector_free_iterator(iter);
    vector_free(v);
}

void f2(void)
{
    vector_t *v = vector_init_p();
    vector_push_back(v, "hello world");
    vector_push_back(v, "kiss me");
    size_t size = vector_size(v);
    for (size_t i = 0; i < size; i++)
        printf("%s\n", (char*)vector_entry_p(v, i));
}

void freestr(void *str)
{
    free(str);
}

void f3(void)
{
    vector_t *v = vector_init_p();
    vector_set_free_handler(v, freestr);
    vector_push_back(v, strdup("hello world"));
    vector_push_back(v, strdup("kiss me"));
}

struct foo {
    int a;
    char *b;
};

struct foo *allocfoo(int a, char *b)
{
    struct foo *f = malloc(sizeof(struct foo));
    f->a = a;
    f->b = b;
    return f;
}

void freefoo(void *foo)
{
    struct foo *f = (struct foo *)foo;
    free(f->b);
    free(f);
}

void f4(void)
{
    vector_t *v = vector_init_p();
    vector_set_free_handler(v, freefoo);
    struct foo *foo = allocfoo(1, strdup("hello"));
    vector_push_back(v, foo);
}

int main(void)
{
    f1();
    f2();
    f3();
    f4();
}

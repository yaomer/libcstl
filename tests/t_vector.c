#include <libcstl/vector.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_int(void)
{
    printf("<test_int>\n");
    vector_t *v = vector_init(sizeof(int));
    for (int i = 0; i < 10; i++)
        vector_push_back(v, &i);
    for (int i = 0; i < 10; i++)
        printf("%d ", *(int*)vector_entry(v, i));
    printf("\n");
    printf("capacity: %zu, size: %zu\n", vector_capacity(v), vector_size(v));
    vector_shrink_to_fit(v);
    printf("capacity: %zu, size: %zu\n", vector_capacity(v), vector_size(v));
    vector_pop_back(v);
    vector_erase(v, vector_begin(v));
    vector_iterator it = vector_begin(v);
    do {
        printf("%d ", *(int*)vector_get(it));
    } while (vector_next(it));
    printf("\n");
    vector_free_iterator(it);
    vector_free(v);
}

void test_double(void)
{
    printf("<test_double>\n");
    vector_t *v = vector_init(sizeof(double));
    double a = 1.1, b = 2.22, c = 3.333;
    vector_push_back(v, &a);
    vector_push_back(v, &b);
    vector_push_back(v, &c);
    int size = vector_size(v);
    for (int i = 0; i < size; i++)
        printf("%f ", *(double*)vector_entry(v, i));
    printf("\n");
    vector_free(v);
}

void test_string(void)
{
    printf("<test_string>\n");
    vector_t *v = vector_init(sizeof(char*));
    const char *s1 = "hello, world";
    const char *s2 = "not only sql";
    const char *s3 = "face to face";
    /* 如果字符串在堆上分配，则需要设置free_handler */
    vector_push_back(v, (void*)&s1);
    vector_push_back(v, (void*)&s2);
    vector_push_back(v, (void*)&s3);
    int size = vector_size(v);
    for (int i = 0; i < size; i++)
        printf("%s\n", *(char**)vector_entry(v, i));
}

struct foo {
    char *name;
    int score;
    int rank;
};

void v1free(void *data)
{
    struct foo *f = (struct foo *)data;
    free(&(f->name));
}

struct foo *alloc_foo(char *name, int score, int rank)
{
    struct foo *f = malloc(sizeof(*f));
    f->name = strdup(name);
    f->score = score;
    f->rank = rank;
    return f;
}

void v2free(void *data)
{
    struct foo *f = (struct foo *)data;
    free(f->name);
    free(f);
}

void test_struct(void)
{
    printf("<test_struc_foo>\n");
    vector_t *v1 = vector_init(sizeof(struct foo));
    vector_set_free_handler(v1, v1free);
    struct foo f1 = { strdup("John"), 100, 1 };
    struct foo f2 = { strdup("Mike"), 80, 3 };
    struct foo f3 = { strdup("Lily"), 65, 5 };
    vector_push_back(v1, &f1);
    vector_push_back(v1, &f2);
    vector_push_back(v1, &f3);
    int size = vector_size(v1);
    for (int i = 0; i < size; i++) {
        struct foo *f = (struct foo *)vector_entry(v1, i);
        printf("%s, %d, %d\n", f->name, f->score, f->rank);
    }
    vector_free(v1);
    printf("<test_struc_foo_*>\n");
    /* sizeof(struct foo *)亦可，因为所有类型指针是一样长的 */
    vector_t *v2 = vector_init(sizeof(void*));
    vector_set_free_handler(v2, v2free);
    struct foo *f4 = alloc_foo("Bob", 99, 1);
    struct foo *f5 = alloc_foo("Gennis", 88, 3);
    struct foo *f6 = alloc_foo("Yeak", 77, 5);
    vector_push_back(v2, (void*)&f4);
    vector_push_back(v2, (void*)&f5);
    vector_push_back(v2, (void*)&f6);
    size = vector_size(v2);
    for (int i = 0; i < size; i++) {
        struct foo *f = *(struct foo **)vector_entry(v2, i);
        printf("%s, %d, %d\n", f->name, f->score, f->rank);
    }
    vector_free(v2);
}

int main(void)
{
    test_int();
    test_double();
    test_string();
    test_struct();
}

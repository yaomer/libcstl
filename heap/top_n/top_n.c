#include <stdio.h>
#include "../heap.h"

/*
 * 从1000,000个数中找出前100小的
 */
int
main(void)
{
    int i, a;
    Heap *hp = heap_init();

    /* 先读取前100个数，构建成一个最大堆 */
    for (i = 1; i < 101; i++)
        scanf("%d", &hp->val[i]);
    build_max_heap(hp, 100);
    heap_delete(hp);

    /* 每次剔除最大堆元素，再从剩余数中读入一个，插入到堆中...
     * 最后剩下的即为前100小的数
     */
    for ( ; i <= 1000000; i++) {
        scanf("%d", &a);
        heap_insert(hp, a);
        heap_delete(hp);
    }

    /* 打印出前100小的数 */
    for (i = 1; i < 101; i++)
        printf("%d%c", hp->val[i], i % 10 == 0 ? '\n' : ' ');

    heap_destroy(&hp);
    return 0;
}

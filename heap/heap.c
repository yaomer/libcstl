/*
 * (二叉)堆可分为最大堆和最小堆，两者的操作是类似的，
 * 下面我们以最大堆为例，给出堆的基本操作的实现和它的两个主要应用：
 * 堆排序和优先队列
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "heap.h"

/*              一个最大堆
 *       <9>
 *      /   \           1  2  3  4  5  6  7  8  9  index
 *     8     7    <==> [9][8][7][6][4][1][2][3][5]
 *    / \   / \
 *   6   4 1   2
 *  / \
 * 3   5
 */

Heap *
heap_init(void)
{
    Heap *hp;

    malloc_node(hp, Heap);
    bzero(hp, sizeof(*hp));

    return hp;
}

void
heap_destroy(Heap **hp)
{
    assert(hp);
    free(*hp);
    *hp = NULL;
}

/*
 * 维护最大堆的性质
 */

/*
 *        <9>                         <9>
 *       /   \      max_heap(5)      /   \
 *      5     7   ==============>   8     7    一个最大堆
 *     / \   / \  沿5-8-6进行调整  / \   / \
 *    8   4 1   2                 6   4 1   2
 *   / \                         / \
 *  3   6                       3   5
 */
static void
max_heap(Heap *hp, int i)
{
    int l, r, largest;

    l = left(i);
    r = right(i);
    /* 起初没有检查l,r，导致了一些莫名其妙的bug */

    assert(hp);
    /*  找出val[i]、val[l]、val[r]中的最大值  */
    if (l > hp->heapsize)
        return;
    if (hp->val[l] > hp->val[i])
        largest = l;
    else
        largest = i;
    if (r > hp->heapsize)
        goto next;
    if (hp->val[r] > hp->val[largest])
        largest = r;
    /* 如果val[i]是最大的，那么以该节点为根的子树已满足最大堆的性质，函数结束；
     * 否则，最大元素是其某个孩子节点，我们交换val[i]和val[largest]，从而使val[i]
     * 及其孩子都满足最大堆的性质
     */
next:
    if (largest != i) {
        swap(hp->val[i], hp->val[largest]);
        /* 当val[largest]成为val[i]之后，以该节点为根的子树可能会违反最大堆的性质
         * 所以要对该子树递归调用max_heap
         */
        max_heap(hp, largest);
    }
}

/*
 * 构建一个最大堆就是对于以a[n / 2] .. a[n]为根节点的每个子树，
 * 分别执行一次max_heap，使它们都满足最大堆的性质即可
 * 借用下面的heap_insert函数同样可以构建成一个最大堆
 */
void
build_max_heap(Heap *hp, int n)
{
    assert(hp);
    hp->heapsize = n;
    for (int i = n / 2; i >= 1; i--)
        max_heap(hp, i);
}

/*
 * 堆排序过程先利用build_max_heap将整个数组构建成为一个最大堆。
 * 然后通过把最大堆元素a[1]与a[n]交换，我们就将它放到了正确的位置上。
 * 然后我们从堆中去掉该节点(a[1])，可以通过减少heapsize的值来实现。
 * 新的根节点可能会违背最大堆的性质，所以我们对该节点执行max_heap以维护最大堆的性质，
 * 从而在a[1] .. a[n - 1]上构建出一个新的最大堆。
 * 堆排序会不断重复这一过程，直到堆的大小下降为2。
 */

/*
 * 待排序数组
 * [3][2][5][7][6][1][8]
 *       <3>                           <8>                   <5>
 *      /   \    build_max_heap       /   \    swap(8, 5)   /   \       max_heap(1)
 *     2     5   ==============>     7     3   =========>  7     3 [8]  ==========>
 *    / \   / \                     / \   / \             / \   /
 *   7   6 1   8                   2   6 1   5           2   6 1
 *       <7>                     1
 *      /   \    swap(7, 1)     / \            max_heap(1)
 *     6     3   =========>    6   3 [7][8]    ==========>  ...  [1][2][3][5][6][7][8]
 *    / \   /                 / \
 *   2   5 1                 2   5
 */
void
heap_sort(Heap *hp, int n)
{
    build_max_heap(hp, n);
    for (int i = n; i >= 2; i--) {
        swap(hp->val[1], hp->val[i]);
        hp->heapsize--;
        max_heap(hp, 1);
    }
}

/*
 * 最大优先队列
 */

/*
 * 为了在堆中插入一个元素，我们先对堆进行扩充(通过增加heapsize的值来实现)，
 * 然后将待插入的元素key放到扩充后的位置上去，接下来对于节点key，我们沿树向上进行调整，
 * 如果它的值大于它的父节点的值，我们就将它与其父节点进行交换...直到它的值不大于它的父节点的值，
 * 这时就满足了最大堆的性质，即将key插入到了堆中正确的位置上。
 */

/*
 *      8                  <8>                         <8>
 *     / \   insert 7     /   \    沿7-3-8进行调整    /   \
 *    5   3  =======>    5     3   ==============>   5     7
 *   / \                / \   /                     / \   /
 *  2   4              2   4 7                     2   4 3
 */
void
heap_insert(Heap *hp, int key)
{
    int i, j;

    assert(hp);
    if ((i = ++hp->heapsize) >= MAXVAL)
        error("heap overflow");

    hp->val[i] = key;
    j = parent(i);
    while (i > 1 && hp->val[i] > hp->val[j]) {
        swap(hp->val[i], hp->val[j]);
        i = j;    /*  更新i的值  */
        j = parent(i);
    }
}

/*
 * 去掉并返回最大堆元素(过程类似于堆排序)
 */
int
heap_delete(Heap *hp)
{
    int max;

    assert(hp);
    if (hp->heapsize < 1)
        error("heap underflow")

    max = hp->val[1];
    hp->val[1] = hp->val[hp->heapsize--];
    /*  将堆的最后一个元素赋给最大堆元素，然后缩减堆，即去除堆的最后一个元素  */
    max_heap(hp, 1);
    return max;
}

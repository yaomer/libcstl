#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * 产生1000000个随机数
 */
int
main(void)
{
    int i;

    for (i = 0; i < 1000000; i++) {
        if (i % 1000 == 999)
            srand(clock());
        printf("%d%c", rand() % 1000000, i % 10 == 9 ? '\n' : ' ');
    }
}

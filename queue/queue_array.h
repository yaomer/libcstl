#ifndef _QUEUE_ARRAY_H
#define _QUEUE_ARRAY_H

#define MAXVAL 100

typedef struct queue {
    int val[MAXVAL];
    int head;
    int tail;
} Queue;

Queue   *queue_init(void);
void    queue_push(Queue *, int);
int     queue_pop(Queue *);
int     queue_top(Queue *);
void    queue_clear(Queue **);
int     is_queue_full(Queue *);
int     is_queue_empty(Queue *);

#endif  /* _QUEUE_ARRAY_H */

#ifndef _QUEUE_LIST_H
#define _QUEUE_LIST_H

typedef struct queue {
    int val;
    struct queue *next;
} _Queue;

typedef struct {
    _Queue *front;
    _Queue *rear;
} Queue;

Queue   *queue_init(void);
void    queue_push(Queue *, int);
void    queue_pop(Queue *);
int     queue_top(Queue *);
void    queue_clear(Queue **);
int     is_queue_full(Queue *);
int     is_queue_empty(Queue *);

#endif  /* _QUEUE_LIST_H */

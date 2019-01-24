#ifndef _SKIPL_H
#define _SKIPL_H

typedef struct skipl {
    int val;
    struct skipl *next;
    struct skipl *down;
} _Skipl;

typedef struct {
    _Skipl *top;
    int level;
} Skipl;

#endif  /* _SKIPL_H */

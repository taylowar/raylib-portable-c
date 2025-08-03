
#ifndef CAKE_HEAP_H_
#define CAKE_HEAP_H_

#define HEAP_ITEM int

typedef struct {
    size_t size;
    size_t capacity;
    HEAP_ITEM *es;
} Heap;

#ifdef CAKE_HEAP_IMPLEMENTAION
#endif // CAKE_HEAP_IMPLEMENTAION
#endif // CAKE_HEAP_H_


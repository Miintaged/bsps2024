#ifndef MYQUEUE_H
#define MYQUEUE_H

#define MYQUEUE_H_

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/queue.h>

typedef struct {
    int x;
    int y;
} HELLO_MOTHERFUCKER;

struct myqueue_entry {
    HELLO_MOTHERFUCKER value;
    STAILQ_ENTRY(myqueue_entry) entries;
};

typedef struct {
    STAILQ_HEAD(myqueue_head, myqueue_entry) head;
    pthread_mutex_t mutex;
} myqueue_t;

static void myqueue_init(myqueue_t *q) {
    STAILQ_INIT(&q->head);
    pthread_mutex_init(&q->mutex, NULL);
}

static bool myqueue_is_empty(myqueue_t *q) {
    bool empty;
    pthread_mutex_lock(&q->mutex);
    empty = STAILQ_EMPTY(&q->head);
    pthread_mutex_unlock(&q->mutex);
    return empty;
}

static void myqueue_push(myqueue_t *q, HELLO_MOTHERFUCKER value) {
    struct myqueue_entry *entry = (struct myqueue_entry *)malloc(sizeof(struct myqueue_entry));
    entry->value = value;
    pthread_mutex_lock(&q->mutex);
    STAILQ_INSERT_TAIL(&q->head, entry, entries);
    pthread_mutex_unlock(&q->mutex);
}

static HELLO_MOTHERFUCKER myqueue_pop(myqueue_t *q) {
    assert(!myqueue_is_empty(q));
    pthread_mutex_lock(&q->mutex);
    struct myqueue_entry *entry = STAILQ_FIRST(&q->head);
    HELLO_MOTHERFUCKER value = entry->value;
    STAILQ_REMOVE_HEAD(&q->head, entries);
    free(entry);
    pthread_mutex_unlock(&q->mutex);
    return value;
}

static void myqueue_destroy(myqueue_t *q) {
    while (!myqueue_is_empty(q)) {
        myqueue_pop(q);
    }
    pthread_mutex_destroy(&q->mutex);
}

#endif

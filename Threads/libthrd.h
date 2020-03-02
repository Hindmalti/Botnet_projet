#ifndef LIBTHRD_H
#define LIBTHRD_H

#define MAX_MUTEX     128
#define MUTEX_THREAD  0

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

/**** Structures ****/

/**
 *  thread_param_t
 *  Arguments des threads
 *
 * thread_param_t permet de passer plusieurs arguments
 * à un thread qui ne prend qu'un void* en paramètre
 *
 */

typedef struct thread_param {
    void *arg;
    void (*fonction)(void *);
} thread_param_t;

/**** Prototypes ****/

int lanceThread(void (*)(void *), void *, int);
void P(int);
void V(int);

#endif
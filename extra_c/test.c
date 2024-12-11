#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutexes[2];

int main()
{
    pthread_mutex_init(&mutexes[1], NULL);
    pthread_mutex_init(&mutexes[2], NULL);
    pthread_thread_create()
}

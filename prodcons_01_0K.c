#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXPRODUCING 10
#define MAXAPPENDING 10
#define MAXTAKING 5
#define MAXCONSUMING 5

#define _wait(a) sleep(rand() % a)
#define out(s) \
    printf(s); \
    fflush(stdout)
#define outi(s, n) \
    printf(s, n); \
    fflush(stdout)

int n;
sem_t s, delay;

void produce();
void append();
void consume();
void take();

// --- FUNCIONES DE LOS HILOS (FALTABAN) ---
void *producer(void *data)
{
    while (1)
    {
        produce();
        sem_wait(&s);
        append();
        n = n + 1;
        outi("[P] \t \t item: %d\n", n);
        sem_post(&s);
        sem_post(&delay); // Avisa SIEMPRE que hay un nuevo ítem
    }
    pthread_exit(0);
}

void *consumer(void *data)
{
    while (1)
    {
        sem_wait(&delay); // Espera primero a que haya un ítem
        sem_wait(&s);
        take();
        int m = n; // Variable auxiliar 'm' solicitada en el enunciado
        outi("[C] \t \t item: %d\n", m);
        n = n - 1;
        sem_post(&s);
        consume();
    }
    pthread_exit(0);
}
// -----------------------------------------

/** main() function */
int main(int argc, char *argv[])
{
    pthread_t *consumer_pt, *producer_pt;
    srand(time(NULL));
    sem_init(&s, 0, 1);
    sem_init(&delay, 0, 0);
    consumer_pt = (pthread_t *)malloc(sizeof(pthread_t));
    producer_pt = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_create(consumer_pt, NULL, consumer, NULL);
    pthread_create(producer_pt, NULL, producer, NULL);
    pthread_exit(0);
}

void produce()
{
    out("[P] Producing\n");
    _wait(MAXPRODUCING);
    out("[P] Produced\n");
}

void append()
{
    out("[P] \t Appending\n");
    _wait(MAXAPPENDING);
    out("[P] \t Appended\n");
}

void take()
{
    out("[C] Taking\n");
    _wait(MAXTAKING);
    out("[C] Taked\n");
}

void consume()
{
    out("[C] \t Consuming\n");
    _wait(MAXCONSUMING);
    out("[C] \t Consumed\n");
}

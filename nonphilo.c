#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

sem_t chopsticks[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;

void *philosopher(void *arg) {
    int id = *(int *)arg;

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking\n", id);
        sleep(2);

        // Pick up chopsticks
        pthread_mutex_lock(&mutex);
        state[id] = HUNGRY;
        printf("Philosopher %d is hungry\n", id);
        pthread_mutex_unlock(&mutex);

        sem_wait(&chopsticks[id]);
        sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);

        // Eating
        pthread_mutex_lock(&mutex);
        state[id] = EATING;
        printf("Philosopher %d is eating\n", id);
        pthread_mutex_unlock(&mutex);

        sleep(2);

        // Put down chopsticks
        sem_post(&chopsticks[id]);
        sem_post(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);

        // Back to thinking
        pthread_mutex_lock(&mutex);
        state[id] = THINKING;
        printf("Philosopher %d is thinking again\n", id);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    int ids[NUM_PHILOSOPHERS];
    pthread_t philosophers[NUM_PHILOSOPHERS];

    // Initialize semaphores for chopsticks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up semaphores and mutex
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}
//clang -o nonphilo nonphilo.c
//./nonphilo


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_EATING_CYCLES 2

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < NUM_EATING_CYCLES; i++) {
        printf("Philosopher %d is sitting down\n", id);

        pthread_mutex_lock(&chopsticks[left]);
        printf("Philosopher %d picks the left chopstick\n", id);
        usleep(100000);  // Simulate some eating time

        pthread_mutex_lock(&chopsticks[right]);
        printf("Philosopher %d picks the right chopstick\n", id);
        printf("Philosopher %d begins to eat\n", id);
        usleep(100000);  // Simulate some eating time

        pthread_mutex_unlock(&chopsticks[left]);
        printf("Philosopher %d leaves the left chopstick\n", id);

        pthread_mutex_unlock(&chopsticks[right]);
        printf("Philosopher %d leaves the right chopstick\n", id);

        printf("Philosopher %d is thinking\n", id);
    }

    printf("Philosopher %d is shutting down\n", id);

    return NULL;
}

int main() {
    int ids[NUM_PHILOSOPHERS];
    pthread_t philosophers[NUM_PHILOSOPHERS];

    // Initialize mutexes for chopsticks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    return 0;
}
// danyalnemati@Danyals-MacBook-Pro bankers_example_code % clang -o effdiningphilo effdiningphilo.c

// danyalnemati@Danyals-MacBook-Pro bankers_example_code % ./effdiningphilo                        

// Philosopher 0 is sitting down
// Philosopher 0 picks the left chopstick
// Philosopher 1 is sitting down
// Philosopher 1 picks the left chopstick
// Philosopher 2 is sitting down
// Philosopher 2 picks the left chopstick
// Philosopher 3 is sitting down
// Philosopher 3 picks the left chopstick
// Philosopher 4 is sitting down
// Philosopher 4 picks the left chopstick
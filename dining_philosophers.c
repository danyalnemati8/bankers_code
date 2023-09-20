#include <stdio.h>
#include <pthread.h>
#include <dispatch/dispatch.h> // Include this header for dispatch semaphores
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

dispatch_semaphore_t chopsticks[NUM_PHILOSOPHERS];
dispatch_semaphore_t mutex;

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    // Alternate between thinking and eating multiple times
    for (int i = 0; i < 2; i++) {
        // Thinking
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        // Pick up chopsticks
        dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(chopsticks[left], DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(chopsticks[right], DISPATCH_TIME_FOREVER);
        dispatch_semaphore_signal(mutex);

        // Eating
        printf("Philosopher %d is eating\n", id);
        sleep(1);

        // Put down chopsticks
        dispatch_semaphore_signal(chopsticks[left]);
        dispatch_semaphore_signal(chopsticks[right]);
    }

    printf("Philosopher %d is done\n", id);

    return NULL;
}

int main() {
    int ids[NUM_PHILOSOPHERS];

    // Initialize dispatch semaphores for chopsticks and mutex
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        chopsticks[i] = dispatch_semaphore_create(1);
    }
    mutex = dispatch_semaphore_create(1);

    // Create philosopher threads
    pthread_t philosophers[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up dispatch semaphores (not necessary, but good practice)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        dispatch_release(chopsticks[i]);
    }
    dispatch_release(mutex);

    return 0;
}
/*
Philosopher 0 is thinking
Philosopher 1 is thinking
Philosopher 2 is thinking
Philosopher 3 is thinking
Philosopher 4 is thinking
Philosopher 0 is eating
Philosopher 2 is eating
Philosopher 0 is thinking
Philosopher 2 is thinking
Philosopher 3 is eating
Philosopher 3 is thinking
Philosopher 1 is eating
Philosopher 4 is eating
Philosopher 1 is thinking
Philosopher 4 is thinking
Philosopher 0 is eating
Philosopher 2 is eating
Philosopher 0 is done
Philosopher 2 is done
Philosopher 3 is eating
Philosopher 1 is eating
Philosopher 1 is done
Philosopher 3 is done
Philosopher 4 is eating
Philosopher 4 is done
*/

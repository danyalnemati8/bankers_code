//Danyal Nemati and Joel Anil John
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>


sem_t semaphores[5];
pthread_t T[5];
pthread_attr_t attr[5];

void* thinking(int ph){
    printf("Philosopher %d is thinking\n", ph);
}

void eating(int ph){
    printf("Philosopher %d is eating\n", ph);
}

void* putDownChopsticks(int ph){

    int right = (ph + 1) % 5;
    int left = (ph + 5) % 5;
    sem_post(&semaphores[left]);
    printf("Philosopher %d leaves the left chopstick\n",ph);
    sem_post(&semaphores[right]);
    printf("Philosopher %d leaves the right chopstick\n",ph);
        
    return 0;
}

void* pickUpChopsticks(int param){
    int ph= param;
    int right = (ph + 1) % 5;
    int left = (ph + 5) % 5;
        if(ph == 1){
            sem_wait(&semaphores[left]);
            printf("Philosopher %d picks the left chopstick\n",ph);
            sem_wait(&semaphores[right]);
            printf("Philosopher %d picks the right chopstick\n",ph);

            printf("Philosopher %d begins to eat\n",ph);

            sleep(2);
            
        } else{
            sem_wait(&semaphores[right]);
            printf("Philosopher %d picks the right chopstick\n",ph);
            sem_wait(&semaphores[left]);
            printf("Philosopher %d picks the left chopstick\n",ph);
            
            
            printf("Philosopher %d begins to eat\n",ph);

            sleep(2);

            
        }
        
    return 0;
}

void* philosopher_runner(int param){
    printf("Philosopher %d is sitting down\n",param);
    int count = 2;
    while(count > 0){
        thinking(param);
        pickUpChopsticks(param);
        eating(param);
        putDownChopsticks(param);
        thinking(param);
        count--;
    }
    printf("Philosopher %d is shutting down\n",param);
}

int main(int argc, const char* argv[]){
    int i = 0;
    int n[5];
    
    for(int i = 0; i<5; i++){
        sem_init(&semaphores[i], 0, 1);
    }
    for (i = 0; i < 5; ++i) {
        pthread_attr_init(&attr[i]);
    }
    for(int i = 0; i<5; i++){
        pthread_create(&T[i], &attr[i], philosopher_runner, (void *)i);
    }
    for(int i = 0; i<5; i++){
        pthread_join(T[i], NULL);
    }

    return 0;
}

// Output:
// Philosopher 1 is sitting down
// Philosopher 1 is thinking
// Philosopher 1 picks the left chopstick
// Philosopher 1 picks the right chopstick
// Philosopher 1 begins to eat
// Philosopher 2 is sitting down
// Philosopher 2 is thinking
// Philosopher 2 picks the right chopstick
// Philosopher 3 is sitting down
// Philosopher 3 is thinking
// Philosopher 3 picks the right chopstick
// Philosopher 4 is sitting down
// Philosopher 4 is thinking
// Philosopher 4 picks the right chopstick
// Philosopher 0 is sitting down
// Philosopher 0 is thinking
// Philosopher 1 is eating
// Philosopher 1 leaves the left chopstick
// Philosopher 1 leaves the right chopstick
// Philosopher 1 is thinking
// Philosopher 1 is thinking
// Philosopher 1 picks the left chopstick
// Philosopher 1 picks the right chopstick
// Philosopher 1 begins to eat
// Philosopher 1 is eating
// Philosopher 1 leaves the left chopstick
// Philosopher 1 leaves the right chopstick
// Philosopher 1 is thinking
// Philosopher 1 is shutting down
// Philosopher 2 picks the left chopstick
// Philosopher 2 begins to eat
// Philosopher 0 picks the right chopstick
// Philosopher 2 is eating
// Philosopher 2 leaves the left chopstick
// Philosopher 2 leaves the right chopstick
// Philosopher 2 is thinking
// Philosopher 2 is thinking
// Philosopher 2 picks the right chopstick
// Philosopher 2 picks the left chopstick
// Philosopher 2 begins to eat
// Philosopher 2 is eating
// Philosopher 2 leaves the left chopstick
// Philosopher 2 leaves the right chopstick
// Philosopher 2 is thinking
// Philosopher 2 is shutting down
// Philosopher 3 picks the left chopstick
// Philosopher 3 begins to eat
// Philosopher 3 is eating
// Philosopher 3 leaves the left chopstick
// Philosopher 3 leaves the right chopstick
// Philosopher 3 is thinking
// Philosopher 3 is thinking
// Philosopher 4 picks the left chopstick
// Philosopher 4 begins to eat
// Philosopher 4 is eating
// Philosopher 4 leaves the left chopstick
// Philosopher 4 leaves the right chopstick
// Philosopher 4 is thinking
// Philosopher 4 is thinking
// Philosopher 0 picks the left chopstick
// Philosopher 0 begins to eat
// Philosopher 3 picks the right chopstick
// Philosopher 3 picks the left chopstick
// Philosopher 3 begins to eat
// Philosopher 3 is eating
// Philosopher 3 leaves the left chopstick
// Philosopher 3 leaves the right chopstick
// Philosopher 3 is thinking
// Philosopher 3 is shutting down
// Philosopher 0 is eating
// Philosopher 0 leaves the left chopstick
// Philosopher 0 leaves the right chopstick
// Philosopher 0 is thinking
// Philosopher 0 is thinking
// Philosopher 0 picks the right chopstick
// Philosopher 0 picks the left chopstick
// Philosopher 0 begins to eat
// Philosopher 0 is eating
// Philosopher 0 leaves the left chopstick
// Philosopher 0 leaves the right chopstick
// Philosopher 0 is thinking
// Philosopher 0 is shutting down
// Philosopher 4 picks the right chopstick
// Philosopher 4 picks the left chopstick
// Philosopher 4 begins to eat
// Philosopher 4 is eating
// Philosopher 4 leaves the left chopstick
// Philosopher 4 leaves the right chopstick
// Philosopher 4 is thinking
// Philosopher 4 is shutting down

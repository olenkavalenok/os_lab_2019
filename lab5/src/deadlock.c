#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

void resource_one(int *);
void resource_two(int *);
void do_wrap_up(int);
int common = 0;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

int main() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1,NULL,(void*)resource_one,(void*)&common)!=0){
        perror("pthread_create");
        exit(1);
    }
    
    if (pthread_create(&thread2,NULL,(void*)resource_two,(void*)&common)!=0){
        perror("pthread_create");
        exit(1);
    }
    
    if (pthread_join(thread1,NULL)!=0){
        perror("pthread_join");
        exit(1);
    }
    
    if (pthread_join(thread2,NULL)!=0){
        perror("pthread_join");
        exit(1);
    }
    
    return 0;
}

void resource_one(int *pnum_times){
    pthread_mutex_lock(&lock1);
    printf("Start in resource_one\n");
    sleep(1);
    printf("Trying to take resource_two\n");
    pthread_mutex_lock(&lock2);
    printf("Taken resource_two\n");
    pthread_mutex_unlock(&lock1);
    printf("Finish in resource_one\n");
    pthread_mutex_unlock(&lock2);
    pthread_exit(NULL);
}

void resource_two(int *pnum_times){
    pthread_mutex_lock(&lock2);
    printf("Start in resource_two\n");
    sleep(1);
    printf("Trying to take resource_one\n");
    pthread_mutex_lock(&lock1);
    printf("Taken resource_one\n");
    pthread_mutex_unlock(&lock2);
    printf("Finish in resource_two\n");
    pthread_mutex_unlock(&lock1);
    pthread_exit(NULL);
}

void do_wrap_up(int counter){
    int total;
    printf("All done, counter = %d\n", counter);
}
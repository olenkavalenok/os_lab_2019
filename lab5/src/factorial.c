#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <getopt.h>

struct Arguments{
    uint64_t* arr;
    uint64_t begin;
    uint64_t end;
    uint64_t m;
};

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
uint64_t shared;

void *ThreadSum(void *args){
    struct Arguments *a = (struct Arguments *)args;
    for(uint64_t i = a->begin; i < a->end; i++){
        pthread_mutex_lock(&mut);
        *(a->arr) = (*(a->arr)*i)%a->m;
        pthread_mutex_unlock(&mut);
    }
  return 0;
}

int main(int argc, char **argv){
    shared = 1;
    uint32_t k = -1;
    uint32_t pnum = -1;
    uint32_t mod = -1;
    static struct option options[] = {{"k", required_argument, 0, 0},
                                        {"pnum", required_argument, 0, 0},
                                        {"mod", required_argument, 0, 0},
                                        {0, 0, 0, 0}};
    int option_index = 0;
    while(1){
        int c = getopt_long(argc, argv, "f", options, &option_index);
        if (c == -1)
        break;
        switch(c){
        case 0:
        switch(option_index){
            case 0:
            k = atoi(optarg);
            if(k < 0)
            k = -1;
            break;
            case 1:
            pnum = atoi(optarg);
            if(pnum < 1)
            pnum = -1;
            break;
            case 2:
            mod = atoi(optarg);
            if(mod < 1)
            mod = -1;
            break;
        }
        break;
        default:
        printf("Index %d is out of options\n", option_index);
        }
    }

    if (optind < argc){
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (k == -1 || pnum == -1 || mod == -1){
        printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
        return 1;
    }
    
    pthread_t threads[pnum];
    struct Arguments a[pnum];
    for(uint64_t i = 0; i < pnum; i++){
        a[i].arr = &shared;
        a[i].begin = i*k/pnum + 1;
        a[i].end = (i == (pnum - 1)) ? k + 1 : (i+1)*k/pnum + 1;
        a[i].m = mod;
    }
    
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    for (uint32_t i = 0; i < pnum; i++){
        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&a[i])){
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }
    
    for (uint32_t i = 0; i < pnum; i++){
        pthread_join(threads[i], NULL);
    }
    
    printf("result: %lu\n", shared);
    return 0;
}
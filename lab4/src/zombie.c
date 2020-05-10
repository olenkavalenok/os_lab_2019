#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t chld_pid;
    chld_pid = fork();
    if (chld_pid > 0) //Parent
    {
        printf("Parent process!");
        sleep(100); 
        printf("Parent process! Prog finished");
    }
    else //Child
    {
        printf("Child process! Prog finished");
        exit(0);
    }
    return 0;
}
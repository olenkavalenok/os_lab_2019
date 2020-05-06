#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

int main(int c, char **argv)
{
char *argv1[4]={" ","2","8",NULL};
int pid=fork();
if (pid==0)
{
execv("sequiential_min_max", argv1);
return 0;
}
return(0);
}
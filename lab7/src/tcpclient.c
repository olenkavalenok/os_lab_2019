#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]){
    int fd;
    int nread;
    int SERV_PORT = -1;
    int BUFSIZE = -1;
    char IP[16] = {'\0'};

    while (1){
        int current_optind = optind ? optind : 1;
        static struct option options[] = {{"SERV_PORT", required_argument, 0, 0},
                                        {"BUFSIZE", required_argument, 0, 0},
                                        {"IP",required_argument,0,0},
                                        {0, 0, 0, 0}};
        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);
        if (c == -1)
        break;
        switch (c){
            case 0:{
                switch (option_index){
                    case 0:
                    SERV_PORT = atoi(optarg);
                    if (SERV_PORT <= 0){
                        printf("SERV_PORT is a positive number\n");
                        return 1;
                    }
                    break;
                    case 1:
                    BUFSIZE = atoi(optarg);
                    if (BUFSIZE <= 0){
                        printf("BUFSIZE is a positive number\n");
                        return 1;
                    }
                    break;
                    case 2:
                    strcpy(IP, optarg);
                    if (strlen(IP) <= 0)
                    return 0;
                    break;
                    default:
                    printf("Index %d is out of options\n", option_index);
                }
            } break;
            case '?':
            printf("Unknown argument\n");
            break;
            default:
            fprintf(stderr, "getopt returned character code 0%o?\n", c);
        }
    }
    
    if (SERV_PORT == -1 || BUFSIZE == -1 || strlen(IP)==0){
        fprintf(stderr, "Using: %s --SERV_PORT 10050 --BUFSIZE 100 --IP 127.0.0.1\n", argv[0]);
        return 1;
    }

    char buf[BUFSIZE];
  
    struct sockaddr_in servaddr;
    if (argc < 3){
        printf("Too few arguments \n");
        exit(1);
    }
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket creating");
        exit(1);
    }

    memset(&servaddr, 0, SIZE);
    servaddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0){
        perror("bad address");
        exit(1);
    }
  
    servaddr.sin_port = htons(SERV_PORT);

    if (connect(fd, (SADDR *)&servaddr, SIZE) < 0){
        perror("connect");
        exit(1);
    }
    
    write(1, "Input message to send: \n", 22);
    while ((nread = read(0, buf, BUFSIZE)) > 0){
        if (write(fd, buf, nread) < 0){
            perror("write");
            exit(1);
        }
    }
    
    close(fd);
    exit(0);
}
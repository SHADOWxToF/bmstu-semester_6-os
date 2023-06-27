#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <netinet/in.h>

#define SOCK_NAME "socket.soc"
#define PORT 65535

int fd;

void handler()
{
    close(fd);
    exit(0);
}

int main() 
{
    srand(time(0));
    signal(SIGINT, &handler);
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket problem");
        exit(1);
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(PORT);
    
    if (connect(fd, (struct sockaddr *) &server, sizeof(server)) == -1)
    {
        perror("connect problem");
        close(fd);
        exit(1);
    }
    char buf[100];
    sprintf(buf, "client with pid %d ask a server pid", getpid());
    printf("asking a server pid...\n");
    write(fd, buf, 50);
    read(fd, buf, 100);
    printf("recieved message: %s\n", buf);
    close(fd);
    return 0;
}

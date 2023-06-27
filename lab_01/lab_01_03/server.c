#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define SOCK_NAME "server.soc"
#define MAXCLIENT 10

int fd;

void handler(int signal)
{
    unlink(SOCK_NAME);
    close(fd);
    exit(0);
}

int main()
{
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket problem");
        return 1;
    }
    struct sockaddr server, client;
    server.sa_family = AF_UNIX;
    strcpy(server.sa_data, SOCK_NAME);
    if (bind(fd, &server, sizeof(server)) == -1)
    {
        perror("bind problem");
        close(fd);
        return 1;
    }
    signal(SIGINT, &handler);
    char buf[50];
    int size = sizeof(client);
    while (1)
    {
        if (recvfrom(fd, buf, 50, 0, &client, &size) == -1)
        {
            perror("ошибка при recvfrom");
            unlink(SOCK_NAME);
            close(fd);
            return 1;
        }
        char newbuf[100];
        printf("server with pid %d recieved message from client with pid %s\n", getpid(), buf);
        sprintf(newbuf, "message from server: %s+%d=%d", buf, getpid(), atoi(buf) + getpid());
        if (sendto(fd, newbuf, sizeof(newbuf), 0, &client, sizeof(client)) == -1)
        {
            perror("ошибка при sendto");
            unlink(SOCK_NAME);
            close(fd);
            return 1;
        }
    }
    unlink(SOCK_NAME);
    close(fd);
    return 0;
}

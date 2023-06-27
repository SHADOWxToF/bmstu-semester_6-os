#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define CLIENT_SOCK "client.soc"
#define SERVER_SOCK "server.soc"

int fd;

void handler()
{
    unlink(CLIENT_SOCK);
    close(fd);
    exit(0);
}

int main() 
{
    srand(time(0));
    signal(SIGINT, &handler);
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket problem");
        return 1;
    }
    struct sockaddr server, client;
    client.sa_family = AF_UNIX;
    strcpy(client.sa_data, CLIENT_SOCK);
    server.sa_family = AF_UNIX;
    strcpy(server.sa_data, SERVER_SOCK);
    if ((bind(fd, &client, sizeof(client))) == -1)
    {
        perror("connect problem");
        close(fd);
        return 1; 
    }
    char buf[50];
    sprintf(buf, "%d", getpid());
    if (sendto(fd, buf, sizeof(buf), 0, &server, sizeof(server)) == -1)
    {
        perror("ошибка при sendto");
        unlink(CLIENT_SOCK);
        close(fd);
        return 1;
    }
    if (recvfrom(fd, buf, 50, 0, NULL, NULL) == -1) 
    {
        perror("ошибка при recvfrom");
        unlink(CLIENT_SOCK);
        close(fd);
        return 1;
    }
    printf("%s\n", buf);
    unlink(CLIENT_SOCK);
    close(fd);
    return 0;
}

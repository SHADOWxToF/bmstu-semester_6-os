#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define SOCK_NAME "server.soc"

int fd;

void handler(int signal)
{
    unlink(SOCK_NAME);
    close(fd);
    exit(0);
}

int main(int args, char *argv[])
{
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket problem");
        return 1;
    }
    struct sockaddr srvr_name, client_name;
    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME);
    if (bind(fd, &srvr_name, sizeof(srvr_name)) == -1)
    {
        perror("bind problem");
        close(fd);
        return 1;
    }
    signal(SIGINT, &handler);
    char buf[50];
    while(1)
    {
        if (recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL) == -1)
        {
            perror("ошибка при recvfrom");
            unlink(SOCK_NAME);
            close(fd);
            return 1;
        }
        printf("%s\n", buf);
    }
    unlink(SOCK_NAME);
    close(fd);
    return 0;
}

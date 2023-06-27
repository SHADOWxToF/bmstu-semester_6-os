#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SOCK_NAME "server.soc"

int main() 
{
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket problem");
        return 1;
    }
    struct sockaddr srvr_name;
    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME);
    char buf[50];
    printf("sending message to server...\n");
    sprintf(buf, "message from client with pid = %d", getpid());
    if (sendto(fd, buf, strlen(buf) + 1, 0, &srvr_name, sizeof(srvr_name)) == -1) 
    {
        perror("ошибка при sendto");
        close(fd);
        exit(1);
    }
    printf("message was sended\n");
    close(fd);
    return 0;
}

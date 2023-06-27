#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
# include <unistd.h>


int main(int argc, char **argv)
{
    int sockets[2];
    char buf[50];
    char *msg[3] = {"aaa", "bbb", "ccc"};
    int pid;
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0)
    {
        perror("socketpair() failed");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 3; ++i)
    {
        if ((pid = fork()) == -1)
        {
            perror("fork() failed");
            return EXIT_FAILURE;
        }
        else if (pid == 0) 
        {
            close(sockets[1]);
            sleep(2 * i);
            write(sockets[0], msg[i], sizeof(msg[i]));
            close(sockets[0]);
            break;
        }
    }
    if (pid)
    {
        close(sockets[0]);
        for (int i = 0; i < 3; ++i)
        {
            read(sockets[1], buf, sizeof(buf));
            printf("%s\n", buf);
        }
        close(sockets[1]);
    }
}
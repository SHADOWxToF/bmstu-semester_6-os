#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/epoll.h>


#define SOCK_NAME "server.soc"
#define MAX_CLIENTS 10
#define PORT 65535
#define MAX_EVENTS 10

int listenfd;

void handler(int signal)
{
    unlink(SOCK_NAME);
    close(listenfd);
    exit(0);
}

int main()
{
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket problem");
        return 1;
    }
    if (fcntl(listenfd, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl problem");
        close(listenfd);
        return 1;
    }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // регистрация на всех адресах
    server.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr *) &server, sizeof(server)) == -1)
    {
        perror("bind problem");
        close(listenfd);
        return 1;
    }
    
    signal(SIGINT, &handler);
    
    if (listen(listenfd, MAX_CLIENTS) == -1)
    {
        perror("listen problem");
        unlink(SOCK_NAME);
        close(listenfd);
        return 1;
    }
    
    int epollfd = epoll_create(MAX_CLIENTS);
    if (epollfd == -1)
    {
        perror("epoll problem");
        unlink(SOCK_NAME);
        close(listenfd);
        return 1;
    }
    
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
    {
        perror("epoll problem");
        unlink(SOCK_NAME);
        close(listenfd);
        return 1;
    }
    
    printf("server has a pid: %d\n", getpid());

    while (1)
    {   
        int nfds;
        if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1)
        {
            perror("epoll problem");
            unlink(SOCK_NAME);
            close(listenfd);
            return 1;
        }
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == listenfd)
            {
                int new_socketfd;
                if ((new_socketfd = accept(listenfd, NULL, NULL)) == -1)
                {
                    perror("accept problem");
                    unlink(SOCK_NAME);
                    close(listenfd);
                    return 1;
                }
                
                if (fcntl(new_socketfd, F_SETFL, O_NONBLOCK) == -1)
                {
                    perror("fcntl problem");
                    close(listenfd);
                    return 1;
                }
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_socketfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, new_socketfd, &ev) == -1)
                {
                    perror("epoll problem");
                    unlink(SOCK_NAME);
                    close(listenfd);
                    return 1;
                }
            }
            else
            {
                char buf[50], buf2[100];
                if (read(events[i].data.fd, buf, 50))
                {
                    printf("%s\n", buf);
                    sprintf(buf2, "%s; server pid: %d", buf, getpid());
                    write(events[i].data.fd, buf2, 100);
                }
                else
                    close(events[i].data.fd);
            }
        }
        
    }
    close(epollfd);
    unlink(SOCK_NAME);
    close(listenfd);
    return 0;
}

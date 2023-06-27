#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

void *thread(void *args)
{
    char c;
    int fd = open("alphabet.txt",O_RDONLY);
    int flag = 1;
    while (flag == 1)
    {
        if ((flag = read(fd, &c, 1)) == 1)
            write(1, &c, 1);
    }
    close(fd);
    return NULL;
}

int main()
{
    pthread_t th1;
    pthread_t th2;
    pthread_create(&th1, NULL, thread, NULL);
    pthread_create(&th2, NULL, thread, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}
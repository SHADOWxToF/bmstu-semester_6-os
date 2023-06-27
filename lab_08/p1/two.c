#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>

void *thread(void *args)
{
    int fd = *(int *) args;
    FILE *fs = fdopen(fd,"r");
    char buff[20];
    setvbuf(fs,buff,_IOFBF,20); // full buffering
    int flag = 1;
    while(flag == 1)
    {
        char c;
        flag = fscanf(fs,"%c",&c);
        if (flag == 1) 
        {
            fprintf(stdout,"%c",c);
        }
    }
    fclose(fs);
    return NULL;
}

int main()
{
    int fd = open("alphabet.txt",O_RDONLY);
    pthread_t th1;
    pthread_t th2;
    pthread_create(&th1, NULL, thread, &fd);
    pthread_create(&th2, NULL, thread, &fd);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}
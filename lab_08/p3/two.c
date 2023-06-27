#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

void *thread(void *args)
{
    int shift = *(int *) args;
    FILE *file = fopen("new_alphabet.txt", "w");
    char c = 'a' + shift;
    while (c <= 'z')
    {
        fprintf(file, "%c", c);
        c += 2;
    }
    struct stat filestat;
    fstat(file->_fileno, &filestat);
    printf("shift=%d, inode=%lu, size=%ld\n", shift, filestat.st_ino, filestat.st_size);
    fclose(file);
    stat("new_alphabet.txt", &filestat);
    printf("shift=%d, inode=%lu, size=%ld\n", shift, filestat.st_ino, filestat.st_size);
    return NULL;
}

int main()
{
    pthread_t th1;
    pthread_t th2;
    int shift1 = 0;
    int shift2 = 1;
    pthread_create(&th1, NULL, thread, &shift1);
    pthread_create(&th2, NULL, thread, &shift2);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}
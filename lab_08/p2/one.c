#include <unistd.h>
#include <fcntl.h>

int main()
{
    char c;    
    int fd1 = open("alphabet.txt",O_RDONLY);
    int fd2 = open("alphabet.txt",O_RDONLY);
    int flag = 1;
    while(flag == 1)
    {
        if ((flag = read(fd1,&c,1)) == 1)
            write(1,&c,1);
        if (flag == 1 && (flag = read(fd2,&c,1)) == 1)
            write(1,&c,1);
    }
    close(fd1);
    close(fd2);
    return 0;
}
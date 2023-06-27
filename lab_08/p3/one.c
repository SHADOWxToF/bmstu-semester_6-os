#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    FILE *file1 = fopen("new_alphabet.txt", "w");
    FILE *file2 = fopen("new_alphabet.txt", "w");
    char c = 'a';
    int flag = 0;
    while (c <= 'z')
        (flag = !flag) ? fprintf(file1, "%c", c++) : fprintf(file2, "%c", c++);
    struct stat filestat;
    fstat(file1->_fileno, &filestat);
    printf("inode=%lu, size=%ld\n", filestat.st_ino, filestat.st_size);
    fclose(file1);
    
    fstat(file2->_fileno, &filestat);
    printf("inode=%lu, size=%ld\n", filestat.st_ino, filestat.st_size);
    fclose(file2);
    
    stat("new_alphabet.txt", &filestat);
    printf("inode=%lu, size=%ld\n", filestat.st_ino, filestat.st_size);
    return 0;
}
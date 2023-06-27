# include <stdio.h>
# include <string.h>
# include <dirent.h>
# include <unistd.h>
# include <stdlib.h>

# define BUF_SIZE 500

void environ(char *buf)
{
    char name[50];
    sprintf(name, "%s/environ", buf);
    FILE *f = fopen(name, "r");
    char buffer[BUF_SIZE];
    int len;
    while ((len = fread(buffer, 1, BUF_SIZE, f)) > 0)
    {
    for (int i = 0; i < len; i++)
        if ( buffer[i] == 0)
         buffer[i] = 10;
         buffer[len - 1] = 0;
        printf("%s",  buffer);
    }
    fclose(f);
}

void state(char *buf)
{
    char name[50];
    sprintf(name, "%s/stat", buf);
    FILE *f = fopen(name, "r");
    char buffer[BUF_SIZE];
    fread(buffer, 1, BUF_SIZE, f);
    char *pch = strtok(buffer, " ");
    
    printf("stat: \n");
    int i = 1;
    while(i <= 52 && pch != NULL)
    {
        printf("%d - %s\n", i, pch);
        pch = strtok(NULL, " ");
        i++;
    }
    fclose(f);
}

void cmdline(char *buf)
{
    char name[50];
    sprintf(name, "%s/cmdline", buf);
    FILE *file = fopen(name, "r");
    char buffer[BUF_SIZE];
    fscanf(file, "%s", buffer);
    printf("\ncmdline: %s\nfd:\n", buffer);
    struct dirent *dirp;
    DIR *dp;
    char str[BUF_SIZE];
    char path[BUF_SIZE];
    sprintf(name, "%s/fd", buf);
    dp = opendir(name);
    while ((dirp = readdir(dp)) != NULL)
    {
        if ((strcmp(dirp->d_name, ".") != 0) &&
            (strcmp(dirp->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", name, dirp->d_name);
            readlink(path, str, BUF_SIZE);
            printf("%s -> %s\n", dirp->d_name, str);
        }
    }
    closedir(dp);
}

void maps(char *buf)
{
    char *line = NULL;
    int start_addr, end_addr, page_size = 4096;
    size_t line_size = BUF_SIZE;
    ssize_t line_length;
    size_t pages_cnt, total_pages = 0;

    char pathToOpen[BUF_SIZE];
    sprintf(pathToOpen, "%s/maps", buf);
    FILE *file = fopen(pathToOpen, "r");
    printf("\nMAPS:\n");
    int lengthOfRead;
    do
    {
        line_length = getline(&line, &line_size, file);
        if (!feof(file) && line_length == -1)
        {
            perror("getline:");
            fclose(file);
            free(line);
            exit(1);
        }
        // line[line_length] = '\n';
        sscanf(line, "%x-%x", &start_addr, &end_addr);
        pages_cnt = (end_addr - start_addr) / page_size;
        total_pages += pages_cnt;
        printf("%ld\t%s\n", pages_cnt, line);
    } while (!feof(file));
    free(line);
    printf("Всего используется страниц: %lu\n", total_pages);
    fclose(file);

    char buff[BUF_SIZE];
    sprintf(buff,"%s/stat", buf);
    FILE *f = fopen(buff, "r");
    fread(buff, 1, BUF_SIZE, f);
    char *pch = strtok(buff, " ");
    
    int cnt = 1;
    while(cnt!= 23 && pch != NULL)
    {
        pch = strtok(NULL, " ");
        ++cnt;
    }
    fclose(f);
    if (cnt != 23)
        printf("Ошибка во время чтения файла stat\n");
    size_t stat_cnt;
    sprintf(buf, "%s", pch);
    sscanf(buf, "%lu", &stat_cnt);
    printf("Количество страниц согласно файлу stat: %lu\n", stat_cnt / page_size);
}

int main()
{
    char buf[50];
    int pid;
    printf("Введите pid: ");
    scanf("%d", &pid);
    sprintf(buf, "/proc/%d", pid);
    FILE *f = fopen(buf, "r");
    int choice = -1;
    while (choice)
    {
        printf("\n\n\nВыберите необходимую задачу и введите её номер для запуска.\n");
        printf("1 - информацию об окружении процесса\n");
        printf("2 - информацию о состоянии процесса\n");
        printf("3 - информация из файла cmdline и директории fd\n");
        printf("4 - информация из файла maps\n");
        printf("0 - выход\n\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        char ch = 1;
        while (ch != '\n')
            scanf("%c", &ch);
        switch (choice)
        {
        case 1:
            environ(buf);
            choice = -1;
            break;
        case 2:
            state(buf);
            choice = -1;
            break;
        case 3:
            cmdline(buf);
            choice = -1;
            break;
        case 4:
            maps(buf);
            choice = -1;
        case 0:
            break;
        default:
            printf("\nНеверный ввод, повторите попытку!!!\n\n");
            break;
        }
    }
}
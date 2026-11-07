#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>

#define BLUE "\e[1;34m"
#define RESET "\e[0m"	


static void format_size(char *out, size_t out_size, long long bytes)
{
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    double size = (double)bytes;
    int i = 0;

    while (size >= 1024.0 && i < 4)
    {
        size /= 1024.0;
        i++;
    }

    if (i == 0)
        snprintf(out, out_size, "%lld %s", bytes, units[i]);
    else
        snprintf(out, out_size, "%.1f %s", size, units[i]);
}

static void print_entry(const char *name, int dir, int indent, const char *size)
{
    if (indent)
        printf("%*s|-> ", indent, "");

    if (dir)
        printf(BLUE "%s" RESET "\n", name);
    else
        printf("%s (%s)\n", name, size);
}


int is_dir(const char *path)
{
    struct stat st;
    if(stat(path, &st) != 0)
    {
        return 0;
    }
    return S_ISDIR(st.st_mode);
}

int walk(const char *path, int indent)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("opendir");
        return 1;
    }

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL)
    {
        const char *name = dp-> d_name;
        if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        char full[1024];
        snprintf(full, sizeof(full), "%s/%s", path, name);

        struct stat st;
        if(stat(full, &st) != 0)
            continue;
        
        if(S_ISDIR(st.st_mode))
        {
            //is dir
            print_entry(name, 1, indent, "");
            walk(full, indent + 3);
        }
        else
        {
            //is file
            char size[32];
            format_size(size, sizeof(size), (long long)st.st_size);
            print_entry(name, 0, indent, size);
        }
    }

    closedir(dir);
    return 0;
}

int main(int argc, char **argv)
{
    //default = current dir
    const char *path = ".";

    if (argc > 1)
    {
        path = argv[1];
    }
    int err = walk(path, 0);
    return err;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <unistd.h>
#include <string.h>

char *pathcat(const char *str1, char *str2);

int main()
{
    struct dirent *dp;
    char *fullpath;
    
    char wd[PATH_MAX];
    getcwd(wd, PATH_MAX);
    char const *jpg = ".jpg";
    
    //const char *path="C:\\test\\"; // Directory target
    DIR *dir = opendir(wd); // Open the directory - dir contains a pointer to manage the dir
    while (dp=readdir(dir)) // if dp is null, there's no more content to read
    {
        fullpath = pathcat(wd, dp->d_name);
        printf("%s\n", fullpath);
        char *ext = strrchr(fullpath, '.');
        if(ext)
        {
        	printf("\t%s\n", ext);
        	printf("\t\tis jpg: %s\n",strcmp(ext, jpg)?"no":"yes");
        }
        free(fullpath);
    }
    closedir(dir); // close the handle (pointer)
    return 0;
}

char *pathcat(const char *str1, char *str2)
{
    char *res;
    size_t strlen1 = strlen(str1);
    size_t strlen2 = strlen(str2);
    int i, j;
    res = (char *) calloc(strlen1+strlen2+2, sizeof(char)); 
    strcpy(res, str1);
    res[strlen1]='/';
    for (i=strlen1+1, j=0; ((i<(strlen1+strlen2+1)) && (j<strlen2)); i++, j++)
        res[i] = str2[j];
    res[strlen1+strlen2+2] = '\0';
    return res;
}

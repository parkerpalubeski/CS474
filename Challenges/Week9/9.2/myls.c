#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/*
 * Takes a dirname as a string ("." for current directory) and shows the
 * contents of that directory.
 */
void ls(const char *dirname)
{
    DIR* d = opendir(dirname);
    if(d == NULL){ perror("opendir"); return;}

    struct dirent *ent;
    while((ent = readdir(d))!= NULL){
        //construct the path
        //for some reason i decided to do this manually
        char fullpath[4096];
        int i = 0;
        while(dirname[i]!='\0'){
            fullpath[i] = dirname[i];
            i++;
        }
        fullpath[i] = '/';
        i++;
        int j = 0;
        while(ent->d_name[j]!='\0'){
            fullpath[i] = ent->d_name[j];
            i++;
            j++;
        }
        fullpath[i] = '\0';


        //read with stat
        struct stat s;
        if(stat(fullpath, &s) == -1){ //error check
            perror("stat");
            printf("%s", ent->d_name);
            return;
        }
        printf("%-11s: %lu %o %d %d %ld %ld %ld\n", ent->d_name, s.st_ino, s.st_mode, s.st_uid, s.st_gid, s.st_size, s.st_blksize, s.st_blocks);
    }

}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: myls dirname\n");
        exit(1);
    }

    char *dirname = argv[1];
    
    ls(dirname);
}
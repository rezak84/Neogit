#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
// bakhsh add az roye seyed zadam (amir mohammad hamidi) avl talash
// kardam befahmam chi mige kheyli ghesmatasho  nafahmidam
// manzoram az bakhsh add tabe add va chizaii k lazem dasht hast

char neeogit_location[85];

// start part of syntax
int check_dir(char *path);
int init(int argc, char *const argv[]);
int config_local_username(char *username);
int config_local_email(char *email);
int config_global_username(char *username);
int config_global_email(char *email);
int add(char *name_off_ile);
int check_neeogit_exist();
int check_root();
int check_stagehistory();
int check_nee_root(); // hamitor k az esmesh malome nee va root rock mikone
int check_exist(char searchfile[], char orgfile[]);
int check_glob_exist();
void search_stagedfiles(int depth);
void search_name(char *path);
// end part of syntax

int config_global_username(char *username)
{
    // bayad set konm k harvagh ino  zad  bere  ro hame config proje ha
    // copy kone

    FILE *file = fopen("config.glob.name", "w");
    if (file == NULL)
    {
        perror("file not found");
        return 1;
    }
    fprintf(file, "username: %s", username);
    fclose(file);
    return 0;
}
int config_global_email(char *email)
{
    FILE *file = fopen("config.glob.email", "w");
    if (file == NULL)
    {
        perror("file not found");
        return 1;
    }
    fprintf(file, "email: %s", email);
    fclose(file);
    return 0;
}

// int check_glob_exist()
// {
//     DIR *dir = opendir(".neegit");
//     dir = chdir("..");
//     system("touch search.txt");
//     system("ls > search.txt");
//     FILE *file = fopen("search.txt", "r");
//     char search[85];
//     while (fgets(search, sizeof(search), file))
//     {
//         if (strcmp(search, "globconfig"))
//         {
//             system("rm search.txt");
//             fclose(file);
//             return 1;
//         }
//     }
//     system("rm search.txt");
//     fclose(file);
//     return 0;
// }
// int config_global_username(char *username)
// {
//     DIR *dir = opendir(".neeogit");
//     dir = chdir("..");
//     if (check_glob_exist() == 0)
//     {
//         FILE *creat = fopen("globconfig", "w");
//         fclose(creat);
//     }
//     FILE *file = fopen("globconfig", "r");
//     char copy[85];
// }

int init(int argc, char *const argv[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        return 1;
    }
    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do
    {
        // find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening current directory\n");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == 4 && strcmp(entry->d_name, ".neeogit") == 0)
            {
                exists = true;
            }
        }
        closedir(dir);
        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);
    // return to the initial cwd
    if (chdir(cwd) != 0)
        return 1;

    if (!exists)
    {
        if (mkdir(".neeogit", 0755) != 0)
        {
            return 1;
        }
        // creat stagearae for the add part
        FILE *poin2 = fopen(".neeogit/config.email", "w");
        FILE *poin3 = fopen(".neeogit/config.name", "w");
        fclose(poin2);
        fclose(poin3);
        FILE *poin4 = fopen("config.glob.name", "w");
        FILE *poin5 = fopen("config.glob.email", "w");
        fclose(poin4);
        fclose(poin5);
        system("mkdir .neeogit/stagearea");
    }
    else
    {
        perror("neeogit repository has already initialized\n");
    }
    return 0;
}

int config_local_username(char *username)
{
    FILE *file = fopen(".neeogit/config.name", "w");
    if (file == NULL)
    {
        return 1;
    }
    fprintf(file, "username: %s\n", username);
    fclose(file);
    return 0;
}
int config_local_email(char *email)
{
    FILE *file = fopen(".neeogit/config.email", "w");
    if (file == NULL)
    {
        return 1;
    }
    fprintf(file, "email: %s\n", email);
    fclose(file);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stdout, "please enter a valid command\n");
        return 1;
    }
    else if (strcmp(argv[1], "init") == 0)
    {
        init(argc, argv);
        return 0;
    }
    else if (strcmp(argv[1], "config") == 0)
    {
        // for global
        if (strcmp(argv[2], "-global") == 0)
        {
            if (strcmp(argv[3], "user.name") == 0)
            {
                config_global_username(argv[4]);
                return 0;
            }
            else if (strcmp(argv[3], "email") == 0)
            {
                config_global_email(argv[4]);
                return 0;
            }
            else
            {
                perror("hala k eshteba neveshti nomreh bede :)\n");
                return 1;
            }
        }
        // for local
        // i  think this part is correct
        else
        {
            if (strcmp(argv[2], "user.name") == 0)
            {
                config_local_username(argv[3]);
                return 0;
            }
            // for email
            else if (strcmp(argv[2], "email") == 0)
            {
                config_local_email(argv[3]);
                return 0;
            }
            else
            {
                perror("hala k eshtebah type kardi  nomreh bede :)\n");
                return 1;
            }
        }
    }
    // for add
    else if (strcmp(argv[1], "add") == 0)
    {
        if (testproject() == 0)
        {
            printf("you didn't initialized neogit in your project\n");
            return 0;
        }
        if (strcmp(argv[2], "-f") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                add_file(argv[i], 0);
            }
        }
        else if (strcmp(argv[2], "-n") == 0)
        {
            int depth;
            if (argv[3] == NULL)
            {
                depth = 1;
                search_stagedfiles(depth);
            }
            else
            {
                sscanf(argv[3], "%d", &depth);
                search_stagedfiles(depth);
            }
        }
        // else if (strcmp(argv[2], "-redo") == 0)
        // {
        //     char floc[80];
        //     system("pwd > pq.txt");
        //     FILE *present = fopen("pq.txt", "r");
        //     fgets(floc, 80, present);
        //     system("rm pq.txt");
        //     int len = strlen(floc);
        //     char loc[80];
        //     strcpy(loc, neeogit_location);
        //     strcat(loc, "/.neeogit/staged_files/reset.txt");
        //     FILE *reset = fopen(loc, "r");
        //     char search[80];
        //     while (fgets(search, 80, reset))
        //     {
        //         strcpy(search, search + len);
        //         int length = strlen(search);
        //         search[length - 1] = '\0';
        //         add_file(search, 1);
        //     }
        //     fclose(reset);
        // }
        else
        {
            add_file(argv[2], 0);
        }
    }

    else if (strcmp(argv[1], "reset") == 0)
    {
    }

    else
    {
        perror("chon eshteba nveshti nomreh bede\n");
    }

    return 0;
}
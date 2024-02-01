#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char neeogit_location[85];

int init(int argc, char *const argv[]);
int config_local_username(char *username);
int config_local_email(char *email);
int add(char *name_off_ile);
int check_neeogit_exist();
int check_root();

int check_root()
{
    system("touch alaki.txt");
    system("pwd > alaki.txt");
    FILE *file = fopen("alaki.txt", "r");
    if (strcmp(file, "/\n") == 0)
    {
        system("rm alaki.txt");
        fclose(file);
        return 1;
    }
    else
    {
        system("rm alaki.txt");
        fclose(file);
        return 0;
    }
}
// alave bar namesh y kare dige hame mikone va adress neeogit ro save mikone
int check_neeogit_exist()
{
    system("touch folder.txt");
    system("ls -a > folder.txt");
    FILE *file = fopen("folder.txt", "r");
    char check[85];
    int flag;
    while (fgets(check, sizeof(check), file))
    {
        // check for the neeogit
        if (strcmp(check, ".neeogit\n") == 0)
        {
            // y  motagheyer global tarif shod baraye save location neeogit
            system("touch location.txt");
            system("pwd > location.txt");
            FILE *location = fopen("location.txt", "r");
            fgets(neeogit_location, sizeof(neeogit_location), location);
            // injasho nafahmidam chera
            int lenth = strlen(neeogit_location);
            neeogit_location[lenth - 1] = '\0';
            // ta inja
            fclose(location);
            system("rm location.txt");
            flag = 1;
        }
    }
    fclose(file);
    system("rm folder.txt");
    if (flag == 1)
    {
        return 1;
    }
    return 0;
}
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
            perror("Error opening current directory");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == 4 && strcmp(entry->d_name, ".neogit") == 0)
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
        if (mkdir(".neogit", 0755) != 0)
        {
            return 1;
        }
        // creat stagearae for the add part
        FILE *poin1 = fopen(".neogit/stagearea", "w");
        FILE *poin2 = fopen(".neogit/config", "w");
        fclose(poin1);
        fclose(poin2);
    }
    else
    {
        perror("neogit repository has already initialized");
    }
    return 0;
}

int config_local_username(char *username)
{
    FILE *file = fopen(".neogit/config", "a");
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
    FILE *file = fopen(".neogit/config", "a");
    if (file == NULL)
    {
        return 1;
    }
    fprintf(file, "email: %s\n", email);
    fclose(file);
    return 0;
}

int add(char *name_of_file)
{
    // pwd strcat
    bool exist = false;
    FILE *file = fopen(".neeogit/stagearea", "r");
    if (file == NULL)
    {
        perror("erorr opening stagearea");
        return 1;
    }
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        perror("erorr opening now dir");
        return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, name_of_file) == 0)
        {
            perror("this file already exist in stagearea");
            return 1;
        }
        else
        {
            exist = true;
        }
    }
    fclose(file);
    if (exist)
    {
        FILE *stage = fopen(".neeogit/stagearea", "a");
        fprintf(stage, "%s\n", name_of_file);
        fclose(stage);
    }
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stdout, "please enter a valid command");
        return 1;
    }
    if (strcmp(argv[1], "init") == 0)
    {
        init(argc, argv);
        return 0;
    }
    if (strcmp(argv[1], "config") == 0)
    {
        // for global
        if (strcmp(argv[2], "-global") == 0)
        {
            // bakhsh global badan  age tonestam ok konm

            // if (strcmp(argv[3], "user.name") == 0)
            // {
            //     char *username;
            //     sscanf(argv[4], "%s", username);
            //     config_global_username(username);
            // }
            // // for email
            // else
            // {
            //     char *email;
            //     sscanf(argv[4], "%s", email);
            //     config_global_email(email);
            // }
        }
        // for local
        // i  think this part is correct
        else
        {
            if (strcmp(argv[2], "user.name") == 0)
            {
                char *username;
                sscanf(argv[3], "%s", username);
                config_local_username(username);
            }
            // for email
            else
            {
                char *email;
                sscanf(argv[3], "%s", email);
                config_local_email(email);
            }
        }
    }
    // for add
    if (strcmp(argv[1], "add") == 0)
    {
        int flag = 0, flagroot = 0;
        while (1)
        {
            if (check_neeogit_exist() == 0)
            {
                flag = 1;
                break;
            }
            if (root_check() == 1)
            {
                flagroot++;
            }
        }

        // add with -f
        if (strcmp(argv[2], "-f") == 0)
        {
        }
        // normal add
        else
        {
        }
    }

    return 0;
}
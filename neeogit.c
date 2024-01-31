#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int init(int argc, char *const argv[]);
// int config_global_username(char *usernaem);
// int config_global_email(char *email);
int config_local_username(char *username);
int config_local_email(char *email);
int add(char name_off_ile);

// for the creat folder we need in the init

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
        fopen(".neogit/stagearea", "w");
        fopen(".neogit/config", "w");
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

int add(char name_off_ile)
{
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
    if (strcmp(argv[1], "add"))
    {
    }

    return 0;
}
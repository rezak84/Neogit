#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
char neogit_project_location[60];
char lastfolder[90];
char time_path[90];
int check_exist(char searchfile[], char orgfile[])
{
    char search[80];
    strcpy(search, searchfile);
    strcat(search, "\n");
    FILE *file = fopen(orgfile, "r");
    char pathes[80];
    int flag = 0;
    while (fgets(pathes, 80, file))
    {
        if (strcmp(pathes, search) == 0)
        {
            flag++;
            break;
        }
    }
    fclose(file);
    return flag;
}
int find_file(char file_name[])
{
    char filecopy[50];
    strcpy(filecopy, file_name);
    strcat(filecopy, "\n");
    int test = 0;
    system("touch details.txt");
    system("ls -a > details.txt");
    FILE *file = fopen("details.txt", "r");
    char search[50];
    while (fgets(search, 50, file))
    {
        if (strcmp(search, filecopy) == 0)
        {
            test++;
            break;
        }
    }
    fclose(file);
    system("rm details.txt");
    return test;
}
// it has our present project .neogit folder location;
int check_neogit()
{
    int test = 0;
    system("touch details.txt");
    system("ls -a > details.txt");
    FILE *file = fopen("details.txt", "r");
    char search[70];
    while (fgets(search, 70, file))
    {
        if (strcmp(search, ".neogit\n") == 0)
        {
            // save our present neogit folder
            system("touch loc.txt");
            system("pwd > loc.txt");
            FILE *loc = fopen("loc.txt", "r");
            fgets(neogit_project_location, 60, loc);
            int len = strlen(neogit_project_location);
            neogit_project_location[len - 1] = '\0';
            fclose(loc);
            system("rm loc.txt");
            // finish;
            test++;
            break;
        }
    }
    if (test == 1)
    {
        fclose(file);
        system("rm details.txt");
        return 0;
    }
    fclose(file);
    system("rm details.txt");
    return 1;
}
int check_root()
{
    system("touch details.txt");
    system("pwd > details.txt");
    FILE *file = fopen("details.txt", "r");
    char search[60];
    fgets(search, 60, file);
    if (strcmp(search, "/\n") == 0)
    {
        fclose(file);
        system("rm details.txt");
        return 0;
    }
    else
    {
        fclose(file);
        system("rm details.txt");
        return 1;
    }
}
int testproject()
{
    int test = 0;
    int flag = 0;
    while (1)
    {
        if (check_neogit() == 0)
        {
            flag++;
            break;
        }
        else if (check_root() == 0)
        {
            test++;
        }
        if (test > 1)
        {
            break;
        }
        chdir("..");
    }
    return flag;
}
int assign_local(char *type, char *name)
{
    if (testproject() == 0)
    {
        printf("you didn't make any new project\n");
        return 0;
    }
    else if (strcmp(type, "user.name") == 0)
    {
        chdir(neogit_project_location);
        chdir(".neogit");
        FILE *file = fopen("account_name.txt", "w");
        fprintf(file, "%s", name);
        fclose(file);
    }
    else if (strcmp(type, "user.email") == 0)
    {
        chdir(neogit_project_location);
        chdir(".neogit");
        FILE *file = fopen("account_email.txt", "w");
        fprintf(file, "%s", name);
        fclose(file);
    }
    return 1;
}
void assign_global(char *type, char *name)
{
    char search[70];
    if (strcmp(type, "user.name") == 0)
    {
        FILE *global_username = fopen("/home/.neogit_app/global_names.txt", "w");
        fprintf(global_username, "%s", name);
        fclose(global_username);
    }
    else if (strcmp(type, "user.email") == 0)
    {
        FILE *global_useremail = fopen("/home/.neogit_app/global_emails.txt", "w");
        fprintf(global_useremail, "%s", name);
        fclose(global_useremail);
    }
}
// it is ok!
int checkdirectory(char *path)
{
    struct stat filestat;
    if (stat(path, &filestat) == 0)
    {
        if (S_ISDIR(filestat.st_mode))
        {
            return 1;
        }
        return 0;
    }
    return -1;
}
// it is ok!
int checktracked_files(char file[])
{
    char location[90];
    strcpy(location, neogit_project_location);
    strcat(location, "/.neogit/staged_files/trackted_files.txt");
    FILE *trackted_files = fopen(location, "r");
    char search[80];
    int flag = 0;
    while (fgets(search, 80, trackted_files))
    {
        if (strcmp(file, search) == 0)
        {
            flag++;
            break;
        }
    }
    fclose(trackted_files);
    return flag;
}
void writetrcktedfiles(char file[])
{
    char location[90];
    strcpy(location, neogit_project_location);
    strcat(location, "/.neogit/staged_files/trackted_files.txt");
    FILE *trackted_files = fopen(location, "a");
    fprintf(trackted_files, "%s", file);
    fclose(trackted_files);
}
int modifyfile(char file[], int mode)
{
    // file is path of our file;
    char path[80];
    char location[80];
    system("touch path.txt");
    system("pwd > path.txt");
    FILE *pathfile = fopen("path.txt", "r");
    fgets(path, 80, pathfile);
    fclose(pathfile);
    system("rm path.txt");
    int len = strlen(path);
    path[len - 1] = '/';
    strcpy(location, path);
    strcat(path, file);
    char main_path[80];
    strcpy(main_path, path);
    struct stat filestat;
    if (stat(path, &filestat) == 0)
    {
        strcat(path, "\n");
        time_t present = filestat.st_mtime;
        char timelineloc[80];
        char temp[80];
        strcpy(timelineloc, neogit_project_location);
        strcat(timelineloc, "/.neogit/commits/timeline.txt");
        strcpy(temp, neogit_project_location);
        strcat(temp, "/.neogit/commits/temp.txt");
        FILE *timeline = fopen(timelineloc, "r");
        FILE *tempfile = fopen(temp, "w");
        char search[70];
        int flag = 0;
        while (fgets(search, 70, timeline))
        {
            fprintf(tempfile, "%s", search);
            if (strcmp(search, path) == 0)
            {
                fgets(search, 70, timeline);
                if (strcmp(search, ctime(&present)) != 0)
                {
                    flag++;
                }
                fprintf(tempfile, "%s", search);
                flag++;
            }
        }
        if (flag == 0)
        {
            fprintf(tempfile, "%s", path);
            fprintf(tempfile, "%s", ctime(&present));
        }
        if (((flag == 0 || flag == 2) || (checktracked_files(path) == 0)) || (mode == 1))
        {
            if (checktracked_files(path) == 0)
            {
                writetrcktedfiles(path);
            }
            char loc[80];
            strcpy(loc, neogit_project_location);
            strcat(loc, "/.neogit/staged_files");
            char COMMAND[180];
            strcpy(COMMAND, "cp ");
            strcat(COMMAND, main_path);
            strcat(COMMAND, " ");
            strcat(COMMAND, loc);
            system(COMMAND);
            strcat(loc, "/staged_files.txt");
            FILE *staged_files = fopen(loc, "a");
            fprintf(staged_files, "%s", path);
            fprintf(staged_files, "%s", ctime(&present));
            fclose(staged_files);
        }
        fclose(timeline);
        fclose(tempfile);
        chdir(neogit_project_location);
        chdir(".neogit/commits");
        system("rm timeline.txt");
        system("mv temp.txt timeline.txt");
        chdir(location);
        return 1;
    }
    else
    {
        perror("Error");
        return -1;
    }
}
// it is ok!
int modify(int mode)
{
    int count = 0;
    system("touch file.txt");
    system("ls -a > file.txt");
    FILE *files = fopen("file.txt", "r");
    char search[50];
    char filenames[50][50];
    while (fgets(search, 50, files))
    {
        if (strcmp(search, "file.txt\n") != 0)
        {
            strcpy(filenames[count], search);
            count++;
        }
    }
    fclose(files);
    system("rm file.txt");
    for (int i = 0; i < count; i++)
    {
        int len = strlen(filenames[i]);
        filenames[i][len - 1] = '\0';
        char path[90];
        system("touch path.txt");
        system("pwd > path.txt");
        FILE *pathfile = fopen("path.txt", "r");
        fgets(path, 90, pathfile);
        int length = strlen(path);
        path[length - 1] = '/';
        strcat(path, filenames[i]);
        fclose(pathfile);
        system("rm path.txt");
        // path is compoletely true;
        int type = checkdirectory(path);
        char Astage[80];
        strcpy(Astage, neogit_project_location);
        strcat(Astage, "/.neogit/staged_files/Astagedfiles.txt");
        if ((((type == 1) && (strcmp(filenames[i], ".neogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0))
        {
            if (check_exist(path, Astage) == 0)
            {
                FILE *add = fopen(Astage, "a");
                fprintf(add, "%s", path);
                fprintf(add, "\n");
                fclose(add);
            }
            chdir(path);
            modify(mode);
            chdir("..");
        }
        else if (((strcmp(filenames[i], ".neogit") != 0)) && (type == 0))
        {
            if (check_exist(path, Astage) == 0)
            {
                FILE *add = fopen(Astage, "a");
                fprintf(add, "%s", path);
                fprintf(add, "\n");
                fclose(add);
                modifyfile(filenames[i], mode);
            }
        }
    }
}
// it is ok!
int add_file(char filepath[], int add_mode)
{
    char path[90];
    char Astage[80];
    system("touch path.txt");
    system("pwd > path.txt");
    FILE *pathfile = fopen("path.txt", "r");
    fgets(path, 90, pathfile);
    fclose(pathfile);
    system("rm path.txt");
    int len = strlen(path);
    path[len - 1] = '\0';
    strcat(path, "/");
    strcat(path, filepath);
    strcpy(Astage, neogit_project_location);
    strcat(Astage, "/.neogit/staged_files/Astagedfiles.txt");
    if (checkdirectory(path) == 1)
    {
        if (check_exist(path, Astage) == 0)
        {
            FILE *add = fopen(Astage, "a");
            fprintf(add, "%s", path);
            fprintf(add, "\n");
            fclose(add);
        }
        chdir(path);
        modify(add_mode);
        chdir("..");
    }
    else if (checkdirectory(path) == 0)
    {
        if (check_exist(path, Astage) == 0)
        {
            FILE *add = fopen(Astage, "a");
            fprintf(add, "%s", path);
            fprintf(add, "\n");
            fclose(add);
            modifyfile(filepath, add_mode);
        }
    }
    else
    {
        printf("there isn't any file or directory with this name\n");
        return 0;
    }
    return 1;
}
// it is ok!
void search_name(char *path)
{
    char location[80];
    char pathcopy[80];
    strcpy(pathcopy, path);
    strcat(pathcopy, "\n");
    strcpy(location, neogit_project_location);
    strcat(location, "/.neogit/staged_files/Astagedfiles.txt");
    FILE *Astagedfiles = fopen(location, "r");
    char search[80];
    int flag = 0;
    while (fgets(search, 80, Astagedfiles))
    {
        if (strcmp(pathcopy, search) == 0)
        {
            flag++;
            printf("%s", path);
            printf(" is in staging area.\n");
            break;
        }
    }
    if (flag == 0)
    {
        printf("%s", path);
        printf(" isn't in staging area.\n");
    }
}
void search_stagedfiles(int depth)
{
    int count = 0;
    system("touch file.txt");
    system("ls -a > file.txt");
    FILE *files = fopen("file.txt", "r");
    char search[50];
    char filenames[50][50];
    while (fgets(search, 50, files))
    {
        if (strcmp(search, "file.txt\n") != 0)
        {
            strcpy(filenames[count], search);
            count++;
        }
    }
    fclose(files);
    system("rm file.txt");
    for (int i = 0; i < count; i++)
    {
        int len = strlen(filenames[i]);
        filenames[i][len - 1] = '\0';
        char path[90];
        system("touch path.txt");
        system("pwd > path.txt");
        FILE *pathfile = fopen("path.txt", "r");
        fgets(path, 90, pathfile);
        int length = strlen(path);
        path[length - 1] = '/';
        strcat(path, filenames[i]);
        fclose(pathfile);
        system("rm path.txt");
        // path is compoletely true;
        int type = checkdirectory(path);
        if (((((type == 1) && (strcmp(filenames[i], ".neogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0)) && (depth > 1))
        {
            search_name(path);
            chdir(path);
            search_stagedfiles(depth - 1);
            chdir("..");
        }
        else if (((((type == 1) && (strcmp(filenames[i], ".neogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0)) && (depth == 1))
        {
            search_name(path);
        }
        else if (((strcmp(filenames[i], ".neogit") != 0)) && (type == 0))
        {
            search_name(path);
        }
    }
}
void addredo(char path[])
{
    char loc[80];
    strcpy(loc, neogit_project_location);
    strcat(loc, "/.neogit/staged_files/reset.txt");
    FILE *reset = fopen(loc, "a");
    fprintf(reset, "%s", path);
    fclose(reset);
}
int delstage(char path[], int mode)
{
    char loc[100];
    char temploc[100];
    char pathcopy[100];
    strcpy(pathcopy, path);
    strcat(pathcopy, "\n");
    strcpy(loc, neogit_project_location);
    strcat(loc, "/.neogit/staged_files/Astagedfiles.txt");
    FILE *Astagedfiles = fopen(loc, "r");
    strcpy(temploc, neogit_project_location);
    strcat(temploc, "/.neogit/staged_files/temp.txt");
    FILE *temp = fopen(temploc, "w");
    char search[100];
    while (fgets(search, 100, Astagedfiles))
    {
        if (strcmp(search, pathcopy) != 0)
        {
            fprintf(temp, "%s", search);
        }
    }
    fclose(Astagedfiles);
    fclose(temp);
    char remove[200];
    strcpy(remove, "rm ");
    strcat(remove, loc);
    system(remove);
    char rename[200];
    strcpy(rename, "mv ");
    strcat(rename, temploc);
    strcat(rename, " ");
    strcat(rename, loc);
    system(rename);
    if (mode == 0)
    {
        char stageloc[80];
        char stemploc[80];
        strcpy(stageloc, neogit_project_location);
        strcat(stageloc, "/.neogit/staged_files/staged_files.txt");
        FILE *staged_files = fopen(stageloc, "r");
        strcpy(stemploc, neogit_project_location);
        strcat(stemploc, "/.neogit/staged_files/stempfiles.txt");
        FILE *stemp = fopen(stemploc, "w");
        char search[80];
        while (fgets(search, 80, staged_files))
        {
            if (strcmp(search, pathcopy) == 0)
            {
                addredo(pathcopy);
                fgets(search, 80, staged_files);
                continue;
            }
            fprintf(stemp, "%s", search);
        }
        fclose(staged_files);
        fclose(stemp);
        char sremove[90];
        strcpy(sremove, "rm ");
        strcat(sremove, stageloc);
        system(sremove);
        char srename[120];
        strcpy(srename, "mv ");
        strcat(srename, stemploc);
        strcat(srename, " ");
        strcat(srename, stageloc);
        system(srename);
        // remove file from staged_files folder:
        char COMMAND[100];
        strcpy(COMMAND, "rm ");
        strcat(COMMAND, neogit_project_location);
        strcat(COMMAND, "/.neogit/staged_files/");
        char *ptr = strtok(path, "/");
        char file[80];
        while (1)
        {
            strcpy(file, ptr);
            ptr = strtok(NULL, "/");
            if (ptr == NULL)
                break;
        }
        strcat(COMMAND, file);
        system(COMMAND);
    }
}
void writetimeline()
{
    int count = 0;
    system("touch file.txt");
    system("ls -a > file.txt");
    FILE *files = fopen("file.txt", "r");
    char search[80];
    char filenames[80][80];
    while (fgets(search, 80, files))
    {
        if (strcmp(search, "file.txt\n") != 0)
        {
            strcpy(filenames[count], search);
            count++;
        }
    }
    fclose(files);
    system("rm file.txt");
    for (int i = 0; i < count; i++)
    {
        int len = strlen(filenames[i]);
        filenames[i][len - 1] = '\0';
        char path[90];
        system("touch path.txt");
        system("pwd > path.txt");
        FILE *pathfile = fopen("path.txt", "r");
        fgets(path, 90, pathfile);
        int length = strlen(path);
        path[length - 1] = '/';
        strcat(path, filenames[i]);
        fclose(pathfile);
        system("rm path.txt");
        // path is compoletely true;
        int type = checkdirectory(path);
        if ((((type == 1) && (strcmp(filenames[i], ".neogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0))
        {
            chdir(path);
            writetimeline();
            chdir("..");
        }
        else if (((strcmp(filenames[i], ".neogit") != 0)) && (type == 0))
        {
            char firstlocation[80];
            strcpy(firstlocation, neogit_project_location);
            strcat(firstlocation, "/.neogit/commits/timeline.txt");
            FILE *timeline = fopen(firstlocation, "a");
            struct stat filestat;
            if (stat(path, &filestat) == 0)
            {
                fprintf(timeline, "%s\n", path);
                time_t present = filestat.st_mtime;
                fprintf(timeline, "%s", ctime(&present));
            }
            fclose(timeline);
        }
    }
}

int main(int argc, char *argv[])
{
    // we pure all txt files that we need for our programm in .neogit_app folder in /home and we made that before;
    // now we have this 2D strig which have all commands:
    char allcommands[50][50];
    // first step is to build neogit init command
    if (strcmp(argv[1], "init") == 0)
    {
        // we save all locations of our projects in locations.txt in /home:
        FILE *location = fopen("/home/.neogit_app/locations.txt", "a");
        // check that if there is .neogit folder in our present directory or above not?
        system("touch project_location.txt");
        system("pwd > project_location.txt");
        FILE *file = fopen("project_location.txt", "r");
        char first_location[70];
        fgets(first_location, 70, file);
        fclose(file);
        system("rm project_location.txt");
        // get our present location;
        int test = 0;
        while (1)
        {
            if (test > 1)
            {
                break;
            }
            else if (check_neogit() == 0)
            {
                printf("you initialized neogit before in this project\n");
                break;
            }
            else if (check_root() == 0)
            {
                test++;
            }
            chdir("..");
        }
        if (test == 2)
        {
            int len = strlen(first_location);
            first_location[len - 1] = '\n';
            fprintf(location, "%s", first_location);
            first_location[len - 1] = '\0';
            chdir(first_location);
            strcpy(neogit_project_location, first_location);
            system("mkdir .neogit");
            chdir(".neogit");
            // we make staged files & primary directory in this place:
            system("mkdir staged_files");
            chdir("staged_files");
            system("touch trackted_files.txt");
            system("touch Astagedfiles.txt");
            chdir("..");
            system("mkdir commits");
            chdir("commits");
            system("touch allbranches.txt");
            system("touch HEAD.txt");
            chdir("../..");
            ////////
            writetimeline();
            chdir(".neogit");
            // now we are back in .neogit folder;
            // copy all project to our primary directories;
            // and also we make 2 account.txt for save our email and username
            char NAME[30], EMAIL[30];
            FILE *global_username = fopen("/home/.neogit_app/global_names.txt", "r+");
            char search1[30];
            if (fgets(search1, 30, global_username))
            {
                strcpy(NAME, search1);
                fclose(global_username);
            }
            else
            {
                fclose(global_username);
            }
            FILE *global_useremail = fopen("/home/.neogit_app/global_emails.txt", "r+");
            if (fgets(search1, 30, global_useremail))
            {
                strcpy(EMAIL, search1);
                fclose(global_useremail);
            }
            else
            {
                fclose(global_useremail);
            }
            FILE *account = fopen("account_name.txt", "w");
            fprintf(account, "%s", NAME);
            fclose(account);
            FILE *account2 = fopen("account_email.txt", "w");
            fprintf(account, "%s", EMAIL);
            fclose(account2);
        }
        fclose(location);
    }
    // user.name and user.email
    else if (strcmp(argv[1], "config") == 0)
    {
        if (strcmp(argv[2], "-global") == 0)
        {
            assign_global(argv[3], argv[4]);
        }
        else if (strcmp(argv[2], "-global") != 0)
        {
            assign_local(argv[2], argv[3]);
        }
    }
    // part of add:i am betee than this
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
        else if (strcmp(argv[2], "-redo") == 0)
        {
            char floc[80];
            system("pwd > pq.txt");
            FILE *present = fopen("pq.txt", "r");
            fgets(floc, 80, present);
            system("rm pq.txt");
            int len = strlen(floc);
            char loc[80];
            strcpy(loc, neogit_project_location);
            strcat(loc, "/.neogit/staged_files/reset.txt");
            FILE *reset = fopen(loc, "r");
            char search[80];
            while (fgets(search, 80, reset))
            {
                strcpy(search, search + len);
                int length = strlen(search);
                search[length - 1] = '\0';
                add_file(search, 1);
            }
            fclose(reset);
        }
        else
        {
            add_file(argv[2], 0);
        }
    }
}
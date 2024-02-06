#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
// bakhsh add az roye seyed zadam (amir mohammad hamidi) avl talash
// kardam befahmam chi mige kheyli ghesmatasho  nafahmidam
// manzoram az bakhsh add tabe add va chizaii k lazem dasht has

char neeogit_location[185];

// start part of syntax
int check_dir(char *path);
int init(int argc, char *const argv[]);
int config_local_username(char *username);
int config_local_email(char *email);
int config_global_username(char *username);
int config_global_email(char *email);
int add(char *name_off_ile, int add_mode);
int modify(int mode);
int modifyfile(char file[], int mode);
int check_neeogit_exist();
int check_root();
int check_stagehistory();
int check_nee_root(); // hamitor k az esmesh malome nee va root rock mikone
int check_exist(char searchfile[], char orgfile[]);
int check_glob_name_exist();  //
int check_glob_email_exist(); //
int search_stagedfiles(int depth);
int search_name(char *path);
// end part of syntax

int check_glob_name_exist()
{
    DIR *dir = opendir("/home");
    system("touch search.txt");
    system("ls > search.txt");
    FILE *file = fopen("search.txt", "r");
    if (file == NULL)
    {
        perror("segmen check glob");
        return 1;
    }
    char search[185];
    while (fgets(search, sizeof(search), file) != NULL)
    {
        if (strcmp(search, "config.glob.name.txt") == 0)
        {
            fclose(file);
            system("rm search.txt");
            return 0;
        }
    }
    fclose(file);
    system("rm search.txt");
    return 1;
}
int check_glob_email_exist()
{
    DIR *dir = opendir("/home");
    system("touch search.txt");
    system("ls > search.txt");
    FILE *file = fopen("search.txt", "r");
    if (file == NULL)
    {
        perror("segmen check glob");
        return 1;
    }
    char search[185];
    while (fgets(search, sizeof(search), file) != NULL)
    {
        if (strcmp(search, "config.glob.email.txt") == 0)
        {
            fclose(file);
            system("rm search.txt");
            return 0;
        }
    }
    fclose(file);
    system("rm search.txt");
    return 1;
}

int config_global_username(char *username)
{
    // bayad set konm k harvagh ino  zad  bere  ro hame config proje ha
    // copy kone

    FILE *file = fopen("/home/config.glob.name.txt", "w");
    if (file == NULL)
    {
        perror("file not found1");
        return 1;
    }
    fprintf(file, "username: %s", username);
    fclose(file);

    return 0;
}
int config_global_email(char *email)
{
    FILE *file = fopen("/home/config.glob.email.txt", "w");
    if (file == NULL)
    {
        perror("file not found2");
        return 1;
    }
    fprintf(file, "email: %s", email);
    fclose(file);
    return 0;
}

int search_name(char *path)
{
    char location[80];
    char pathcopy[80];
    strcpy(pathcopy, path);
    strcat(pathcopy, "\n");
    strcpy(location, neeogit_location);
    strcat(location, "/.neeogit/stagearea/stagehistory.txt");
    FILE *Astagedfiles = fopen(location, "r");
    if (Astagedfiles == NULL)
    {
        perror("segmen fault13");
        return 1;
    }
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

int search_stagedfiles(int depth)
{
    int count = 0;
    system("touch file.txt");
    system("ls -a > file.txt");
    FILE *files = fopen("file.txt", "r");
    if (files == NULL)
    {
        perror("segmen fault12");
        return 1;
    }
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
        if (pathfile == NULL)
        {
            perror("segmen fault11");
            return 1;
        }
        fgets(path, 90, pathfile);
        int length = strlen(path);
        path[length - 1] = '/';
        strcat(path, filenames[i]);
        fclose(pathfile);
        system("rm path.txt");
        // path is compoletely true;
        int type = check_dir(path);
        if (((((type == 1) && (strcmp(filenames[i], ".neeogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0)) && (depth > 1))
        {
            search_name(path);
            chdir(path);
            search_stagedfiles(depth - 1);
            chdir("..");
        }
        else if (((((type == 1) && (strcmp(filenames[i], ".neeogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0)) && (depth == 1))
        {
            search_name(path);
        }
        else if (((strcmp(filenames[i], ".neeogit") != 0)) && (type == 0))
        {
            search_name(path);
        }
    }
}

int check_nee_root() // ino check  konm fk konnm eshtebahe
{
    int test = 0;
    int flag = 0;
    while (1)
    {
        // alaki
        if (check_neeogit_exist() == 1)
        {
            flag++;
            break;
        }
        else if (check_root() == 1)
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

int check_exist(char path[], char stagehis[])
{
    char search[1800];
    strcpy(search, path);
    strcat(search, "\n");
    FILE *file = fopen(stagehis, "r");
    if (file == NULL)
    {
        perror("segmen fault10");
        return 1;
    }
    char pathes[1800];
    int flag = 0;
    while (fgets(pathes, 1800, file))
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
int check_dir(char *path)
{
    if (strstr(path, ".txt") != NULL)
        return 0;
    else
        return 1;
    chdir("..");
    struct stat filestat;
    if (stat(path, &filestat) == 0)
    {
        if (S_ISDIR(filestat.st_mode) == 0)
        {
            return 1;
        }
        return 0;
    }
    return 2;
}
int check_stagehistory()
{
    DIR *dir = opendir(".neeogit/stagearea");
    char search[85];
    system("touch search.txt");
    system("ls > search.txt");
    FILE *file = fopen("search.txt", "r");
    if (file == NULL)
    {
        perror("segmen fault9");
        return 1;
    }
    while (fgets(search, sizeof(search), file))
    {
        if (strcmp(search, "stagehistory.txt\n") == 0)
        {
            system("rm search.txt");
            fclose(file);
            return 1;
        }
    }
    system("rm search.txt");
    fclose(file);
    return 0;
}
int check_root()
{
    system("touch khastam.txt");
    system("pwd > khastam.txt");
    FILE *file = fopen("khastam.txt", "r");
    if (file == NULL)
    {
        perror("segmen fault8");
        return 1;
    }
    char search[185];
    fgets(search, sizeof(search), file);
    if (strcmp(search, "/\n") == 0)
    {
        system("rm khastam.txt");
        fclose(file);
        return 1;
    }
    else
    {
        system("rm khastam.txt");
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
    if (file == NULL)
    {
        perror("segmen fault7");
        return 1;
    }
    char check[185];
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
            if (location == NULL)
            {
                perror("segmen fault6");
                return 1;
            }
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
        system("mkdir .neeogit/stagearea");
        FILE *poin2 = fopen(".neeogit/config.email.txt", "w");
        FILE *poin3 = fopen(".neeogit/config.name.txt", "w");
        FILE *poin1 = fopen(".neeogit/stagearea/stagehistory.txt", "w");
        if (poin1 == NULL && poin2 == NULL && poin3 == NULL)
        {
            perror("segmen fault.");
            return 1;
        }
        fclose(poin1);
        fclose(poin2);
        fclose(poin3);
        // baraye sakht glob to home

        system("touch khastam.txt");
        system("pwd > khastam.txt");
        FILE *nowdir = fopen("khastam.txt", "r");
        if (nowdir == NULL)
        {
            perror("khastam erorr");
            return 1;
        }
        char alaki[180];
        fgets(alaki, sizeof(alaki), nowdir);
        fclose(nowdir);
        if (check_glob_name_exist() == 1)
        {
            system("rm khastam.txt");
            chdir("/home");
            FILE *poin4 = fopen("config.glob.name.txt", "w");
            fclose(poin4);
            if (poin4 == NULL)
            {
                perror("segmen fault..");
                return 1;
            }
            chdir(alaki);
        }
        // name
        else
        {
            FILE *file1 = fopen("/home/config.glob.name.txt", "r");
            if (file1 == NULL)
            {
                perror("segmen4");
                return 1;
            }
            char transfer[185];
            char adress[185];
            fgets(transfer, 185, file1);
            fclose(file1);
            strcpy(adress, neeogit_location);
            strcat(adress, "/config.name.txt");
            FILE *file2 = fopen(adress, "w");
            if (file2 == NULL)
            {
                perror("segme3");
                return 1;
            }
            fprintf(file2, "%s", transfer);
            fclose(file2);
        }
        if (check_glob_email_exist() == 1)
        {
            system("rm khastam1.txt");
            chdir("/home");
            FILE *poin5 = fopen("config.glob.email.txt", "w");
            fclose(poin5);
            if (poin5 == NULL)
            {
                perror("segmen fault..");
                return 1;
            }
            chdir(alaki);
        }
        else
        {
            FILE *file1 = fopen("/home/config.glob.email.txt", "r");
            if (file1 == NULL)
            {
                perror("segmen2");
                return 1;
            }
            char transfer[185];
            char adress[185];
            fgets(transfer, 185, file1);
            fclose(file1);
            strcpy(adress, neeogit_location);
            strcat(adress, "/config.email.txt");
            FILE *file2 = fopen(adress, "w");
            if (file2 == NULL)
            {
                perror("segme1");
                return 1;
            }
            fprintf(file2, "%s", transfer);
            fclose(file2);
        }
    }
    else
    {
        perror("neeogit repository has already initialized\n");
    }
    return 0;
}

int config_local_username(char *username)
{
    FILE *file = fopen(".neeogit/config.name.txt", "w");
    if (file == NULL)
    {
        perror("khata5");
        return 1;
    }
    fprintf(file, "username: %s\n", username);
    fclose(file);
    return 0;
}
int config_local_email(char *email)
{
    FILE *file = fopen(".neeogit/config.email.txt", "w");
    if (file == NULL)
    {
        perror("khata4");
        return 1;
    }
    fprintf(file, "email: %s\n", email);
    fclose(file);
    return 0;
}
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
        int type = check_dir(path);
        char Astage[80];
        strcpy(Astage, neeogit_location);
        strcat(Astage, "/.neeogit/stagearea/stagehistory.txt");
        if ((((type == 1) && (strcmp(filenames[i], ".neeogit") != 0)) && (strcmp(filenames[i], ".") != 0)) && (strcmp(filenames[i], "..") != 0))
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
        else if (((strcmp(filenames[i], ".neeogit") != 0)) && (type == 0))
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
int add(char *name_of_file, int add_mode)
{
    char path[1900];
    char stagehis[1800];
    system("touch path.txt");
    system("pwd > path.txt");
    // if zir azafeh
    if (check_stagehistory() == 0)
    {
        DIR *dir = opendir(".neeogit/stagearea");
        system("touch stagehistory.txt");
        closedir(dir);
    }
    system("touch path.txt");
    system("pwd > path.txt");
    FILE *pathfile = fopen("path.txt", "r");
    if (pathfile == NULL)
    {
        perror("segmen fault3");
        return 1;
    }
    fgets(path, sizeof(path), pathfile);
    fclose(pathfile);
    system("rm path.txt");
    int len = strlen(path);
    path[len - 1] = '\0';
    strcat(path, "/");
    strcat(path, name_of_file);
    strcpy(stagehis, neeogit_location);
    strcat(stagehis, "/.neeogit/stagearea/stagehistory.txt");
    // printf("%s\n", path);
    if (check_dir(path) == 1)
    {
        if (check_exist(path, stagehis) == 0)
        {
            FILE *add = fopen(stagehis, "a");
            if (add == NULL)
            {
                perror("segmen fault2");
                return 1;
            }
            fprintf(add, "%s", path);
            fprintf(add, "\n");
            fclose(add);
        }
        chdir(path);
        modify(add_mode);
        chdir("..");
    }
    else if (check_dir(path) == 0)
    {
        // printf("path : %s", path);
        // printf("stagehis :%s", stagehis);
        // if (check_exist(path, stagehis) == 0)
        // {
        FILE *add = fopen(stagehis, "a");
        if (add == NULL)
        {
            perror("segmen fault1");
            return 1;
        }
        fprintf(add, "%s", path);
        fprintf(add, "\n");
        fclose(add);
        modifyfile(name_of_file, add_mode);
        // }
    }
    else if (check_dir(path) == 2)
    {
        printf("there isn't any file or directory with this name\n");
        return 0;
    }
    return 1;
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
    // printf("file path :%s", path);
    if (stat(path, &filestat) == 0)
    {
        strcat(path, "\n");
        char search[70];
        char loc[80];
        strcpy(loc, neeogit_location);
        strcat(loc, "/.neeogit/stagearea"); // age khata dad benevisam stagedarea chon yedone d bardashtm
        char COMMAND[180];
        strcpy(COMMAND, "cp ");
        strcat(COMMAND, main_path);
        strcat(COMMAND, " ");
        strcat(COMMAND, loc);
        system(COMMAND);
        chdir(neeogit_location);
        chdir(location);
        return 1;
    }
    else
    {
        perror("Error");
        return -1;
    }
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
            else if (strcmp(argv[3], "user.email") == 0)
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
        if ((check_nee_root()) == 0)
        {
            printf("you didn't initialized neogit in your project\n");
            return 0;
        }
        if (strcmp(argv[2], "-f") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                add(argv[i], 0);
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
        else
        {
            add(argv[2], 0);
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
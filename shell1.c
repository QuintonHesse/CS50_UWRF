#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

// print a start up message
void printWelcome()
{
    printf("******************************************\n"); // 42 stars
    printf("*                                        *\n");
    printf("*      ****Welcome to my shell!****      *\n");
    printf("*                                        *\n");
    printf("******************************************\n");
}

void printPrompt()
{
    char buffer[1000];
    getcwd(buffer, 1000);
    printf("%s$ ", buffer);
    fflush(stdout);
}

void printHelp()
{
    printf("\n*** SHELL FEATURES ***"
           "\nList of Built-Ins:"
           "\n> exit <optional exit code>"
           "\n> cd <directory>"
           "\n> pwd"
           "\n> help"
           "\n\nYou can press Ctrl-C to terminate this shell.\n");
}

void handleExit(char *arg)
{
    if (arg == NULL)
    {
        printf("Goodbye!\n");
        exit(0);
    }

    char *endptr = NULL;
    long code = strtol(arg, &endptr, 10);

    if (*endptr != '\0')
    {
        fprintf(stderr, "Exit code must be a number. You entered: %s\n", arg);
        return;
    }

    if (code < 0 || code > 255)
    {
        fprintf(stderr, "Exit code must be a number from 0 to 255! You entered: %ld\n", code);
        return;
    }

    printf("Goodbye!\n");
    exit(code);

}

void handleCd(char *arg)
{
    if (arg == NULL)
    {
        int err = chdir(getenv("HOME"));
        if (err == -1)
        {
            fprintf(stderr, "cd failed: No such file or directory\n");
        }
        return;
    }

    int err = chdir(arg);
    if (err == -1)
    {
        fprintf(stderr, "cd failed: No such file or directory\n");
    }

}

int handleBuiltIns(char *cmd, char *arg)
{
    if (strcmp(cmd, "exit") == 0)
    {
        handleExit(arg);
        return 1;
    }
    else if (strcmp(cmd, "help") == 0)
    {
        printHelp();
        return 1;
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        char buffer[1000];
        getcwd(buffer, 1000);
        printf("%s\n", buffer);
        return 1;
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        handleCd(arg);
        return 1;
    }

    return 0;
}

void call(char *argv[])  // { "echo", "hello", NULL}
{
    if (handleBuiltIns(argv[0], argv[1]))
    {
        return;
    }

    pid_t pid = 0;
    int status;

    pid = fork();

    if (pid == 0) // in child
    {
        int ret = execvp(argv[0], argv);
        if (ret == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    if (pid > 0) // in parent
    {
        pid = wait(&status);
    }
    if (pid < 0)
    {
        perror("fork");
    }
    return;
}
void removeTrailingSpaces(char *str) {
    int i = strlen(str) - 1;
    while (str[i] == ' ') i--;
    str[i+1] = '\0';
}
char* getFilenamesReturnCommand(char *buffer, char *infile, char *outfile) {
    char* ptr = buffer;
    infile[0] = '\0';  // start with empty string infile and outfile
    outfile[0] = '\0';

    while(*ptr != '<' && *ptr != '>' && *ptr != '\0') ptr++;  // look for delimiter

    if (*ptr == '<') {  // if infile is present
        *(ptr++) = '\0';
        while(*ptr == ' ') ptr++;
        while(*ptr != '>' && *ptr != '\0') *(infile++) = *(ptr++);
        *infile = '\0';
        removeTrailingSpaces(infile);
    }


    if (*ptr == '>') {  // if outfile is present
        *(ptr++) = '\0';
        while(*ptr == ' ') ptr++;
        while(*ptr != '\0') *(outfile++) = *(ptr++);
        *outfile = '\0';
        removeTrailingSpaces(outfile);
    }

    removeTrailingSpaces(buffer);
    return buffer;  // at this point, buffer contains the command string

}


int main()
{
    char buffer[1000];

    printWelcome();

    int stdin_bak = dup(STDIN_FILENO);
    int stdout_bak = dup(STDOUT_FILENO);
    int pipefd[2];


    while (1)
    {
        printPrompt();

        fgets(buffer, 1000, stdin);

        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
        char infile[100];
        char outfile[100];



        char *cmds[1000];
        int i = 0;


        cmds[i] = strtok(buffer, "|");
        while(cmds[++i] = strtok(NULL, "|"));

        char *cmds1[1000];
        int h = 0;
        cmds1[h] = strtok(buffer, "<>");
        while(cmds1[++h] = strtok(NULL, "<>"));


        for (int q = 0; cmds[q] != NULL; q++)
        {
            //printf("Command %d: %s\n", q, cmds[q]);
            char *argv[1000];
            int j = 0;
            argv[j] = strtok(cmds[q], " \n");
            while(argv[++j] = strtok(NULL, " \n"));
            //printf("Command %d: %s\n", q, cmds[q]);
            //for (int r = 0; argv[r] != NULL; r++)
           // {
                //printf("  Arg %d: %s\n", r, argv[r]);
           // }
            char* cmd2 = getFilenamesReturnCommand(buffer, infile, outfile);
            printf("cmd: \"%s\", in: \"%s\", out: \"%s\"\n", cmd2, infile, outfile);
            /*for (int k = 0; cmd2[k] != NULL; k++)
        {
            //printf("Cmds1 %d: %s\n", k, cmds1[k]);
            char* argv1[1000];
            int w = 0;
            argv1[w] = strtok(cmd2[k], " \n");
            while(argv1[++w] = strtok(NULL, " \n"));
            */


            if (infile[0] != '\0')
            {
                int in_fd = open(infile, O_RDONLY);
                if (in_fd < 0)
                {
                    perror("open infile");
                    continue;
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
                call(argv);
                dup2(stdin_bak, STDIN_FILENO);
            }
            if (outfile[0] != '\0')
            {
                int out_fd = open(outfile, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                if (out_fd < 0)
                {
                    perror("open outfile");
                    continue;
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
                call(argv);
                dup2(stdout_bak, STDOUT_FILENO);
            }
        

            if (cmds[q+1] == NULL)
            {

                dup2(stdout_bak, STDOUT_FILENO);
                call(argv);
                dup2(stdin_bak, STDIN_FILENO);

            }
            else{
                if (pipe(pipefd) == -1)
                {
                    perror("pipe");
                    continue;
                }
                dup2(pipefd[1], STDOUT_FILENO);
                call(argv);
                dup2(pipefd[0], STDIN_FILENO);

                close(pipefd[0]);
                close(pipefd[1]);

            }


            //call(argv);
        }


    
    close(stdin_bak);
    close(stdout_bak);
    }

    return 0;
}


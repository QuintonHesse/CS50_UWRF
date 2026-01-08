#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h> //found on stack overflow for isdigit function
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX 1000

#define comMax 10

char* getFilenamesReturnCommand(char* buffer, char *infile, char *outfile);

void welcome();
void getInput(char *userIn);
void getDirect();
void changeDirect(char *dir);
void getpwd();
void printHelp();//intitialize prototypes
int parsePipe(char* str, char** strHasPipe);
bool hasPipe = false;

int main() {
    welcome();
    while (1) {
        getDirect();
        fflush(stdout);
        char userIn[1000];//initialize variables
        getInput(userIn);
        userIn[strcspn(userIn, "\n")] = '\0'; // remove newline


        char* commands[comMax + 1];//https://www.geeksforgeeks.org/c/array-of-pointers-in-c/
        int comCount = 0;
        char infile[100];
        char outfile[100];
        bool hasFileRedir = false;





        //printf("First token: %s\n", token1); //gets input tokens
        //it was tough understanding how to get input and and have enough storage, array was only solution
        //tokenized while loop was interesting and necessary to learn 3 links from stack overflow
        //https://stackoverflow.com/questions/67994268/how-do-i-strtok-through-a-str-and-assign-each-token-to-an-array-that-is-a-global
         if (strchr(userIn, '|') != NULL) {
            char* token = strtok(userIn, "|");
            hasPipe = true;
            //printf("Pipe detected in input.\n");
             int comCountP = 0;
            while (token != NULL && comCount < comMax) {
                commands[comCount++] = token;
                token = strtok(NULL, "|");
            }
        } else {
            char* token = strtok(userIn, " ");
            hasPipe = false;
            //printf("No pipe detected in input.\n");
            while (token != NULL && comCount < comMax) {
                commands[comCount++] = token;
                token = strtok(NULL, " ");
            }
        }
        if (strchr(userIn, '<') != NULL || strchr(userIn, '>') != NULL) {

            //getFilenamesReturnCommand(userIn, infile, outfile);
            //printf("cmd: \"%s\", infile: \"%s\", outfile: \"%s\"\n", userIn, infile, outfile);
            hasFileRedir = true;

        }




        commands[comCount] = NULL;//null terminate the array

        if (comCount == 0) {
            continue; // no command entered
        }

        char* tokenCom = commands[0];

        if (strcmp(tokenCom, "exit") == 0) {
            long exitCode = 0; //stores token after current command if valid
            if (comCount > 1 && commands[1] != NULL) {//if both are not null
                char *endptr = NULL;
                exitCode = strtol(commands[1], &endptr, 10);//https://www.tutorialspoint.com/c_standard_library/c_function_strtol.html
                if (endptr == commands[1] || *endptr != '\0') { //https://www.reddit.com/r/C_Programming/comments/1il2zo1/what_is_the_proper_and_safe_way_to_use_strtol/
                    fprintf(stderr, "Exit code must be a number. You entered: %s\n", commands[1]);
                    continue;
                }
                if (exitCode < 0 || exitCode > 255) {
                    fprintf(stderr, "Exit code must be a number from 0 to 255! You entered: %ld\n", exitCode);
                    continue;
                }
            }
            printf("Goodbye!\n");
            exit((int)exitCode);
        }

        if (strcmp(tokenCom, "help") == 0) { //string compare function calls
            printHelp();
            continue;
        }

        if (strcmp(tokenCom, "pwd") == 0) {
            getpwd();
            continue;
        }

        if (strcmp(tokenCom, "cd") == 0) {
            if (comCount > 1) { //handles cd lack of input
                changeDirect(commands[1]); // passes argument
            } else {
                changeDirect(NULL); //passes null
            }
            continue;
        }



        if (hasPipe) { //pipe logic from https://www.geeksforgeeks.org/c/making-linux-shell-c/
            //printf("Pipe command detected.\n");
        if (comCount < 2) {
            fprintf(stderr, "Invalid pipe command.\n");
            continue;
        }
            int pipefd[2];
            pid_t p1, p2;
            if (pipe(pipefd) == -1) {
                fprintf(stderr, "Pipe Failed\n");
                continue;
            }
            char cmd1Copy[100]; //make copies of commands to tokenize again
            strncpy(cmd1Copy, commands[0], sizeof(cmd1Copy) - 1);//string copy to avoid modifying original
            char* cmd1Args[comMax + 1];
            int cmd1Count = 0;
            char* token1 = strtok(cmd1Copy, " ");//tokenize first command
            while (token1 != NULL && cmd1Count < comMax) {
                cmd1Args[cmd1Count++] = token1;
                token1 = strtok(NULL, " ");
            }
            cmd1Args[cmd1Count] = NULL;

            char cmd2Copy[100];//second command copy
            strncpy(cmd2Copy, commands[1], sizeof(cmd2Copy) - 1);
            char* cmd2Args[comMax + 1];
            int cmd2Count = 0;
            char* token2 = strtok(cmd2Copy, " ");
            while (token2 != NULL && cmd2Count < comMax) {
                cmd2Args[cmd2Count++] = token2;
                token2 = strtok(NULL, " ");
            }
            cmd2Args[cmd2Count] = NULL;


            p1 = fork();
            if (p1 < 0) {
                fprintf(stderr, "Fork Failed\n");
                continue;
        }
            if (p1 == 0) { //child 1
                close(pipefd[0]); //close read end
                dup2(pipefd[1], STDOUT_FILENO); //redirect stdout to write end of pipe
                close(pipefd[1]);
                if (execvp(cmd1Args[0], cmd1Args) < 0) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } else { //parent process
                p2 = fork();
                if (p2 < 0) {
                    fprintf(stderr, "Fork Failed\n");
                    return 1;
                }
                if (p2 == 0) { //child 2
                    close(pipefd[1]); //close write end
                    dup2(pipefd[0], STDIN_FILENO); //redirect stdin to read end of pipe
                    close(pipefd[0]);
                    if (execvp(cmd2Args[0], cmd2Args) < 0) {
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }
                } else { //parent process
                    close(pipefd[0]);
                    close(pipefd[1]);
                    waitpid(p1, NULL, 0);
                    waitpid(p2, NULL, 0);
                }
            }
            continue;
        }
        if (hasFileRedir) {
            getFilenamesReturnCommand(userIn, infile, outfile);
            int stdoutCopy = dup(STDOUT_FILENO);
            int stdinCopy = dup(STDIN_FILENO);
            int inputfd = open(infile, O_RDONLY);
            int outputfd = open(outfile, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            dup2(inputfd, STDIN_FILENO);
            dup2(outputfd, STDOUT_FILENO);
            close(inputfd);
            close(outputfd);
            // After command execution, restore original stdin and stdout
            dup2(stdinCopy, STDIN_FILENO);
            dup2(stdoutCopy, STDOUT_FILENO);
            close(stdinCopy);
            close(stdoutCopy);






        }



        pid_t pid = 0;
        int status = 0;
        //printf("Executing command: %s\n", tokenCom);
        pid = fork(); //forks process
        if (pid < 0) {
            fprintf(stderr, "Fork Failed\n");
            continue;
        }
        if (pid == 0) { //child process
            execvp(commands[0], commands); //executes command
                perror("execvp"); //if execvp fails
                exit(EXIT_FAILURE); //exit child process


            }else { //parent process
            pid = wait(&status); //wait for child process to finish
        }
    }






       //fprintf(stderr, "Command, %s, not recognized!\n", tokenCom);


    return 0;
}


void welcome(){ //prints welcome message
    printf("******************************************\n");
    printf("*                                        *\n");
    printf("*      ****Welcome to my shell!****      *\n");
    printf("*                                        *\n");
    printf("******************************************\n");
}

void getInput(char *userIn){ //gets user input
    fgets(userIn, 1000, stdin);
    }


void getDirect(){ //gets current directory
    char *currd = getcwd(NULL, 0);
    printf("%s$ ", currd);

}
void getpwd(){ //gets current directory
    char *currd = getcwd(NULL, 0);
    size_t dollar = strcspn(currd, "$");
    currd[dollar] = '\0';
    printf("%s\n", currd);//https://www.geeksforgeeks.org/c/strcspn-in-c/
}
void changeDirect(char *dir){ //changes directory
    char *targDir = dir;//target driectory
    int dirRes = -1;//directory result
    if (dir == NULL){//if no arg
        targDir = getenv("HOME"); //class pages

    }
    dirRes = chdir(targDir); //https://www.geeksforgeeks.org/linux-unix/chdir-in-c-language-with-examples/
    if (dirRes != 0) {//error
        fprintf (stderr,"cd failed: No such file or directory\n");
    }
}
void printHelp() //prints help information
{
    printf("\n*** SHELL FEATURES ***"
           "\nList of Built-Ins:"
           "\n> exit <optional exit code>"
           "\n> cd <directory>"
           "\n> pwd"
           "\n> help"
           "\n\nYou can press Ctrl-C to terminate this shell.\n");
}
void removeTrailingSpaces(char* str) {
    int i = strlen(str) - 1;
    while (str[i] == ' ') i--;
    str[i + 1] = '\0';
}
char* getFilenamesReturnCommand(char* buffer, char *infile, char *outfile) {
    char* pntr = buffer;
    infile[0] = '\0';
    outfile[0] = '\0';

    while(*pntr != '<' && *pntr != '>' && *pntr != '\0') {
        pntr++;
    }
    if (*pntr == '\0') {
        return NULL; // No redirection found
    }
    if (*pntr == '<') {
        *(pntr++) = '\0'; // Split the string
        while (*pntr == ' ') pntr++; // Skip spaces
        while (*pntr != '>' && *pntr != '\0') *(infile++) = *(pntr++); // Read infile
        *infile = '\0';// Null-terminate infile
        removeTrailingSpaces(infile);
    }
    if (*pntr == '>') {
        *(pntr++) = '\0'; // Split the string
        while (*pntr == ' ') pntr++; // Skip spaces
        while (*pntr != '<' && *pntr != '\0') *(outfile++) = *(pntr++); // Read outfile
        *outfile = '\0';
        removeTrailingSpaces(outfile);
    }
    removeTrailingSpaces(buffer);
    return buffer;
}

/*other helpful links
https://www.geeksforgeeks.org/c/making-linux-shell-c/

https://stackoverflow.com/questions/4788374/writing-a-basic-shell

https://medium.com/@santiagobedoa/coding-a-shell-using-c-1ea939f10e7e

https://app.codecrafters.io/courses/shell/overview */



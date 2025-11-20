#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void welcome();
void getInput();
void getDirect();

void main() {
welcome();
while(1){
    getDirect();
    char done[] = "exit\n";
    char userIn[1000];
    getInput(userIn);
    int result = strcmp (done, userIn);
    //printf("input received is %s, %d\n", userIn, result);
    if (result == 0) {
        printf("Goodbye!\n");
        return exit(0);
    }

}

}

void welcome(){
    printf("******************************************\n");
    printf("*                                        *\n");
    printf("*      ****Welcome to my shell!****      *\n");
    printf("*                                        *\n");
    printf("******************************************\n");
}

void getInput(char *userIn){
    fgets(userIn, 1000, stdin);
    printf("You typed: %s", userIn);
}
void getDirect(){
    char *currd = getcwd(NULL, 0);
    printf("%s$ ", currd);
}

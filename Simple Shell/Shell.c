#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h> //to get a username of linux terminal
void split(char *str,char *params[50]);
void execute(pid_t processID,int stat,char *params[50]);
void terminate(char *command);
void logfile(int signal);
void changeDirectory(char *path);
char pathFile[250]; //fixed Path to put logfile in it
FILE *file;
int main()
{
    char username[50];
    getlogin_r(username,sizeof(username));
    getcwd(pathFile,sizeof(pathFile));
    strcat(pathFile,"/logfile.txt");
    while(1){
        pid_t processID;
        int stat;
        char command[150];
        char *params[50];
        char currentWorkingDirectory[256];
        getcwd(currentWorkingDirectory,sizeof(currentWorkingDirectory));
        printf("[%s]@%s:",username,currentWorkingDirectory);
        gets(command);
        split(command,params);
        execute(processID,stat,params);
        signal(SIGCHLD,logfile);
    }
    return 0;
}
void split(char *str,char *params[50]){
    int rows=0;
    char *splittedCommand;
    char parameters[50][50];
    splittedCommand =strtok(str," ");
    while(splittedCommand != NULL)
    {
        strcpy(parameters[rows],splittedCommand);
        splittedCommand=strtok(NULL," ");
        rows++;
    }
    terminate(parameters[0]);
    for (int i=0;i<rows+1;i++){
		params[i]=parameters[i];
    }
    params[rows]= NULL;
}
void terminate(char *command){
    if(strcmp(command,"exit") == 0){
        fclose(logfile);
        exit(0);
	}
}
void execute(pid_t processID ,int stat,char *params[50]){
    if(strcmp(params[0],"cd") == 0){
        changeDirectory(params[1]);
        return;
    }
    processID = fork();
    if(processID >= 0){
        if(processID == 0){
            execvp(params[0],params);
        }else{
            wait(&stat);
        }
    }else{
        printf("Error Fork\n");
    }
}


void logfile(int signal)
{
    file=fopen(pathFile,"a");
    fprintf(file,"Child process has been succesfully terminated\n");
}
void changeDirectory(char *path){
   char currentWorkingDirectory[256];
   getcwd(currentWorkingDirectory,sizeof(currentWorkingDirectory));
   strcat(currentWorkingDirectory,"/");
   strcat(currentWorkingDirectory,path);
   chdir(currentWorkingDirectory);
  }

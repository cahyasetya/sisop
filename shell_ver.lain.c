#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void do_the_job(char **cmd, int bg){
	pid_t pid;
	int i;
	pid = fork();
	if(pid==0)
        execvp(cmd[0], cmd);
	else{
        if(bg==0) wait(0);
	}
}

void change_direction(char *cmd_run[]){
	if(cmd_run[1] != NULL) chdir(cmd_run[1]);
	else
	{
        char directoryofuser[10]="/home/";
        char *user=getlogin();
        strcat(directoryofuser, user);
        chdir(directoryofuser);
    }
}

void ignore(int signal){
	char *username, hostname[100], cwd[1024];
	gethostname(hostname,100);
        username = getlogin();
	getcwd(cwd, sizeof(cwd));

	fprintf(stderr,"\n\x1b[31m%s@%s \x1b[34m%s : \x1b[0m", username, hostname, cwd);
}

int main(){
	char *username, hostname[100], cwd[1024], sintaks[100000], *cmd_run[50], temp[100];
	int i, bg = 0, length;
	signal(SIGINT, ignore);
	signal(SIGTSTP, ignore);

	while(1){
		gethostname(hostname,100);
        	username = getlogin();
		getcwd(cwd, sizeof(cwd));

		printf("\x1b[31m%s@%s \x1b[34m%s : \x1b[0m", username, hostname, cwd);
        scanf("%[^\n]", sintaks);
        getchar();
        if(sintaks[0]==0) continue;
        else{
            if(sintaks[strlen(sintaks)-1]==38) bg=1;
            cmd_run[0] = strtok(sintaks, " &");

            for(i = 1; i < 100; i++)
                cmd_run[i] = strtok(NULL, " &");

            if(strcmp(cmd_run[0], "cd") == 0) change_direction(cmd_run);
            else if (strcmp(cmd_run[0],"exit") == 0) break;
            else do_the_job(cmd_run, bg);
            bg = 0;
        }
        sintaks[0]=0;
	}
	printf("\n");
}

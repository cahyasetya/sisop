#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void do_the_job(char **cmd, int bg){
	int p_pid, i;
	p_pid = fork();

	if(bg == 1){
		if(p_pid == 0){ 
			execvp(cmd[0], cmd);			
		}
	}
	else if(p_pid == 0) execvp(cmd[0], cmd);
	else {
		waitpid(p_pid, &i, 0);
	}
}

void change_direction(char *cmd_run[]){
	if(cmd_run[1] != NULL) chdir(cmd_run[1]);
	else chdir("..");
}

void ignore(int signal){
	char *username, hostname[100], cwd[1024];
	gethostname(hostname,100);
        username = getlogin();
	getcwd(cwd, sizeof(cwd));

	fprintf(stderr,"\n\x1b[31m%s@%s \x1b[34m%s &: \x1b[0m", username, hostname, cwd);
}

int main(){
	char *username, hostname[100], cwd[1024], cmd[1024], *cmd_run[50], temp[100];
	int i, bg = 0, length;
	signal(SIGINT, ignore);
	signal(SIGTSTP, ignore);

	while(1){
		gethostname(hostname,100);
        	username = getlogin();
		getcwd(cwd, sizeof(cwd));

		printf("\x1b[31m%s@%s \x1b[34m%s &: \x1b[0m", username, hostname, cwd);

		if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;
		if(cmd[strlen(cmd)-1] == '\n') cmd[strlen(cmd)-1] = '\0';

		cmd_run[0] = strtok(cmd, " ");
		if(cmd_run[0] == NULL) break;
		
		for(i = 1; i < 100; i++){
			cmd_run[i] = strtok(NULL, " ");
			if(cmd_run[i] == NULL){
				strcpy(temp, cmd_run[i-1]);
				length = strlen(temp);
				
				if(temp[length-1] == '&'){
					temp[length-1] = '\0';
					strcpy(cmd_run[i-1],temp);
					bg = 1;
				}
				break;
			}
		}

		if(strcmp(cmd_run[0], "cd") == 0) change_direction(cmd_run);
		else if (strcmp(cmd_run[0],"exit") == 0) break;
		else do_the_job(cmd_run, bg);
		bg = 0;
	}
	printf("\n");
}

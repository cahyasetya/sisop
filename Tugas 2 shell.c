//Segmentation fault program
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct job{
	//struct job *next;
	char **cmd;
	int status;
}job;

void search_command(char cmd[], job *j){
	int i;
	char *temp;
	j->cmd[0] = strtok(cmd, " ");
	if(j->cmd[i] == NULL) return;

	for(i = 1; i < 100; i++){
		j->cmd[i] = strtok(NULL, " ");
		printf("\n%s-",*j->cmd);
		if(j->cmd[i] == NULL) break;
	}
}

void background_job(){
}

/*void change_direction(job *j){
    chdir(j->cmd[1]);
}*/

void do_the_job(job *j){
	int p_pid, wstatus;
	pid_t s;

	p_pid = fork();
	if(p_pid == 0)
		execvp(j->cmd[0], j->cmd);
	else s = waitpid(p_pid, &wstatus, WNOHANG);
}

int main(){
	char *username, hostname[100], cwd[1024], cmd[250];
	job *j;
	signal(SIGINT, SIG_IGN);
	//SIGSTOP cannot be ignored, trapped, or blocked;

	while(1){
		gethostname(hostname,100);
        	username = getlogin();
		if (getcwd(cwd, sizeof(cwd)) != NULL); //cwd = strdup(direction(cwd));

		printf("%s@%s %s &: ", username, hostname, cwd);

		if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;
		if(cmd[strlen(cmd)-1] == '\n') cmd[strlen(cmd)-1] = '\0';
		j = malloc(sizeof(job)); // SIGSEV disini butuh pencerahan
		search_command(cmd, j);

		if(strcmp(j->cmd[0], "cd") == 0) change_direction(j);
		else if (strcmp(j->cmd[0],"exit") == 0) break;
		else do_the_job(j);
		free(j);
	}
	printf("\n");
}

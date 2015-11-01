#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

void sigint(int numb){}
void sigstop(int numb){}

int main()
{
    //signal(SIGINT, sigint);
    signal(SIGSTOP, sigstop);
    char input[100];
    char *arg[100];
    char* split;
    while(1){
        scanf("%[^\n]%*c", input);
        if(strcmp(input, "exit")==0) break;
        else{
            pid_t pid=fork();
            if(pid==0){
                split=strtok(input, " ");
                if(strcmp(split, "cd")==0)
                {
                    chdir(strtok(NULL, "\n"));
                }
                else{
                    int i=0;
                    while(split!=NULL)
                    {
                        arg[i]=split;
                        split=strtok(NULL, " ");
                        i++;
                    }
                    arg[i]=NULL;
                    execvp(arg[0], arg);
                }
            }
            else{
                wait(0);
            }
        }
    }
    return 0;
}

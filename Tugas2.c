#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>

typedef struct{
    char *input;
    int *counter;
} arguments;

void *cek(void *args)
{
    arguments *arg=args;
    int c=atoi(arg->input);
	int flag=0;
	int i;
    if(c<=1) flag=1;
	else if(c==2) flag=0;
	else{
		for(i=2; i<c; i++)
		{
			if(c%i==0) flag=1;
		}
	}
	if(flag==0)
        (*(arg->counter))++;
	free(arg);
}

int main(){
	char input[10];
	pthread_t t[10];
	printf("Masukkan input: ");
	scanf("%[^\n]", input);
	char *coba=strtok(input, " ");
	int i=1;
	int counter=0;
	while(coba!=NULL)
	{
        arguments *arg=malloc(sizeof(*arg));
        arg->input=coba;
        arg->counter=&counter;
        pthread_create(&t[i], NULL, cek, arg);
        coba=strtok(NULL, " ");
        i++;
    }
    printf("Terdapat %d bilangan prima\n", counter);
	return 0;
}

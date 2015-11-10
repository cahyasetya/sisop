#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>

typedef struct{
    int input;
    int *counter;
} arguments;

void *cek(void *args)
{
    arguments *arg=args;
	int flag=0;
	int i;
    if(arg->input<=1) return;
	else if (arg->input>2){
		for(i=2; i<arg->input; i++)
		{
			if(arg->input%i==0) return;
		}
	}
    (*(arg->counter))++;

	free(arg);
}

int main(){
	int input;
	pthread_t *t;
	printf("Masukkan n: ");
	scanf("%d", &input);
	t=malloc(sizeof(pthread_t)*input);
	int counter=0;
	int i;
	for(i=1; i<input; i++)
	{
        arguments *arg=malloc(sizeof(*arg));
        arg->input=i;
        arg->counter=&counter;
        pthread_create(&t[i-1], NULL, cek, arg);
    }
    for(i=0;i<input-2; i++)
    pthread_join(t[i], NULL);
    printf("Terdapat %d bilangan prima\n", counter);
    free(t);
	return 0;
}

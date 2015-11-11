#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int belumtulis=1;
int terminated=0;

typedef struct{
    int input;
    int *counter;
} arguments;

struct file{
	char in[25];
	char out[25];
	pthread_t *t;
};

void *cek(void *args){
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

void cari_prima_input(){
	int input;
	pthread_t *t;
	printf("\n\tMasukkan n:\n\t-");
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
    printf("\n\tTerdapat %d bilangan prima\n\n", counter);
    free(t);
}

void *thread2(void *arg){
    struct file *args=(struct file*)arg;
    FILE *in, *out;
    if(fopen(args->in, "r")){
        in = fopen(args->in, "r");
		out = fopen(args->out, "w");
    }
	while(belumtulis){
		if(terminated)return;
	}
    	if(strcmp(args->in, args->out) != 0){

		char c=fgetc(in);

		while(c != EOF) {
			fputc(c, out);
			c = fgetc(in);
		}
		fclose(in);
		fclose(out);
	}
}

void *thread1(void *filedesc){
	FILE *infile, *outfile;
	struct file *fdesc = (struct file*) filedesc;
	char temp;

	//use normal c function
	//File yang input dan outputnya sama langsung return.
	if(strcmp(fdesc->in, fdesc->out) != 0){
		outfile = fopen(fdesc->out, "w");
		infile = fopen(fdesc->in, "r");

		temp = fgetc(infile);
		belumtulis=0;
		while(temp != EOF) {
			fputc(temp, outfile);
			temp = fgetc(infile);
		};
		fclose(outfile);
		fclose(infile);
		terminated=1;
	}

	//use bash command
	//execl("/bin/cp", "/bin/cp", fdesc->in, fdesc->out, (char *)0);
}

void menu_membaca(){
	FILE *check;
	pthread_t t1, t2;
	char temp, temp2[25];

	printf("\n\tInputkan file yang akan disalin : \n\t-");
	scanf("%c", &temp);
	scanf("%[^\n]", temp2);

	if(fopen(temp2,"r") == NULL){
		printf("\n\tFile source yang disalin tidak ditemukan. Penyalinan gagal.\n");
	}else{
		struct file *file1 = malloc(sizeof(struct file));
		struct file *file2 = malloc(sizeof(struct file));

		strcpy(file1->in,temp2);
		printf("\n\tNama file Output ke-1 dari hasil menyalin : \n\t-");
		scanf("%c", &temp);
		scanf("%[^\n]", file1->out);
		printf("\n\tNama file Output ke-2 dari hasil menyalin : \n\t-");
		scanf("%c", &temp);
		scanf("%[^\n]", file2->out);
		strcpy(file2->in,file1->out);

		pthread_create(&t1, NULL, thread1, file1);
		pthread_create(&t2, NULL, thread2, file2);
		pthread_join(t2, NULL);
		free(file1);
		free(file2);
	}
	printf("\n");
}

int menu(){
	int pilih;
	printf("\t1) Mencari bilangan prima kurang dari N.\n");
	printf("\t2) Multithread untuk menyalin isi file.\n");
	printf("\t9) Exit.\n\n\t-");

	scanf("%d", &pilih);
	return pilih;
}

int main(){
	int n = 1;

	while(n != 9){
		n = menu();
		if(n == 1) cari_prima_input();
		else if(n == 2) menu_membaca();
	}
}

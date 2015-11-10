//Versi lain dari 'tugas 2.c'. Thread yang digunakan cuma 2. Code masih RAW dan belum simple.

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int input;
    int *counter;
} arguments;

struct file{
	char in[25];
	char out[25];
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
}

void *membaca_menulis(void *filedesc){
	FILE *infile, *outfile;
	struct file *fdesc = (struct file*) filedesc;

	//use normal c function
	char temp;
	infile = fopen(fdesc->in, "r");
	outfile = fopen(fdesc->out, "w");

	temp = fgetc(infile);

	while(temp != EOF) {
		fputc(temp, outfile);
		temp = fgetc(infile);
	};

	fclose(infile);
	fclose(outfile);

	//use bash command
	//execl("/bin/cp", "/bin/cp", fdesc->in, fdesc->out, (char *)0);
}

void menu_membaca(){
	struct file *file1 = malloc(sizeof(struct file));
	struct file *file2 = malloc(sizeof(struct file));
	pthread_t t1, t2;
	char temp;

	printf("\n\tInputkan file yang akan disalin : \n\t-");
	scanf("%c", &temp);
	scanf("%[^\n]", file1->in);
	printf("\n\tNama file Output ke-1 dari hasil menyalin : \n\t-");
	scanf("%c", &temp);
	scanf("%[^\n]", file1->out);
	printf("\n\tNama file Output ke-2 dari hasil menyalin : \n\t-");
        scanf("%c", &temp);
        scanf("%[^\n]", file2->out);
	strcpy(file2->in,file1->out);
	pthread_create(&t1, NULL, membaca_menulis, file1);
	pthread_create(&t2, NULL, membaca_menulis, file2);

	pthread_join(t1, NULL);
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

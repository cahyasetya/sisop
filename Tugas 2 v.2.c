//Versi lain dari 'tugas 2.c'. Thread yang digunakan cuma 2. Code masih RAW dan belum simple.

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct data{
	int angka;
	int jum;
};

struct file{
	char in[25];
	char out[25];
};

void *check_prima(void *pass){
	struct data *bil = (struct data*) pass;
	int n;

	if(bil->angka == 2){
		printf("%d ", bil->angka);
		bil->jum += 1;
	}else{
		for(n=2; n <= bil->angka/2; n++){
			if(bil->angka%n == 0) return;
		}
		printf("%d ", bil->angka);
                bil->jum += 1;
	}
	return;
}

void cari_prima_input(){
	pthread_t t1, t2;
	int input, i, jumlah;
	struct data *data1 = malloc(sizeof(struct data));
	struct data *data2 = malloc(sizeof(struct data));

	jumlah = 0;
	data1->jum=0;
	data2->jum=0;
	printf("\n\tMasukkan input :\n\t-");
	scanf("%d", &input);
	printf("\n");

	for(i = 2; i < input; i++){
		data1->angka = i;
		pthread_create(&t1, NULL, check_prima, data1);
		i = i+1;
		data2->angka = i;

		if(data2->angka <input)
			pthread_create(&t2, NULL, check_prima, data2);

		pthread_join(t1, NULL);
        	pthread_join(t2, NULL);
	}
	jumlah = data1->jum + data2->jum;
	printf("\n\nJumlah bilangan prima : %d.\n\n", jumlah);

	free(data1);
	free(data2);
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

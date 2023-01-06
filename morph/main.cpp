#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "image_processing.h"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short *resimdizi_org = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i*N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf("%d", &filter_size);

	switch (menu){
		case 1:
			Dilation(N*M, filter_size, resimdizi_org);
			resimYaz(dil_resim, resimdizi_org, N, M, Q);
			break;
		case 2:
			Erosion(N*M, filter_size, resimdizi_org);
			resimYaz(ero_resim, resimdizi_org, N, M, Q);
			break;
		case 3:
			system("EXIT");
			break;
		default:
			system("EXIT");
			break;
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* resim_org) {
	/*for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			resim_org[i][j] = 255;*/
	__asm {
		//KODUNUZU BURAYA YAZINIZ, ASAGIDAKI 5 ORNEK ERISIM ICIN VERILMISTIR SIZ YAZMAYA BASLAMADAN SILINIZ
		MOV EBX, resim_org
		MOV ECX, n
		L1:	MOV BYTE PTR[EBX], 0	;her bir piksel 1 word boyutunda(short oldugu icin)
		ADD EBX, 2
		LOOP L1
	}
	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {
	__asm {
		//KODUNUZU BURAYA YAZINIZ, ASAGIDAKI 5 ORNEK ERISIM ICIN VERILMISTIR SIZ YAZMAYA BASLAMADAN SILINIZ
		MOV EBX, resim_org
		MOV ECX, n
	L1: MOV WORD PTR[EBX], 255		;her bir piksel 1 word boyutunda (short oldugu icin)
		ADD EBX, 2
		LOOP L1
	}
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}

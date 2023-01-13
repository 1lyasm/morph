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
	/*
    * eax, ebx, ecx, edx, esi, edi
	* filter_size 16b
	* n 32 b
	* resim_org 32b
	* min: 8b
	* x, y, x', y', sqrt(n) 16b
	* h ebx: x, l ebx: y, h edx: x', l edx: y'
	* edi: sqrt(n), esi: min
	* eax, ecx: free
	* all else stack
    */
	__asm {
		int 3
		push 0
		; find length in one dimension
		mov eax, n
		xor edx, edx
		xor edi, edi
		mov dx, 1
		L1:	sub eax, edx
		inc edi
		cmp eax, 0
		jle L2
		add dx, 2
		jmp L1
	L2:
		; step 1
		mov eax, edi
		dec eax
		mov bx, ax
		shl ebx, 16
		mov bx, ax
	L8:	; step 2.1
		mov esi, 00ffh
		; step 2.2, 2.3
	    mov eax, filter_size
		shr eax, 1
		neg ax
	    mov dx, ax
		shl edx, 16
	L6:
	mov eax, filter_size
		shr eax, 1
		neg ax
		mov dx, ax
		L5 : ; step 2.4, 2.5
		mov ecx, edx
		shr ecx, 16
		mov ax, cx
		mov ecx, ebx
		shr ecx, 16
		add ax, cx
		cmp ax, 0
		jl L3
		mov eax, edi
		sub ax, cx
		mov ecx, edx
		shr ecx, 16
		cmp cx, ax
		jge L3
		; step 2.6, 2.7
		mov ax, bx
		add ax, dx
		cmp ax, 0
		jl L3
		mov eax, edi
		sub ax, bx
		cmp dx, ax
		jge L3
		; step 2.8
		xor eax, eax
		mov ax, bx
		add ax, dx
		mov ecx, edx
		xor edx, edx
		mul edi
		mov edx, ecx
		mov ecx, ebx
		shr ecx, 16
		add eax, ecx
		mov ecx, edx
		sar ecx, 16
		add eax, ecx
		shl eax, 1
		add eax, resim_org
		mov ecx, edx
		mov edx, eax
		mov eax, esi
		cmp byte ptr[edx], al
		jae L4
		; step 2.9
		mov al, byte ptr[edx]
		and eax, 000000ffh
		mov esi, eax
		L4 : mov edx, ecx
		L3 : ; step 2.10
		inc dx
		; step 2.11, 2.12
		mov eax, filter_size
		shr eax, 1
		mov ecx, filter_size
		inc ecx
		shr ecx, 1
		sbb eax, 0
		cmp ax, dx
		jge L5
		; 2.13
		rol edx, 16
		inc edx
		rol edx, 16
		; 2.14
		mov ecx, edx
		shr ecx, 16
		cmp ax, cx
		; 2.15
		jge L6

		; step 3
		sub ebp, 4
		mov ecx, dword ptr[ebp]
		cmp cl, 32
		je L11
		pop eax
		ror eax, cl
		mov ecx, esi
		mov al, cl
		mov ecx, dword ptr[ebp]
		rol eax, cl
		push eax
		add dword ptr[ebp], 8
		jmp L12
	L11:
		mov dword ptr [ebp], 0
		push 0
	L12:
		add ebp, 4

		; step 4
		test ebx, ebx
		jz L9
		mov ecx, ebx
		shr ecx, 16
		cmp cx, 0
		jbe L7
		rol ebx, 16
		dec bx
		rol ebx, 16
		jmp L8
	L7:
		mov eax, edi
		dec eax
		rol ebx, 16
		mov bx, ax
		rol ebx, 16
		dec bx
		jmp L8
	L9:
		
	; pull found minimums
	; from stack and write to array
		mov ecx, n
		shr ecx, 2
		mov edx, resim_org
		int 3
	L10:pop eax
		mov byte ptr [edx + 6], al
		shr eax, 8
		mov byte ptr [edx + 4], al
		shr eax, 8
		mov byte ptr[edx + 2], al
		shr eax, 8
		mov byte ptr[edx], al
		add edx, 8
		loop L10

		mov esp, ebp
		sub esp, 12
	}
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}


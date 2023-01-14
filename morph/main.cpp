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
	__asm {
		; exactly same as erosion, except that maximum (bl) is set to 0 first, 
		; and comparison is done to check if the value is more than maximum

		mov eax, n
		xor edx, edx
		xor edi, edi
		mov dx, 1
		L1 : sub eax, edx
		inc edi
		cmp eax, 0
		jle L2
		add dx, 2
		jmp L1
		L2 :
			mov esi, resim_org
			L11 :
			mov eax, esi
			sub eax, resim_org
			shr eax, 1
			xor edx, edx
			div edi
			ror edx, 16
			mov dx, ax
			rol edx, 16

			; set initial value of max
			xor bl, bl

			mov eax, filter_size
			shr eax, 1
			neg eax
			mov cl, al
			L7 :
		mov eax, edx
			shr eax, 16
			mov bh, ch
			and ecx, 00ffh
			xchg eax, ecx
			cbw
			cwde
			add ecx, eax
			xchg eax, ecx
			mov ch, bh
			cmp eax, 0
			jl L8
			cmp eax, edi
			jge L9
			mov eax, filter_size
			shr eax, 1
			neg eax
			mov ch, al
			L6 :
		xor eax, eax
			mov ax, dx
			mov bh, cl
			shr cx, 8
			and ecx, 00ffh
			xchg eax, ecx
			cbw
			cwde
			add ecx, eax
			xchg eax, ecx
			shl cx, 8
			mov cl, bh
			cmp eax, 0
			jl L10
			cmp eax, edi
			jge L8
			mov eax, edi
			mov bh, ch
			ror ebx, 16
			mov bx, dx
			and cx, 00ffh
			xchg ax, cx
			cbw
			xchg ax, cx
			imul cx
			ror eax, 16
			mov ax, dx
			rol eax, 16
			mov dx, bx
			rol ebx, 16
			mov ch, bh
			shl eax, 1
			add eax, esi
			mov bh, cl
			ror cx, 8
			and cx, 00ffh
			xchg eax, ecx
			cbw
			cwde
			add ecx, eax
			add ecx, eax
			xchg eax, ecx
			rol cx, 8
			mov cl, bh
			xchg eax, ebx
			cmp byte ptr[ebx], al
			xchg eax, ebx

			; this time, pass if below or equal
			jbe L10

			mov ebx, eax
			mov bl, byte ptr[ebx]
			L10 :
		inc ch
			mov eax, filter_size
			shr eax, 1
			cmp ch, al
			jle L6
			L8 :
		inc cl
			mov eax, filter_size
			shr eax, 1
			cmp cl, al
			jle L7
			L9 :
			cmp esi, resim_org
			jne L12
			push 0
			L12:
			pop edx
			mov eax, esi
			sub eax, resim_org
			cmp eax, 0
			jz L13
			shr eax, 2
			jc L13
			shr eax, 1
			jc L13
			push edx
			and ebx, 00ffh
			push ebx
			jmp L14
			L13 :
			mov eax, esi
			sub eax, resim_org
			mov cl, al
			and cl, 7
			shl cl, 2
			ror edx, cl
			and bx, 00ffh
			mov dl, bl
			rol edx, cl
			push edx
			L14 :
			add esi, 2
			mov eax, esi
			sub eax, resim_org
			shr eax, 1
			cmp eax, n
			je L5
			jmp L11
			L5 :
			mov eax, n
			dec eax
			shl eax, 1
			add eax, resim_org
			mov esi, eax
			L4 :
			pop eax
			mov byte ptr[esi - 6], al
			shr eax, 8
			mov byte ptr[esi - 4], al
			shr eax, 8
			mov byte ptr[esi - 2], al
			shr eax, 8
			mov byte ptr[esi], al
			sub esi, 8
			cmp esi, resim_org
			jl L3
			jmp L4
			L3 :
	}
	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {
	__asm {
	; find length in one dimension
	; edi: sqrt(n)
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
	; set index, esi: pixel index
		mov esi, resim_org
	
	L11:
	; put minimum value to bl

	; extract row and column indexes
		mov eax, esi
		sub eax, resim_org
		shr eax, 1
		xor edx, edx
		div edi

	; edx high: row index, edx low: column index
		ror edx, 16
		mov dx, ax
		rol edx, 16

	; make bl temporary minimum
		mov bl, 0ffh

	; find -filter_size / 2
		mov eax, filter_size
		shr eax, 1
		neg eax

	; for cl: ((-filter_size) / 2) to (filter_size / 2) (row)
		mov cl, al
	; check if current row + cl is in bounds
	L7:	
		mov eax, edx
		shr eax, 16
		mov bh, ch
		and ecx, 00ffh
		xchg eax, ecx
		cbw
		cwde
		add ecx, eax
		xchg eax, ecx
		mov ch, bh
		cmp eax, 0
		jl L8
		cmp eax, edi
		jge L9
		; for ch: ((-filter_size) / 2) to (filter_size / 2) (col)
			mov eax, filter_size
			shr eax, 1
			neg eax
			mov ch, al
			; check if current col + ch is in bounds
	L6:			
				xor eax, eax
				mov ax, dx
				mov bh, cl
				shr cx, 8
				and ecx, 00ffh
				xchg eax, ecx
				cbw
				cwde
				add ecx, eax
				xchg eax, ecx
				shl cx, 8
				mov cl, bh
				cmp eax, 0
				jl L10
				cmp eax, edi
				jge L8
		    ; if yes, compute the offsetted index
			; esi + 2 * cl * edi + 2 * ch
				; eax = 2 * cl * edi
					mov eax, edi
					mov bh, ch
					ror ebx, 16
					mov bx, dx
					and cx, 00ffh
					xchg ax, cx
					cbw
					xchg ax, cx
					imul cx
					ror eax, 16
					mov ax, dx
					rol eax, 16
					mov dx, bx
					rol ebx, 16
					mov ch, bh
					shl eax, 1

				; eax = eax + esi
					add eax, esi

				; eax = eax + 2 * ch
					mov bh, cl
					ror cx, 8
					and cx, 00ffh
					xchg eax, ecx
					cbw
					cwde
					add ecx, eax
					add ecx, eax
					xchg eax, ecx
					rol cx, 8
					mov cl, bh

			; compare the value in that index to bl
				; cmp byte ptr [eax], bl
				xchg eax, ebx
				cmp byte ptr[ebx], al
				xchg eax, ebx

			; if not smaller, continue
				jae L10

			; if smaller, update bl
				mov ebx, eax
				mov bl, byte ptr [ebx]

			; increment counters and loop back

				; increment and check inner counter ch
		L10:			
					inc ch
					mov eax, filter_size
					shr eax, 1
					cmp ch, al
					jle L6

				; increment and check outer counter cl
	L8:				
					inc cl
					mov eax, filter_size
					shr eax, 1
					cmp cl, al
					jle L7

	L9:

	; push minimum value
		; if first time, push zero
			cmp esi, resim_org
			jne L12
			push 0

	L12:
		; edx = last pushed
			pop edx

		; check whether a byte is empty
			mov eax, esi
			sub eax, resim_org
			cmp eax, 0
			jz L13
			shr eax, 2
			jc L13
			shr eax, 1
			jc L13

		; if no empty place, push back previous, and push new
			push edx
			and ebx, 00ffh
			push ebx
			jmp L14

	L13:
		; find which byte to fill
			mov eax, esi
			sub eax, resim_org
			
		; find remainder when divided by 8
			mov cl, al
			and cl, 7
			shl cl, 2

		; fill and push back
			ror edx, cl
			and bx, 00ffh
			mov dl, bl
			rol edx, cl
			push edx
	  
	L14:
	; go to next element
	    add esi, 2

	; exit if end of array
		mov eax, esi
		sub eax, resim_org
		shr eax, 1
		cmp eax, n
		je L5

	; go to next element
		jmp L11

	L5: 
	; start from last element of picture
		mov eax, n
		dec eax
		shl eax, 1
		add eax, resim_org
		mov esi, eax

	L4:
	; pop from stack
		pop eax
	
	; write popped value as value of current element
		mov byte ptr [esi - 6], al
		shr eax, 8
		mov byte ptr[esi - 4], al
		shr eax, 8
		mov byte ptr[esi - 2], al
		shr eax, 8
		mov byte ptr[esi], al

	; got four minimums, go back four elements
		sub esi, 8

	; exit loop if first pixel is written
		cmp esi, resim_org
		jl L3

	; continue with one element before
		jmp L4

	L3:
	}
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}

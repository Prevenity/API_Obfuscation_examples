#include "stdafx.h"
#include <Windows.h>

typedef UINT(CALLBACK*FUNKCJA)(HWND, LPCSTR, LPCSTR, UINT);

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE dllhandle = NULL;
	FUNKCJA funkcja1;
	dllhandle = LoadLibraryA("User32.dll");	//tutaj jeszcze ³adujemy user32 ale powinniœmy zastosowaæ poni¿szy kod do wywo³ania LoadLibraryA()
	//funkcja1 = (FUNKCJA) GetProcAddress(dllhandle,"MessageBoxA"); //to ju¿ nie jest nam potrzebne.
	char *title = "Przyklad 02";
	char *body = "Prevenity 2016";
	__asm
	{
			pushad
			xor ecx, ecx
			mov ecx, dword ptr fs : [30h] //&PEB
			mov ecx, dword ptr[ecx + 0Ch] //PEB->Ldr
			mov ecx, dword ptr[ecx + 1Ch] //PEB->LDR_DATA.ORDER
		next_dll:
			mov ecx, dword ptr[ecx]  //next module
			mov eax, dword ptr[ecx + 20h] //module name
			cmp dword ptr[eax + 08h], 320033h    //szukamy user32.dll (unicode)
			jne next_dll
			cmp dword ptr[eax], 00730055h		//j.w.
			jne next_dll
			mov ecx, dword ptr[ecx + 8] //base address of dll to jest adres nag³ówka pliku (MZ).
		find_function:
			mov ebp, ecx		//edi to base address
			mov eax, dword ptr[ecx + 3Ch] //offset dos header
			mov edx, dword ptr[ecx + eax + 78h]  //RVA of EAT
			add edx, ecx //do RVA dodajemy base address
			mov ecx, dword ptr[edx + 18h] //liczba funkcji eksportowanych przez bibliotekê
			mov eax, dword ptr[edx + 20h] //tabela z nazwami EAT
			add eax, ebp //adres tabeli
			mov ebx, eax //j.w.

			//xor ecx, ecx  //zeruj ecx
			//mov ecx, 21Ch //gdy chcemy sprawdziæ konkretn¹ wartoœæ
		find_function_loop :
		    jecxz find_function_finished
			dec ecx
			mov esi, [ebx + ecx * 4]
			add esi, ebp		//w esi jest wskaŸnik do nazwy funkcji
		compute_hash_init :
			xor edi, edi
			xor eax, eax
			cld
		compute_hash :
			lodsb
			test al, al
			jz compute_hash_finished
			ror edi, 0xd
			add edi, eax
			jmp compute_hash
		compute_hash_finished :
		find_function_compare :
			cmp edi, 0xBC4DA2A8		//porównanie hash-y
			jne find_function_loop
			mov ebx, [edx + 0x24]	//pobranie ordinal address table
			add ebx, ebp
			mov cx, [ebx + 2 * ecx]
			mov ebx, [edx + 0x1c]
			add ebx, ebp;
		    mov eax, [ebx + 4 * ecx];  //MessageBoxA function offset
			add eax, ebp			   //adres funkcji MessageBoxA
			mov[esp + 0x1c], eax
		find_function_finished :
			popad
			pushad
			push 0
			push title
			push body
			push 0

			push offset label1

			mov edi, edi
			push ebp
			mov ebp, esp
			lea eax, [eax + 5]
			jmp eax
		label1:
			popad
	}

	return 0;
}


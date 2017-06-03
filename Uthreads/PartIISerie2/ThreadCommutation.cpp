
#include "stdafx.h"

DWORD comutInit, comutEnd;


DWORD WINAPI Thread2(LPVOID arg) {
	
	comutEnd = GetTickCount();
	printf("THREAD B \n ");
	return 0;
}

DWORD WINAPI Thread1(LPVOID arg){
	DWORD idT2;
	HANDLE h2 = CreateThread(NULL, 0, Thread2, NULL, CREATE_SUSPENDED, &idT2);
	SetThreadAffinityMask(h2, 0x01);
	printf("THREAD A \n");
	comutInit = GetTickCount();

	if (SwitchToThread()) printf("Qualquer merda");
	return 0;
}



VOID ComutationTime() {
	DWORD idT1;
	unsigned int iter = 10;
	DWORD tot=0, avgComutTime;
	while (iter >= 0) {
		HANDLE h1 = CreateThread(NULL, 0, Thread1, NULL, 0, &idT1);
		SetThreadAffinityMask(h1, 0x01);
		tot += comutEnd-comutInit;
		printf("Total increment: %ld \n", tot);
		--iter;
	}
	avgComutTime = tot / iter;
	printf("Comutation average time: %ld \n", avgComutTime);
}

int main()
{
	ComutationTime();
	return 0;
}
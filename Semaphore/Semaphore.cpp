// Semaphore.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>

#define MAX_SEM_COUNT 4

int main()
{
	_tsetlocale(0, TEXT(""));
	HANDLE hSemaphore;
	int count = MAX_SEM_COUNT;
	hSemaphore = CreateSemaphore(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, TEXT("Sem"));
	if (hSemaphore == NULL) _tprintf(TEXT("Ошибка создания семафора: %d\n"), GetLastError());
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (!WaitForSingleObject(hSemaphore, 1000)) {
			ReleaseSemaphore(hSemaphore, 1, (LPLONG)&count);
			hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("Sem"));
			_tprintf(TEXT("Это %d экземпляр процесса!\n"), 4 - count);
		}
		else return 0;
	}
	else 
	{
		_tprintf(TEXT("Это 1 экземпляр процесса!\n"));
	}
	WaitForSingleObject(hSemaphore, INFINITE);
	system("pause");
	ReleaseSemaphore(hSemaphore, 1, (LPLONG)&count);
	CloseHandle(hSemaphore);
	return 0;
}
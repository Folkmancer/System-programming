// Mutex.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
	_tsetlocale(0, TEXT(""));
	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, TEXT("MutexObject"));
	if (hMutex == NULL) _tprintf(TEXT("Ошибка создания мьютекса: %d\n"), GetLastError());
	else if (GetLastError() == ERROR_ALREADY_EXISTS) _tprintf(TEXT("Процесс уже запущен!\n"));
	else _tprintf(TEXT("Это первый экземпляр процесса!\n"));
	system("pause");
	CloseHandle(hMutex);
	return 0;
}

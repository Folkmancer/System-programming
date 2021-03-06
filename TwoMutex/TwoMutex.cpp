// TwoMutex.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <ctime>

int main()
{
	_tsetlocale(0, TEXT(""));
	HANDLE hMutex;
	DWORD result;
	DWORD time = 3000;
	clock_t start, end;
	system("pause");
	hMutex = CreateMutex(NULL, FALSE, TEXT("MutexObject"));
	if (hMutex == NULL) _tprintf(TEXT("Ошибка создания мьютекса: %d\n"), GetLastError());
	else if (GetLastError() == ERROR_ALREADY_EXISTS) _tprintf(TEXT("Процесс уже запущен!\n"));
	else _tprintf(TEXT("Это первый экземпляр процесса!\n"));
	start = clock();
	do {
		result = WaitForSingleObject(hMutex, time);
		if (result == WAIT_FAILED) {
			std::cout << "Процесс не может получить доступ к ресурсу\n";
		}
		else if (result == WAIT_TIMEOUT) {
			std::cout << "Превышено время ожидания ресурса. . .\n";
		}
		else {
			std::cout << "Доступ к ресурсу получен. . .\n";
		}	
	} while (WaitForSingleObject(hMutex, time));
	end = clock();
	std::cout << "Время ожидания:  " << end - start << " ms" << std::endl;
	system("pause");
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
	return 0;
}


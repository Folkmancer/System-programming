// Memory.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <iomanip>

int main()
{
	_tsetlocale(0, TEXT(""));
	int M, N, min, max, index;
	bool find = false;
	std::cout << "Введите размероность массива M на N" << std::endl;
	std::cin >> M >> N;
	int** myarr = (int**)GlobalAlloc(0, M * sizeof(int));
	for (int i = 0; i < N; i++) myarr[i] = (int*)GlobalAlloc(0, N * sizeof(int));;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			myarr[i][j] = rand() % 30 + 1;
			std::cout << "[" << i << "]" << "[" << j << "] = " << myarr[i][j] << " ";
		}
		std::cout << std::endl;
	}
	int* minArray = (int*)GlobalAlloc(0, M * sizeof(int));
	for (int i = 0; i < M; i++) {
		min = myarr[i][0];
		for (int j = 0; j < N; j++) {
			if (min >= myarr[i][j]) {
				min = myarr[i][j];
			}	
		}
		minArray[i] = min;
		std::cout << "[" << i << "] = " << minArray[i] << " ";
	}
	std::cout << std::endl;
	max = minArray[0];
	for (int i = 0; i < M; i++) {
		if (max <= minArray[i]) {
			max = minArray[i];
		}
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (myarr[i][j] == max) {
				std::cout << "[" << i << "]" << "[" << j << "] = " << myarr[i][j] << " ";
				find = true;
				break;
			}
		}
		if (find) break;
	}
	std::cout << std::endl;
	HGLOBAL hGlobal = GlobalHandle(myarr);
	GlobalFree(hGlobal);
	hGlobal = GlobalHandle(minArray);
	GlobalFree(hGlobal);
    return 0;
}


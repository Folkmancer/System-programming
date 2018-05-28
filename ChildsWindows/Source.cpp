#include <iostream>
#include <Windows.h>
#include <WinBase.h>
#include <atlbase.h>
using namespace std;

int main() { 
	HWND hwnd, fWindowEx;
	hwnd = FindWindow(NULL, "Калькулятор ‎- Калькулятор");
	if (hwnd != NULL) {
		HMENU hMenu;
		hMenu = GetMenu(hwnd);
		if (hMenu != NULL) { 
			int iCount;     
			iCount = GetMenuItemCount(hMenu);
			cout << "Menu Item - " << iCount << endl; 
		} 
		else cout << " Error Loading Menu" << endl; 
	} 
	else cout << " Error Find Windows" << endl; 
	system("pause");
	return 0;
}
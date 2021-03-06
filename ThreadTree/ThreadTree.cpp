// ThreadTree.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <atlbase.h>

using namespace std;
#define _WIN32_WINT 0x0400
#define MAX_THREADS 2

typedef struct
{
	string path;
	int count;
}tree, *ptree;

DWORD WINAPI FindFile(LPVOID);
DWORD dwThreadIdArray[MAX_THREADS];

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	tree filesTree;
	filesTree.path = "D:\\OneDrive\\Repository\\System";
	filesTree.count = 0;
	HANDLE h = CreateThread(NULL, 0, &FindFile, &filesTree, 0, &dwThreadIdArray[0]);
	//PostThreadMessage(0, WM_QUIT, 0, 0);
	WaitForSingleObject(h, INFINITE);
	CloseHandle(h);
	system("pause");
	return 0;
}

DWORD WINAPI FindFile(LPVOID tr)
{
	USES_CONVERSION;
	WIN32_FIND_DATA FindFileData;
	ptree filesTree = new tree();
	filesTree->path = ((ptree)tr)->path;
	filesTree->count = ((ptree)tr)->count + 1;
	string curr = ((ptree)tr)->path;
	((ptree)tr)->path += "\\*";
	HANDLE hFind = FindFirstFile(A2W(((ptree)tr)->path.c_str()), &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (_tcscmp(FindFileData.cFileName, TEXT(".")) != 0 && _tcscmp(FindFileData.cFileName, TEXT("..")) != 0)
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					for (int i = 0; i < ((ptree)tr)->count; i++)
					{
						_tprintf(TEXT("-"));
					}
					_tprintf(TEXT("%s\n"), FindFileData.cFileName);
					wstring ws(FindFileData.cFileName);
					string add(ws.begin(), ws.end());
					filesTree->path = curr + ("\\" + add);
					FindFile(filesTree);
				}
				else
				{
					for (int i = 0; i < ((ptree)tr)->count; i++)
					{
						_tprintf(TEXT("-"));
					}
					if (_tcscmp(FindFileData.cFileName, TEXT("*.txt"))) _tprintf(TEXT("%s\n"), FindFileData.cFileName);
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);
		FindClose(hFind);
	}
	return 0;
}


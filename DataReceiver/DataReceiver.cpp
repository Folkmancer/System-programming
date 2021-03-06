#include "stdafx.h"
#include "DataReceiver.h"

#define MAX_LOADSTRING 100
#define FIRST_TIMER 1 

HINSTANCE hInst;
HWND hwndEditCopyData;
HWND hwndEditFile;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
int nTimerID;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DATARECEIVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DATARECEIVER));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DATARECEIVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DATARECEIVER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(
	   szWindowClass,
	   szTitle,
	   WS_SYSMENU,
	   CW_USEDEFAULT,
	   0,
	   275,
	   130,
	   nullptr,
	   nullptr,
	   hInstance,
	   nullptr);

   hwndEditCopyData = CreateWindowEx(
	   0, 
	   TEXT("EDIT"),
	   NULL,
	   WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
	   //WS_VSCROLL, | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
	   10,
	   10,
	   240,
	   20,
	   hWnd,
	   NULL,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);

   hwndEditFile = CreateWindowEx(
	   0, 
	   TEXT("EDIT"),
	   NULL,
	   WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
	   //WS_VSCROLL, | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
	   10,
	   40,
	   240,
	   20, 
	   hWnd,
	   NULL,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);

   nTimerID = SetTimer(hWnd, FIRST_TIMER, 1000, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_TIMER:
	{
		TCHAR buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);
		lstrcat(buffer, TEXT("\\file.txt")); 
		HANDLE hFile = CreateFile(
			buffer,
			GENERIC_READ,
			//FILE_SHARE_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			DWORD dwSize = GetFileSize(hFile, &dwSize);
			if (dwSize != 2) {
				TCHAR *lpText = new TCHAR[dwSize];
				DWORD dwBytesRead;
				ReadFile(hFile, lpText, sizeof(TCHAR)*dwSize, &dwBytesRead, NULL);
				SetWindowText(hwndEditFile, lpText);	
				
			}
			else SetWindowText(hwndEditFile, TEXT("Файл пуст!"));
			CloseHandle(hFile);
		}
		else {
			
			SetWindowText(hwndEditFile, TEXT("Файл не удалось открыть!"));
			//KillTimer(hWnd, FIRST_TIMER);
			//MessageBox(hWnd, TEXT("Файл не удалось открыть!"), TEXT("Ошибка"), MB_OK | MB_ICONEXCLAMATION);
			//nTimerID = SetTimer(hWnd, FIRST_TIMER, 4000, NULL);
		}
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COPYDATA:
	{
		COPYDATASTRUCT* cdsImport = (COPYDATASTRUCT*)lParam;
		if (cdsImport->dwData == 0)
		{
			LPCTSTR lpszString = (LPCTSTR)(cdsImport->lpData);
			SetWindowText(hwndEditCopyData, lpszString);
		}
	}
	break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

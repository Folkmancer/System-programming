#include "stdafx.h"
#include "DataSource.h"
#include "atlstr.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;   
HWND hwndEdit;
HWND hwndButtonFile;
HWND hwndButtonWM_COPYDATA;

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];


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
    LoadStringW(hInstance, IDC_DATASOURCE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DATASOURCE));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DATASOURCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DATASOURCE);
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
	   WS_SYSMENU | WS_MINIMIZE,
	   CW_USEDEFAULT, 
	   0, 
	   275, 
	   130, 
	   nullptr, 
	   nullptr, 
	   hInstance, 
	   nullptr);

   hwndEdit = CreateWindowEx(
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
   
   hwndButtonWM_COPYDATA = CreateWindow(
	   TEXT("BUTTON"),
	   TEXT("Процесс"),
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   10,
	   40,
	   100,
	   20,
	   hWnd,
	   (HMENU)1600,
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL); 

   hwndButtonFile = CreateWindow(
	   TEXT("BUTTON"),
	   TEXT("Файл"),
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   150,
	   40,
	   100,
	   20,
	   hWnd,
	   (HMENU)1500, 
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL); 

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
			case 1500:
				{
					TCHAR buffer[MAX_PATH];
					GetCurrentDirectory(sizeof(buffer), buffer);
					lstrcat(buffer, TEXT("\\file.txt"));
					DWORD dwTextSize = GetWindowTextLength(hwndEdit) + 1;
					TCHAR *lpszText = new TCHAR[dwTextSize];
					GetWindowText(hwndEdit, lpszText, dwTextSize);
					HANDLE hFile = CreateFile(
						buffer,
						GENERIC_WRITE,
						NULL, //0
						NULL, 
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, 
						NULL);
					if (hFile != NULL) {
						DWORD dwBytesWritten;
						WriteFile(hFile, lpszText, sizeof(TCHAR)*dwTextSize, &dwBytesWritten, NULL);
						CloseHandle(hFile);
					}
					else if (hFile == INVALID_HANDLE_VALUE)
					{
						MessageBox(hWnd, TEXT("Не удалось создать файл!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
					}
					else MessageBox(hWnd, TEXT("Не удалось создать файл!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
				}				
				break;
			case 1600:
			{
				COPYDATASTRUCT cdsExport;
				DWORD dwTextSize = GetWindowTextLength(hwndEdit) + 1;
				TCHAR *lpszText = new TCHAR[dwTextSize];
				GetWindowText(hwndEdit, lpszText, dwTextSize);
				cdsExport.dwData = 0;
				cdsExport.cbData = sizeof(TCHAR)*dwTextSize;
				cdsExport.lpData = lpszText; 
				HWND hwDispatch = FindWindow(NULL, TEXT("DataReceiver"));
				if (hwDispatch != NULL)
					SendMessage(hwDispatch,
						WM_COPYDATA,
						(WPARAM)(HWND)hWnd,
						(LPARAM)(LPVOID)&cdsExport);
				else
					MessageBox(hWnd, TEXT("Нет соединения с приложением-приёмником!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
			}
			break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
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

# include<Windows.h>
#include "Defines.h"
#include "GameManager.h"


//globala saker

LPCWSTR g_szApplicationName = L"Luffarluffarschachschack";
LPCWSTR g_szWindowClassName = L"MyWindowClass";
GameManager* gm;

static LRESULT CALLBACK WindowProc (HWND   hWnd,
	UINT   msg,
	WPARAM wParam,
	LPARAM lParam)
{
	static int clientX, clientY;
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SPACE:
				{
					gm->space();
					InvalidateRect(hWnd, NULL, TRUE);
					return 0;
				}
			}
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			/*anropa funktionen för att rita här */
			gm->setBoardWidth(clientX);
			gm->setBoardHeight(clientY);
			gm->run();

			EndPaint(hWnd, &ps);
			return 0;
		}
	case WM_CREATE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			clientX=rect.right;
			clientY=rect.bottom;
			break;
		}
	case WM_SIZE:
		{
			clientX=LOWORD(lParam);
			clientY=HIWORD(lParam);
			break;
		}
	case WM_LBUTTONDOWN:
		{
				int curserPosX=LOWORD(lParam);
				int curserPosY=HIWORD(lParam);
				if(gm->debugMode)
				{
					gm->debugClick(curserPosX, curserPosY, true);
				}
				else
				{
					
					if(!gm->isStartingUp())
					{
						gm->click(curserPosX, curserPosY);
					}
					else
					{
						gm->decideMode(curserPosX, curserPosY);
					}
				}
				InvalidateRect(hWnd, NULL, TRUE);
			
			break;
		}
	case WM_RBUTTONDOWN:
		{
			int curserPosX=LOWORD(lParam);
			int curserPosY=HIWORD(lParam);
			if(gm->debugMode)
			{
				gm->debugClick(curserPosX, curserPosY, false);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

	}


	return DefWindowProc (hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{



	HWND hWnd;
	WNDCLASSEX winclass;
	winclass.cbSize        = sizeof(WNDCLASSEX);
	winclass.style         = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc   = WindowProc;
	winclass.cbClsExtra    = 0;
	winclass.cbWndExtra    = 0;
	winclass.hInstance     = hInstance;
	winclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
	winclass.lpszMenuName  = NULL;
	winclass.lpszClassName = g_szWindowClassName;
	winclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);



	//register the window class
	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, L"Class Registration Failed!", L"Error", 0);

		//exit the application
		return 0;
	}

	hWnd=CreateWindowEx(NULL, g_szWindowClassName,g_szApplicationName,WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//hWnd = CreateWindowEx (NULL,                 // extended style
	//	g_szWindowClassName,  // window class name
	//	g_szApplicationName,  // window caption
	//	WS_OVERLAPPEDWINDOW,  // window style
	//	0,                    // initial x position
	//	0,                    // initial y position
	//	WINDOW_WIDTH,         // initial x size
	//	WINDOW_HEIGHT,        // initial y size
	//	NULL,                 // parent window handle
	//	NULL,                 // window menu handle
	//	hInstance,            // program instance handle
	//	NULL);                // creation parameters

	//make sure the window creation has gone OK
	if(!hWnd)
	{
		MessageBox(NULL, L"CreateWindowEx Failed!", L"Error!", 0);
	}

	RECT client;
	GetClientRect(hWnd, &client);
	gm=new GameManager(&hWnd, client.right-client.left , client.bottom-client.top);

	//make the window visible
	ShowWindow (hWnd, 10);//iCmdShow);

	UpdateWindow(hWnd);

	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete gm;

	UnregisterClass( g_szWindowClassName, winclass.hInstance );

	return 0;
}
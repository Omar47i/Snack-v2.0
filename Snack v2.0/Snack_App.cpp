//==================================================================================
// File : Snack_App.cpp
//
// Desc : This is a simple Snack game based on the Win32 app programming
//
// CopyRight 22/10/2010 made by Omar Saeed Abd ElHalim
//==================================================================================
#include <windows.h>
#include "BackBuffer.h"
#include "Define.h"
#include "Class.h"

HWND      ghMainWnd = 0;            //main window handle
HINSTANCE ghAppInst = 0;            //application instance handle

// The backbuffer we will render onto.
BackBuffer* gBackBuffer = 0;

Snack gSnack;
Food gFood;
GameArea gGameArea;

const int food_time = 110;
int frame_count = 0;

//=========================================================
// Function Prototypes
//=========================================================

bool InitMainWindow();
int  Run();

LRESULT CALLBACK 
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=========================================================
// Name: WinMain
// Desc: Program execution starts here.
//=========================================================

int WINAPI 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
		PSTR cmdLine, int showCmd)
{
	ghAppInst = hInstance;

	// Create the main window.
	if( !InitMainWindow() )
	{
		MessageBoxA(0, "Window Creation Failed.", "Error", MB_OK);
		return 0;
	}

	// Enter the message loop.
	return Run();
}

//=========================================================
// Name: InitMainWindow
// Desc: Creates the main window upon which we will
//       draw the game graphics onto.
//=========================================================
bool InitMainWindow()
{
	WNDCLASS wc; 
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = ghAppInst;
	wc.hIcon         = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = TEXT("MyWndClassName");

	RegisterClass( &wc );

	// WS_OVERLAPPED | WS_SYSMENU: Window cannot be resized
	// and does not have a min/max button.  
	ghMainWnd = ::CreateWindowA("MyWndClassName", 
		GAME_NAME, WS_POPUP,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
		0,
	    0, ghAppInst, 0);

	if(ghMainWnd == 0)
	{
		::MessageBoxA(0, "CreateWindow - Failed", 0, 0);
		return 0;
	}

	ShowWindow(ghMainWnd, SW_NORMAL);
	UpdateWindow(ghMainWnd);

	return true;
}

//=========================================================
// Name: Run
// Desc: Encapsulates the message loop.
//=========================================================
int Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(msg.message != WM_QUIT)
	{
		// IF there is a Windows message then process it.
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ELSE, do game stuff.
		else
		{
			// Get the backbuffer DC.
	        HDC hbbDC = gBackBuffer->getDC();

			// draw the game area
			gGameArea.Draw_Game_Area(hbbDC);

			// display the snack
			gSnack.display_snack(hbbDC);

			bool bSnack_hit = gSnack.check_for_snack_hit(hbbDC);
			if( bSnack_hit )
			{
				gBackBuffer->present();
				PlaySound(TEXT("gameover2"), 0, SND_SYNC);
				msg.message = WM_QUIT;
				break;
			}

			// let the food know whether the snack eated the food
			gFood.setFoodPt( gSnack.get_food_pt() );

			// every 50 frames, display the food
			if( (frame_count % food_time) == 0 )
			{
				gFood.setFoodPt();
				// let the snack object know the food coordinates
				gSnack.setFoodPt( gFood.get_food_pt() );
			}

			// display the food
			gFood.display_food(hbbDC);

			// present the back buffer contents
			gBackBuffer->present();

			Sleep(44);

			frame_count++;
		}
	}
	return (int)msg.wParam;
}

//=========================================================
// Name: WndProc
// Desc: The main window procedure.
//=========================================================

LRESULT CALLBACK 
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_CREATE:
		gBackBuffer = new BackBuffer(hWnd, CLIENT_WIDTH, CLIENT_HEIGHT);

		::ShowCursor(FALSE);

		return 0;
	case WM_KEYDOWN: 

		switch( wParam )
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			return 0;

		case VK_UP:
			gSnack.set_Dir(DIR_UP);
			return 0;

		case VK_DOWN:
			gSnack.set_Dir(DIR_DOWN);
			return 0;

		case VK_LEFT:
			gSnack.set_Dir(DIR_LEFT);
			return 0;

		case VK_RIGHT:
			gSnack.set_Dir(DIR_RIGHT);
			return 0;

		default:
			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		delete gBackBuffer;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
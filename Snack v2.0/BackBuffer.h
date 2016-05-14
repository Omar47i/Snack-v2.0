// BackBuffer.h - This is the back buffer header file that will contain a blank bitmap
// that is compatible with the main window DC bitmap format, the back buffer is used
// to draw the game component onto it and then copy its contents to the main window

#ifndef BACKBUFFER_H
#define BACKBUFFER_H

#include <windows.h>

class BackBuffer
{
	// private data members
private:
	HWND mhWnd;                       //handle to the main window
	int  mWidth;                      //width of the main window
	int  mHeight;                     //height of the main window
	HDC  mhdc;                        //handle to a DC
	HBITMAP mhSurface;                //surface bitmap that will be render onot
	HBITMAP mhOldObject;              //handle to the originally loaded bitmap

public:
	// public method members
	BackBuffer(HWND hWnd, int width, int height)
	{
		// save the handle of the main window
		mhWnd = hWnd;

		// SAVE THE DIMENSIONS OF THE MAIN WINDOW
		mWidth  = width;
		mHeight = height;

		// get a handle to the main window device context
		HDC hdc = GetDC(hWnd);

		// create a memory DC that is compatible with the main window DC
		mhdc = ::CreateCompatibleDC(hdc);

		// create the bitmap that is compatible with the
		// main window DC bitmap format
		mhSurface = CreateCompatibleBitmap(hdc, width, height);

		// release the window DC
		ReleaseDC(hWnd, hdc);

		// select the back buffer bitmap
		mhOldObject = (HBITMAP)SelectObject(mhdc, mhSurface);

		// select a white brush
		HBRUSH hOldBrush = (HBRUSH)SelectObject(mhdc, GetStockObject(WHITE_BRUSH));

		// clear the back buffer with a white brush for valid sprites rendering
		Rectangle(mhdc, 0, 0, mWidth, mHeight);

		// select the old brush
		SelectObject(mhdc, hOldBrush);
	}

	~BackBuffer()
	{
		DeleteObject(mhSurface);
		DeleteObject(mhOldObject);
		DeleteDC(mhdc);
	}

	HDC getDC()                   //return the back buffer DC
	{ return mhdc; }

	void present()
	{
		// get the main window DC
		HDC hdc = GetDC(mhWnd);

		// copy the contents of the back buffer to the main window
		BitBlt(hdc, 0, 0, mWidth, mHeight, mhdc, 0, 0, SRCCOPY);

		// release the dc
		ReleaseDC(mhWnd, hdc);
	}
};

#endif
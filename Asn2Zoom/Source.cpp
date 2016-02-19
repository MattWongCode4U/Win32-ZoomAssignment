//Matthew Wong A00925594

/*******************************************************************
A2 FALL 2015

Purpose: To explore scrolling and mapping modes and drawing

Description:

(1) A rectangle is drawn that is 50 pixels less on all sides of the client area.
Rectangle is drawn with pen width = 1, color=black.
This area marks the scrolling and zooming boundary.

(2) A GREEN rectangle (pen width =2) is drawn using an origin of the 
centre of the window.
Horizontal scrolling is done using the horizontal scrollbar with the
range: -clientwidth/2 to clientwidth/2 with the scroll position set to 
the centre of that range. The thumbtracking is disabled as in example.
Scrolling is limited to the black rectangle first described in (1).

(3) Zooming in/out uses the vertical scrollbar with a range set 1 to 1000.
Thumb position is set to 100 to start.
Zooming is limited to the rectangle defined in (1)


DO NOT WORRY THAT SCROLLING A ZOOMED RECTANGLE OFF THE SCROLLABLE AREA
RESULTS IN A CHANGED RECTANGLE (gets thinner).

HELPFUL STUFF:

You need to review InvaldiateRect()
and set the appropriate parameter to the appropriate value. You will
need use of structure RECT for this.

Also be aware of objects you want to zoom and scroll vs. objects you
don't wish to zoom and scroll. 

There is a simple and proper way to accomplish these goals. How you accomplish
these tasks will indicate your understanding of scrolling and mapping.
Marks will be lost for inappropriate solutions or solutions that fail to 
demonstrate proper scrolling and mapping techniques (i.e. just getting it
to seemingly work as the example is NOT enough).

Marking Guide

Rectangles drawn correctly				1 mark
scrolling within limited area			3 marks
zooming within limited area				3 marks
external rectangle not scrolled/zoomed	2 marks
scrollbars range and pos set			1 mark
*******************************************************************/

#include <windows.h>
#include <mmsystem.h>
#define LEFT 102
#define RIGHT 102
#define TOP 52
#define BOTTOM 52

LRESULT CALLBACK HelloWndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					PSTR szCMLine, int iCmdShow){
	static TCHAR szAppName[] = TEXT ("HelloApplication");//name of app
	HWND	hwnd;//holds handle to the main window
	MSG		msg;//holds any message retrieved from the msg queue
	WNDCLASS wndclass;//wnd class for registration

	//defn wndclass attributes for this application
	wndclass.style		= CS_HREDRAW | CS_VREDRAW;//redraw on refresh both directions
	wndclass.lpfnWndProc = HelloWndProc;//wnd proc to handle windows msgs/commands
	wndclass.cbClsExtra	= 0;//class space for expansion/info carrying
	wndclass.cbWndExtra = 0;//wnd space for info carrying
	wndclass.hInstance	= hInstance;//application instance handle
	wndclass.hIcon		= LoadIcon (NULL, IDI_APPLICATION);//set icon for window
	wndclass.hCursor	= LoadCursor (NULL, IDC_ARROW);//set cursor for window
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);//set background
	wndclass.lpszMenuName = NULL;//set menu
	wndclass.lpszClassName = szAppName;//set application name

	//register wndclass to O/S so approp. wnd msg are sent to application
	if (!RegisterClass (&wndclass)){
		MessageBox (NULL, TEXT ("This program requires Windows 95/98/NT"),
					szAppName, MB_ICONERROR);//if unable to be registered
		return 0;
	}

	//create the main window and get it's handle for future reference
	hwnd = CreateWindow(szAppName,		//window class name
						TEXT("Hello World for Windows"), // window caption
						WS_OVERLAPPEDWINDOW| WS_VSCROLL | WS_HSCROLL,	//window style
						CW_USEDEFAULT,		//initial x position
						CW_USEDEFAULT,		//initial y position
						CW_USEDEFAULT,		//initial x size
						CW_USEDEFAULT,		//initial y size
						NULL,				//parent window handle
						NULL,				//window menu handle
						hInstance,			//program instance handle
						NULL);				//creation parameters

	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

	//messgae loop
	while (GetMessage(&msg, NULL, 0, 0)){//get message from queue
		TranslateMessage(&msg);//for keystroke translation
		DispatchMessage(&msg);//pass msg back to windows for processing
		//note that this is to put windows o/s in control, rather than this app
	}

	return msg.wParam;
}


/**
Purpose: To handle windows messages for specific cases including when
		 the window is first created, refreshing (painting), and closing
		 the window.

Returns: Long
Notes:	 CALLBACK is defined as __stdcall which defines a calling
		 convention for assembly (stack parameter passing)
**/
LRESULT CALLBACK HelloWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC		hdc;
	PAINTSTRUCT ps;
	RECT	rect, rect2;
	static int  cxClient,cyClient;
	int iVertPos, iHorzPos;
	SCROLLINFO si;
	HPEN penBLK = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN penGRN = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	GetScrollInfo(hwnd, SB_VERT, &si);
	si.nPos = 100;

	switch (message){
		case WM_CREATE://additional things to do when window is created
			return 0;


		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
			si.nMin = 1;
			si.nMax = 1000;
			si.nPage = cyClient - 500;
			si.nPos = 100;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = -cxClient / 2;
			si.nMax = cxClient / 2;
			si.nPage = cxClient - 1100;
			si.nPos = 0;
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
			return 0;
		
		case WM_HSCROLL:
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;

			GetScrollInfo(hwnd, SB_HORZ, &si);
			iHorzPos = si.nPos;

			rect.left = 51;
			rect.top = 51;
			rect.right = cxClient - 52;
			rect.bottom = cyClient - 52;

			switch (LOWORD(wParam))
			{
			case SB_LINELEFT:
				si.nPos -= 1;
				break;

			case SB_LINERIGHT:
				si.nPos += 1;
				break;

			case SB_PAGELEFT:
				si.nPos -= si.nPage;
				break;

			case SB_PAGERIGHT:
				si.nPos += si.nPage;
				break;

			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;

			default:
				break;
			}

			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
			GetScrollInfo(hwnd, SB_HORZ, &si);

			if (si.nPos != iHorzPos)
			{
				InvalidateRect(hwnd, &rect, TRUE);
				UpdateWindow(hwnd);
			}
			return 0;

		case WM_VSCROLL:
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hwnd, SB_VERT, &si);
			iVertPos = si.nPos;

			rect.left = 51;
			rect.top = 51;
			rect.right = cxClient - 52;
			rect.bottom = cyClient - 52;

			switch (LOWORD(wParam)) {
			case SB_TOP:
				si.nPos = si.nMin;
				break;

			case SB_BOTTOM:
				si.nPos = si.nMax;
				break;

			case SB_LINEUP:
				si.nPos--;
				break;

			case SB_LINEDOWN:
				si.nPos++;
				break;

			case SB_PAGEUP:
				si.nPos -= si.nPage;
				break;

			case SB_PAGEDOWN:
				si.nPos += si.nPage;
				break;

			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;

			default:
				break;
			}

			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
			GetScrollInfo(hwnd, SB_VERT, &si);

			if (si.nPos != iVertPos) {
				InvalidateRect(hwnd, &rect, TRUE);
				UpdateWindow(hwnd);
			}
			return 0;

		case WM_PAINT://what to do when a paint msg occurs
			hdc = BeginPaint(hwnd, &ps);//get a handle to a device context for drawing

			SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

			SetMapMode(hdc, MM_ISOTROPIC);
			SetWindowExtEx(hdc, 200, 200, NULL);
			SetViewportExtEx(hdc, 200, 200, NULL);

			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;

			GetScrollInfo(hwnd, SB_HORZ, &si);
			iHorzPos = si.nPos;//current horizontal position

			GetScrollInfo(hwnd, SB_VERT, &si);
			iVertPos = si.nPos;

			//Black Rectangle
			SelectObject(hdc, penBLK);
			Rectangle(hdc, -cxClient / 2 +50, -cyClient /2  + 50, cxClient/2 - 50, cyClient/2 - 50);

			SetWindowExtEx(hdc, 100, 100, NULL);
			SetViewportExtEx(hdc, 100 + iVertPos, 100 + iVertPos, NULL);

			//Green Rectangle
			SelectObject(hdc, penGRN);
			Rectangle(hdc, -50 -iHorzPos, -50, +50 - iHorzPos, +50);

			EndPaint(hwnd, &ps);//release the device context
			return 0;

		case WM_DESTROY://how to handle a destroy (close window app) msg
			PostQuitMessage(0);
			return 0;
	}
	//return the message to windows for further processing
	return DefWindowProc(hwnd, message, wParam, lParam);
}
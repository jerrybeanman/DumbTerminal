/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Session.cpp - Actualy function implementation for Session.h. Handles Physical layer of the program
--		dumb terminal emulator.
--			dumb terminal emulator program
--
-- PROGRAM: Dumb terminal emulator
--
-- FUNCTIONS:
-- VOID Initialize_Window(HINSTANCE &hInst, int nCmdShow, HWND &hwnd, WNDCLASSEX &wcl);
-- VOID Initialize_WNDCLASSEX(WNDCLASSEX &wcl, HINSTANCE &hInst);
-- VOID Display_Help();
-- VOID Draw(const char *str, const HDC &hdc, const COLORREF &color, const BOOL &isRepaint, HWND hwnd);
-- VOID Repaint(HWND hwnd);
-- VOID Handle_Menu_Commands(HWND hwnd, WPARAM wParam);
-- BOOL Connect(HWND hwnd);
-- VOID Disconnect(HWND hwnd);
-- VOID Record_Inputs(const char *str, const COLORREF &color, const BOOL &isRepaint);
--
--
--
-- DATE: October 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- NOTES:
--	This application mainly handles general UI elements and flow of control of the main program.
--  Some functionalities include initilizing a window application to work on, process menuitems,
--  switching in and out of connect mode, displaying characters on screen, and handling resizing
--  performed on the window.
--  The Handle_Menu_Commands will handle and process all actions performed on the menu items, such
--	as displaying the user manual "Help" box, switching background colors for read/write, and
--  displaying serial port configuration window. When the program enters connect mode, read and write
--  operation will be availiable and characters with the corresponding background will be displayed
--  on the screen. The application will also handle special characters that are being processed, such
--  as backspace(remove a character) and return carriage(new line). Additionally, resizing the window
--  will not affect the content on screen and the apllication will handle word warpping accordingly.
--
--	Note that the specific features that handles read and write operations will be outlined in Physical.h
----------------------------------------------------------------------------------------------------------------------*/

#include "Session.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Initialize_Window
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: void Initialize_Window (HINSTANCE		hInst,
--									  int			nCmndShow
--									  HANDLE		hwnd,
--								      WNDCLASSEX	wcl)
--				-HINSTANCE hInst:	A handle to the current instance of the application.
--				-int nCmndShow:		Controls how the window is to be shown. In this case it's set to default
--				-HANDLE hwnd:		The handle returned by CreateWindow()
--				-WNDCLASSEX wcl:	The windows class to initilize, register and update
--
-- RETURNS: void.
--
-- NOTES:
--	This function is called on the start of the application to initialize WNDCLASSEX by calling Initialize_WNDCLASSSEX().
--	It is then registered and associated with the corresponding handle for the window.
--	and associate hwnd with it
----------------------------------------------------------------------------------------------------------------------*/
VOID Initialize_Window(HINSTANCE &hInst, int nCmdShow, HWND &hwnd, WNDCLASSEX &wcl)
{
	Initialize_WNDCLASSEX(wcl, hInst);	//Set valuues to the current windows class
	if (!RegisterClassEx(&wcl))			//Register window class 
		MessageBox(NULL, "Error Registering class:", "", MB_OK);
	if (!(hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10,
		600, 400, NULL, NULL, hInst, NULL)))	//Create an overlapped window
		MessageBox(NULL, "Error Creating window:", "", MB_OK);
	ShowWindow(hwnd, nCmdShow);				//Set the window show state
	UpdateWindow(hwnd);						//Display the window graphically
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Initialize_WNDCLASSEX
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: void Initialize_WNDCLASSEX (WNDCLASSEX	wcl,
--										  HINSTANCE		hInst);
--					-WNDCLASSEX wcl:	The structure that contains the windows class information to initialize
--					-HINSTANCE hInst:	The handle to the current instance of the application
--
-- RETURNS: VOID.
--
-- NOTES:
--	Initializes all members of a WNDCLASSEX to some default values
----------------------------------------------------------------------------------------------------------------------*/
VOID Initialize_WNDCLASSEX(WNDCLASSEX &wcl, HINSTANCE &hInst)
{
	wcl.cbSize	= sizeof(WNDCLASSEX);
	wcl.style	= CS_HREDRAW | CS_VREDRAW;
	wcl.hIcon	= LoadIcon(NULL, IDI_APPLICATION);	// large icon 
	wcl.hIconSm	= NULL;								// use small version of large icon
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);		// cursor style

	wcl.lpfnWndProc		= WndProc;
	wcl.hInstance		= hInst;
	wcl.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	wcl.lpszClassName	= Name;

	wcl.lpszMenuName	= "MYMENU"; // The menu Class
	wcl.cbClsExtra		= 0;		// no extra memory needed
	wcl.cbWndExtra		= 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Display_Help
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: void Display_Help ()
--
-- RETURNS: VOID.
--
-- NOTES:
--	Display help message box containing the information about the functionality of the program. The content is parsed from
--	a text file(Help.txt) using ifstream
----------------------------------------------------------------------------------------------------------------------*/
VOID Display_Help()
{
	std::ifstream iF("Help.txt");	//Open file for reading
	std::string s((std::istreambuf_iterator<char>(iF)),	//Store file content in a string
	std::istreambuf_iterator<char>());
	MessageBox(NULL, s.c_str(), "Help", MB_OK);	//Display help message box with the file content
	iF.close();	//Close file
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Draw
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: void Draw (const char		*str,
--						 const HDC		&hdc,
--						 const COLORREF &color,
--						 const BOOL		&isRepaint);
--					-const char		*str:		A pointer to a character to be drawn
--					-const HDC		&hdc:		The device context
--					-const COLORREF	&color:		The background color str will be displayed in
--					-const BOOL		&isRepaint:	Validate if this is used for repaint or not,
--									if it is, call v.push_back(str), else do nothing
--
-- RETURNS: void
--
-- NOTES:
--	Process characters to be displayed on the screen with the specific background color. Each character's position
--	is pinpointed using the Coordinates struct, displayed left to right. Additionally the function also
--	handles backspace(delete a character) and return characters(new line).
----------------------------------------------------------------------------------------------------------------------*/
void Draw(const char		*str, 
		  const HDC			&hdc, 
		  const COLORREF	&color, 
		  const BOOL		&isRepaint,
		  HWND hwnd)
{
	SIZE		size;				//The current size of the string to be displayed
	TEXTMETRIC	tm;					//The basic information of the font
	GetTextMetrics(hdc, &tm);		//Set tm
	GetTextExtentPoint32(hdc, str, strlen(str), &size);	//Set size
	if (*str == '\b') 
	{
		if (rwHistory.size() >  0)
			rwHistory.pop_back();
		InvalidateRect(hwnd, NULL, TRUE);	//send a WM_PAINT to WndProc
		return;
	}
	else if (*str == '\r')
		coor._x = 0, coor._y += tm.tmHeight + tm.tmExternalLeading; // next line;
	else 
	{
		RECT rc;
		if (GetWindowRect(hwnd, &rc) && coor._x > (rc.right - 50))		//Handles line wrap
			coor._x = 0, coor._y += tm.tmHeight + tm.tmExternalLeading; //Increment x and add new line
		if (SetBkColor(hdc, color) && TextOut(hdc, coor._x, coor._y, str, strlen(str))) //set color and display
			coor._x += size.cx;				//Increment x coordinate
	}
	if (!isRepaint)		//Check if this is called by repaint or not
		rwHistory.push_back(std::make_pair(std::string(str), color));	//Adds the string and color to hwHistory
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Repaint
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: VOID Repaint()
-- RETURNS: VOID
--
-- NOTES:
--	Called when the WM_PAINT macro is triggerd. The function iterate through hwHistory and redraws all the
--	characters currently displayed on the screen
----------------------------------------------------------------------------------------------------------------------*/
VOID Repaint(HWND hwnd)
{
	PAINTSTRUCT ps;		
	HDC hdc = BeginPaint(hwnd, &ps);	//specify for painting operation and fill out ps
	coor.Reset();						//Set the coordinates of coors to (0,0), so paint from the beginning
	for(auto &x : rwHistory)			//Iterate through all the characters that has been processed on the screen
		Draw(x.first.c_str(), hdc, x.second, TRUE, hwnd);
	EndPaint(hwnd, &ps);				//End painting operation
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Handle_Menu_Commands
--
-- DATE: October 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: VOID Handle_Menu_Commands(HWND hwnd, WPARAM wParam);
--					-HWND	hwnd:	Handle to the current window
--					-WPARAM wParam: Contains information of the current message
-- RETURNS: VOID
--
-- NOTES:
--	Handle and process all menu items that are being triggered in the program.
----------------------------------------------------------------------------------------------------------------------*/
VOID Handle_Menu_Commands(HWND hwnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_HELP:
		Display_Help(); //Display help message box	
		break;
	case IDM_CONNECT:
		if (!Connect(hwnd))
			MessageBox(NULL, "Error Creating thread for reading", "", MB_OK);
		break;
	case IDM_EXIT:
		Disconnect(hwnd);
		break;
	case IDM_RRED:
		read_color = RGB(255, 0, 0);
		break;
	case IDM_RWHITE:
		read_color = RGB(255, 255, 255);
		break;
	case IDM_RGREEN:
		read_color = RGB(0, 255, 0);
		break;
	case IDM_RYELLOW:
		read_color = RGB(255, 255, 0);
		break;
	case IDM_RGREY:
		read_color = RGB(102, 102, 102);
		break;
	case IDM_RBLUE:
		read_color = RGB(0, 0, 255);
		break;
	case IDM_WRED:
		write_color = RGB(255, 0, 0);
		break;
	case IDM_WWHITE:
		write_color = RGB(255, 255, 255);
		break;
	case IDM_WGREEN:
		write_color = RGB(0, 255, 0);
		break;
	case IDM_WYELLOW:
		write_color = RGB(255, 255, 0);
		break;
	case IDM_WGREY:
		write_color = RGB(102, 102, 102);
		break;
	case IDM_WBLUE:
		write_color = RGB(0, 0, 255);
		break;
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Connect
--
-- DATE: October 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: BOOL Connect(HWND hwnd);
--					-HWND hwnd: Handle to the current windows
--
-- RETURNS: TRUE if read thread is successfully created. else FALSE
--
-- NOTES:
--	Enters "Connect" mode of the program. Calls Setup_Comm_Config for the user to enter custom communication parameters
--	and creates a thread for reading
----------------------------------------------------------------------------------------------------------------------*/
BOOL Connect(HWND hwnd)
{
	if (!Setup_Comm_Config(hwnd))
		return FALSE;
	isConnected = TRUE;	//Enter connect mode 
	return (rThread = CreateThread(NULL, 0, Read_From_Serial,(LPVOID)hwnd, 0, &rThreadId)) 
	? TRUE : FALSE;	//Create thread for reading
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Disconnect
--
-- DATE: October 4, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: VOID Disconnect(HWND hwnd);
--					-HWND hwnd: Handle to the current windows
--
-- RETURNS: VOID
--
-- NOTES:
--	Exits "Connect" mode of the program. All drawing coordinates and I/O operation histories are cleared to 0.
-- Addtionally it force the window to send a WM_PAINT message to WndProc inorder to wipe out all characters on screen.
----------------------------------------------------------------------------------------------------------------------*/
VOID Disconnect(HWND hwnd)
{
	isConnected = FALSE;	//Exit connect mode
	coor.Reset();			//set x y values to 0
	rwHistory.clear();		//delete content of all I/O operation 
	InvalidateRect(hwnd, NULL, TRUE);	//send a WM_PAINT to WndProc
	CloseHandle(rThread);	//Close read thread handle
	CloseHandle(hComm);		//Close communication handle
}


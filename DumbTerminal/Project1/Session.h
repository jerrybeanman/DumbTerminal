/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Session.h - Headefile that contains fucntion prototypes for the session layer of the
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

#ifndef SESSION_H
#define SESSION_H
#include "Application.h"
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
VOID Initialize_Window(HINSTANCE &hInst, int nCmdShow, HWND &hwnd, WNDCLASSEX &wcl);

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
VOID Initialize_WNDCLASSEX(WNDCLASSEX &wcl, HINSTANCE &hInst);

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
--	Display help message box containing the information about the functionality of the program
----------------------------------------------------------------------------------------------------------------------*/
VOID Display_Help();

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
--					-HWND	hwnd:		Handle to the current window
--
-- RETURNS: void
--
-- NOTES:
--	Process characters to be displayed on the screen with the specific background color. Each character's position
--	is pinpointed using the Coordinates struct, displayed left to right. Additionally the function also
--	handles backspace(delete a character) and return characters(new line).
----------------------------------------------------------------------------------------------------------------------*/
VOID Draw(const char *str, const HDC &hdc, const COLORREF &color, const BOOL &isRepaint, HWND hwnd);

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
VOID Repaint(HWND hwnd);

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
VOID Handle_Menu_Commands(HWND hwnd, WPARAM wParam);

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
BOOL Connect(HWND hwnd);

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
VOID Disconnect(HWND hwnd);

#endif
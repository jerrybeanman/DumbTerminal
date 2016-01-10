/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Physical.h - Headefile that contains fucntion prototypes for the physical layer of the 
--			dumb terminal emulator program
--
-- PROGRAM: Dumb terminal emulator
--
-- FUNCTIONS:
-- BOOL Initialize_Serial_Port();
-- LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
-- BOOL Setup_Comm_Config(HWND hwnd);
-- VOID Output_GetLastError();
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
--	This application will mainly handle window messages passed by the main program,
--  initialization of the serial port, and asynchronous I/O operations being performed on it.
--  When the program is first started, a handle to the serial port will be initilized to perform
--  asynchrounous read/write. Messages in WinMain the is recieved and processed by WndProc, which acts 
--  as the command centre of the program.
--	When the program enters connect mode, a thread that continues to loop on forever that performs
--  reading inputs from the serial port will be created. Any characters read will be displayed in a 
--  colored background specified by the user (default to Green). On the other hand, the writing operation
--  is performed on the main thread of the program (Handled by WndProc). When a keypress is captured 
--  and processed, the corresponding character will be displayed in a colored backgorund also specified 
--  by the user.
--
--	Note that the specific features that handles character display and menuitems will be outlined in Session.h
----------------------------------------------------------------------------------------------------------------------*/

#ifndef PHYSICAL_H
#define PHYSICAL_H
#include "Application.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Initialize_Serial_Port
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: BOOL Initialize_Serial_Port(HANDLE hComm);
--					-HANDLE hComm: The handle to the communication device returned by CreateFile()
--
-- RETURNS: TRUE if the serial port successfully opens, FALSE otherwise.
--
-- NOTES:
--	Attempt to open a communication device, in this case the serial port, for asynchronous reading and writing
----------------------------------------------------------------------------------------------------------------------*/
BOOL Initialize_Serial_Port();

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: LRESULT CALLBACK WndProc(HWND		hwnd,
--									   UINT		Message,
--									   WPARAM	wParam,
--									   LPARAM	lParam);
--					-HWND hwnd:		Handle to the current window
--					-UINT Message:	Message retrieved from WinMain
--					-WPARAM wParam:	Additional message information
--					-LPARAM lParam:	Additional message information
--
-- RETURNS: The return value is the result of the message processing and depends on the message sent.
--
-- NOTES:
--	Processes messages sent to a window from WinMain
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Setup_Comm_Config
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: BOOL Setup_Comm_Config(HWND hwnd);
--					-HWND hwnd: Handle to the current window
--
-- RETURNS: TRUE if the DCB struture is successfully set by the driver-supplied configuration dialog
--				FALSE otherwise
--
-- NOTES:
--	Opens a driver-supplied configuration dialog box for user to initialize the protocol values in a DCB structure 
--	so two devices can communicate between each other
----------------------------------------------------------------------------------------------------------------------*/
BOOL Setup_Comm_Config(HWND hwnd);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Read_From_Serial
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: DWORD WINAPI Read_From_Serial(LPVOID hwnd);
--					-LPVOID hwnd: A void pointer to the handle of the current window
--
-- RETURNS: 0 when the thread is being terminated
--
-- NOTES:
--	Called by the CreateThread function. This function loops forever attempting to read bytes from the serial port.
--	Also checks that the overlapped object that is being passed in the ReadFile function is in a signaled state
--	(character arrived but it's in the que) when ReadFile fails in order to make sure it can successfully process the character.
--	The character is then displayed on the screen in a green background.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI Read_From_Serial(LPVOID hwnd);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Write_To_Serial
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: BOOL Write_To_Serial(HDC	hdc,
--								   HWND hwnd);
--					-HDC hdc:	Device context
--					-HWND hwnd: Handle to the current window
-- RETURNS: 0 if the write operation to the serial port failed, else 1
--
-- NOTES:
--	Performs write operation on the currently opened serial port. Sends 1 character at a time and 
--	draw the character on the window with a yellow background
----------------------------------------------------------------------------------------------------------------------*/
BOOL Write_To_Serial(WPARAM wParam, HWND hwnd);
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: OutPut_GetLastError
--
-- DATE: September 28, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ruoqi Jia
--
-- PROGRAMMER: Ruoqi Jia
--
-- INTERFACE: BOOL OutPut_GetLastError();
--
-- RETURNS: VOID
--
-- NOTES:
--	Outputs a formatted string from the GetLastError() function onto the consol.
--	Mainly used for debugging
----------------------------------------------------------------------------------------------------------------------*/
VOID Output_GetLastError();
void locProcessCommError(DWORD dwError);
#endif

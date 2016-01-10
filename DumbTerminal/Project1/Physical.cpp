/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Physical.cpp - Actualy function implementation for Physical.h. Handles Physical layer of the program
--		dumb terminal emulator
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
--  reading inputs from the serial port will be created(Event-driven approach). Any characters read will be 
--  displayed in a colored background specified by the user (default to Green). On the other hand, the writing operation
--  is performed on the main thread of the program (Handled by WndProc). When a keypress is captured
--  and processed, the corresponding character will be displayed in a colored backgorund also specified
--  by the user.
--
--	Note that the specific features that handles character display and menuitems will be outlined in Session.h
----------------------------------------------------------------------------------------------------------------------*/

#include "Physical.h"
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
BOOL Initialize_Serial_Port()
{
	if (isConnected)
		return FALSE;
	if ((hComm = CreateFile(lpszCommName, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL))
		== INVALID_HANDLE_VALUE)	//Opens the serial port for asyncronous I/O
	{
		MessageBox(NULL, "Error opening COM port:", "", MB_OK);
		return FALSE;
	}
	return TRUE;
}

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
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
	WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		Handle_Menu_Commands(hwnd, wParam);	//Handles menuitem operations
		break;
	case WM_CHAR:							// Process keystroke
		if (isConnected)					//	If currently in connect mode
			Write_To_Serial(wParam, hwnd);
		break;
	case WM_PAINT:							//Process repaint 
			Repaint(hwnd);
		break;
	case WM_DESTROY:						// Terminate program
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}


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
BOOL Setup_Comm_Config(HWND hwnd)
{
	if (!Initialize_Serial_Port())
		return FALSE;
	COMMCONFIG cc;					//Configuration state of the serial port
	cc.dwSize = sizeof(COMMCONFIG);	//Set the size of the structure to default
	cc.wVersion = 0x100;			//Set the version number 
	if (!GetCommConfig(hComm, &cc, &cc.dwSize))		//Retrieves the current configuration of the serial port
		MessageBox(NULL, "Error Retriving COMMCONFIG:", "", MB_OK);
	if (!CommConfigDialog(lpszCommName, hwnd, &cc))	//Display configuration box 
		return FALSE;
	if (!SetCommState(hComm, &cc.dcb))				//Set cc to the current configuration for the serial port
		MessageBox(NULL, "Error Setting COMMCONFIG:", "", MB_OK);
	return TRUE;
}

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
--	Called by the CreateThread function. This function loops forever as long as the program is connected. This uses an
--  event-driven approach by having a event created when a character arrives to the port. This also checks that the overlapped
--	object that is being passed in the ReadFile function is in a signaled state. The character is then displayed on the screen 
--  in a colored background selected from the menu(default to green).
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI Read_From_Serial(LPVOID hwnd)
{
	DWORD read_byte, dwEvent, dwError;
	COMSTAT cs;
	char	str[10] = "";					//Character buffer for reading
	while (isConnected)
	{
		SetCommMask(hComm, EV_RXCHAR);			//Create an event when a character arrives
		HDC		hdc = GetDC((HWND)hwnd);		//Get device context
		if ((ov_read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)	//Create event for reading
			Output_GetLastError();				//Error checking
		if (WaitCommEvent(hComm, &dwEvent, NULL))				//Wait for the event to happen
		{
			ClearCommError(hComm, &dwError, &cs);				//Clear the communication port
			if ((dwEvent & EV_RXCHAR) && cs.cbInQue)
			{
				if (!ReadFile(hComm, str, 1, &read_byte, &ov_read))	//Attempt to read from port
					Output_GetLastError();							//Error checking
				Draw(str, hdc, read_color, FALSE, (HWND)hwnd);		//Display character
			}
				
		}else
			Output_GetLastError();								//Error Checking
	}

	PurgeComm(hComm, PURGE_RXCLEAR);			//Clean out the buffer 
	return 0;
}

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
--	draw the character on the window with a colored background selected from the menu(default to yellow)
----------------------------------------------------------------------------------------------------------------------*/
BOOL Write_To_Serial(WPARAM wParam, HWND hwnd)
{
	char	str[10];
	HDC		hdc = GetDC(hwnd);					//get device context
	sprintf_s(str, "%c", (char)wParam);			//Convert wParam to a string
	Draw((str), hdc, write_color, FALSE, hwnd);		//Display the string
	if ((ov_write.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)	//Create event for writing
		MessageBox(NULL, "Creating write event failed", "", MB_OK);
	if (!WriteFile(hComm, str, strlen(str), NULL, &ov_write))				//Attempt to write to the serial port
		return FALSE;
	CloseHandle(ov_write.hEvent);						//Release device context
	return TRUE;
}

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
--	Outputs a formatted string from the GetLastError() function onto the console.
--	Mainly used for debugging
----------------------------------------------------------------------------------------------------------------------*/
VOID Output_GetLastError()
{
	wchar_t buf[256];			//Buffer to the formatted error string
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);	//Format the DWORD into buffer
	OutputDebugStringW(buf);	//Output buffer 
}
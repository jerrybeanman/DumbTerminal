/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Globals.h - Contains global variables and neccessary header files for the dumb terminal emulator program
--
-- PROGRAM: Dumb terminal emulator
--
-- FUNCTIONS: NONE
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
--	This is a delegator script that contains all the header files and global variables for the main program.
----------------------------------------------------------------------------------------------------------------------*/

#ifndef GLOBALS_H
#define GLOBALS_H
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <windows.h>
#include <stdio.h>
#include "menu.h"
#include "Physical.h"
#include "Session.h"
struct Coordinates					//Contains the x and y coordinates that are used to draw text
{
	unsigned		_x = 0, _y = 0;	
	void Reset() { _x = _y = 0; }	//Set values back to origin
};
static HANDLE	rThread;					//Handle for the read thread
static DWORD	rThreadId;					//Stores the thread id
const	char		Name[] = "Dumb Terminal Emulator";	//Name of the program
const	LPCSTR		lpszCommName = "COM1";	//Port name
extern	HANDLE		hComm;					//Handle for the serial port
extern	BOOL		isConnected;		//Keep track of the current mode for the program
extern	COLORREF	write_color;	//Color to dispplay when writing 
extern	COLORREF	read_color;		//Color to display when reading
extern OVERLAPPED ov_read;
extern OVERLAPPED ov_write;
extern	Coordinates coor;						//Coorinate to display text on the current window
static	std::vector<std::pair<std::string, COLORREF>> rwHistory;	//Stores all I/O history
#endif

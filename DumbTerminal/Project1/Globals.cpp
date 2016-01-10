#include "Globals.h"
HANDLE		hComm;
Coordinates coor;
BOOL		isConnected = FALSE;	//The program is not connected when it starts
OVERLAPPED	ov_read		= { 0 };	//Initialize empty overlapped
OVERLAPPED	ov_write	= { 0 };
COLORREF	write_color = RGB(255, 255, 0);	//Initialize color to yellow
COLORREF	read_color	= RGB(0, 255, 0);	//Initialize color to green

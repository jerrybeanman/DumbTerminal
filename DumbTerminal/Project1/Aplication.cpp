/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Application.cpp - Main application for the program
--
-- PROGRAM: Dumb terminal emulator
--
-- FUNCTIONS:
--	int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
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
--	This application contains the WinMain, which acts as the main application for the Dumb Termulator Emulator program.
--	The program's main objective is to perform read and write operation asynchrounously using Event-Driven approach on 
--	a serial port by sending characters between two machines and display it correspondingly. Additionally a help menu will 
--  be availiable  and the program is able to switch between "Connect" and "Command" mode. 
--
--  Note: All functionalities for the program is throughly outlined in Physical.h/cpp and Session.h/cpp
----------------------------------------------------------------------------------------------------------------------*/
#include "Application.h"
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
	LPSTR lspszCmdParam, int nCmdShow)
{
	HWND hwnd;
	WNDCLASSEX wcl;
	MSG Msg;
	Initialize_Window(hInst, nCmdShow, hwnd, wcl);
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}
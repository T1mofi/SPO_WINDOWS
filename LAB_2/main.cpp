#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <vector>

using namespace std;

vector<HANDLE> exitEvetnsVector;
HANDLE FreeProcessorEvent;
char EventID[30];

void createPr()
{
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&prInfo, sizeof(prInfo));

	sprintf_s(EventID, "%d", exitEvetnsVector.size() + 1);
	exitEvetnsVector.push_back(CreateEvent(NULL,
		FALSE,
		FALSE,
		EventID));

	char commandLine[256];
	sprintf_s(commandLine, "CHILD.exe %s", EventID);

	if (!CreateProcess(NULL,       
		commandLine,       
		NULL,       
		NULL,       
		FALSE,      
		NULL,       
		NULL,       
		NULL,       
		&stInfo,    
		&prInfo    
	)){
		return;
	}

	CloseHandle(prInfo.hProcess);
}

void closePr()
{
	if (exitEvetnsVector.size() > 0) {
		//WaitForSingleObject(FreeProcessorEvent, INFINITE);
		
		SetEvent(exitEvetnsVector.back());
		CloseHandle(exitEvetnsVector.back());
		exitEvetnsVector.pop_back();
	}
}

void closeAllPr()
{
	while (exitEvetnsVector.size() > 0) {
		closePr();
	}
}


int main(int argc, char* argv[]) {
	FreeProcessorEvent = CreateEvent(
		NULL,        //security
		FALSE,       //auto reset
		TRUE,        //signal state
		"FreeProcessor"    
	);

	while (true)
	{	
		switch (_getch())
		{
		case '=':
			createPr();
			break;
		case '-':
			closePr();
			break;
		case 'q':
			closeAllPr();
			system("pause");
			return 0;
		}
	}
}
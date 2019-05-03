#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <vector>

using namespace std;

vector<HANDLE> exitSemaphoresVector;
char SemaphoreID[30];

void createPr()
{
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&prInfo, sizeof(prInfo));

	sprintf_s(SemaphoreID, "%d", exitSemaphoresVector.size() + 1);
	exitSemaphoresVector.push_back(CreateSemaphore(NULL, 0, 1, SemaphoreID));

	char commandLine[256];
	sprintf_s(commandLine, "Child.exe %s", SemaphoreID);

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
	)) {
		return;
	}

	CloseHandle(prInfo.hProcess);
}

void closePr()
{
	if (exitSemaphoresVector.size() > 0) {
		ReleaseSemaphore(exitSemaphoresVector.back(), 1, NULL);
		CloseHandle(exitSemaphoresVector.back());
		exitSemaphoresVector.pop_back();
	}
}

void closeAllPr()
{
	while (exitSemaphoresVector.size() > 0) {
		closePr();
	}
}


int main(int argc, char* argv[]) {

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
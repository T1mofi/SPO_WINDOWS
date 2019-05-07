#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <vector>

using namespace std;

vector<HANDLE> exitSemaphoresVector;
HANDLE closeServer = CreateSemaphore(NULL, 0, 1, "closeServer");

void createServer()
{
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&prInfo, sizeof(prInfo));

	char commandLine[256];
	sprintf_s(commandLine, "Parent.exe");

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
	CloseHandle(prInfo.hThread);
}


void createClient()
{
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&prInfo, sizeof(prInfo));

	char semaphoreID[30];

	sprintf_s(semaphoreID, "%d", exitSemaphoresVector.size() + 1);
	exitSemaphoresVector.push_back(CreateSemaphore(NULL, 0, 1, semaphoreID));

	char commandLine[256];
	sprintf_s(commandLine, "Child.exe %s", semaphoreID);

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

void closeClient()
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
		closeClient();
	}

	ReleaseSemaphore(closeServer, 1, NULL);
}


int main(int argc, char* argv[]) {

	createServer();

	while (true)
	{
		switch (_getch())
		{
		case '=':
			createClient();
			break;
		case '-':
			closeClient();
			break;
		case 'q':
			closeAllPr();
			CloseHandle(closeServer);
			return 0;
		}
	}
}
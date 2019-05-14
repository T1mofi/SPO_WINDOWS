#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>

using namespace std;

void createManager() {
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;
	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&prInfo, sizeof(prInfo));

	char commandLine[256];
	sprintf_s(commandLine, "Manager.exe");

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


int main(int argc, char* argv[])
{

	createManager();

	char string[40];
	
	HANDLE closeServer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "closeServer");
	HANDLE canPrint = CreateSemaphore(NULL, 0, 1, "Print");
	HANDLE canWrite = CreateSemaphore(NULL, 0, 1, "Write");
	
	HANDLE conn = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		40,
		"FileMap");

	LPVOID buff = MapViewOfFile(conn,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		40);

	while (true) {
		ReleaseSemaphore(canWrite, 1, NULL);
		WaitForSingleObject(canPrint, INFINITE);

		memset(string, '\0', 40);
		strncpy(string, (char*)buff, 40);

		for (int i = 0; i < strlen(string); i++)
		{
			cout << string[i];
			Sleep(300);
		}

		cout << endl;

		if (WaitForSingleObject(closeServer, 0) == WAIT_OBJECT_0) {
			break;
		}
	}

	CloseHandle(closeServer);
	CloseHandle(canPrint);
	CloseHandle(canWrite);
	UnmapViewOfFile(buff);
	CloseHandle(conn);

	return 0;
}

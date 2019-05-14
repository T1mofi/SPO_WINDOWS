#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <ctime>

using namespace std;

HANDLE hMutex;
char semaphoreID[30];


void* Thread(void* pParams)
{
	HANDLE closeClient = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, semaphoreID);
	HANDLE canPrint = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Print");
	HANDLE canWrite = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Write");

	HANDLE conn = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		"FileMap");

	LPVOID buff = MapViewOfFile(conn,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		40);

	char emptyString[40];
	memset(emptyString, '\0', 40);

	char * uniqueString = (char*)calloc(40, 1);
	uniqueString = _itoa(GetCurrentThreadId(), uniqueString, 10);

	while (true) {

		WaitForSingleObject(canWrite, INFINITE);

		if (WaitForSingleObject(closeClient, 0) == WAIT_OBJECT_0)
		{
			ReleaseSemaphore(canWrite, 1, NULL);
			break;
		}

		WaitForSingleObject(hMutex, INFINITE);
		CopyMemory((PVOID)buff, emptyString, 40);
		CopyMemory((PVOID)buff, uniqueString, 40);
		ReleaseMutex(hMutex);

		ReleaseSemaphore(canPrint, 1, NULL);

	}

	CloseHandle(closeClient);
	CloseHandle(canPrint);
	CloseHandle(canWrite);
	UnmapViewOfFile(buff);
	CloseHandle(conn);

	return 0;
}


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

	sprintf_s(semaphoreID, "%d", exitSemaphoresVector.size() + 1);
	exitSemaphoresVector.push_back(CreateSemaphore(NULL, 0, 1, semaphoreID));

	HANDLE hThr;

	if ((hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, NULL, 0, NULL)) == 0) {
		cout << "Thread create error" << endl;
	}

	CloseHandle(hThr);
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
	
	hMutex = CreateMutex(NULL, FALSE, NULL);

	/*createServer();*/
	
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
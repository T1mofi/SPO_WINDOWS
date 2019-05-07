#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	char semaphoreID[30];
	strcpy(semaphoreID, argv[1]);

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
	uniqueString = _itoa(GetCurrentProcessId(), uniqueString, 10);

	while (true) {

		WaitForSingleObject(canWrite, INFINITE);

		if (WaitForSingleObject(closeClient, 0) == WAIT_OBJECT_0)
		{
			ReleaseSemaphore(canWrite, 1, NULL);
			break;
		}

		CopyMemory((PVOID)buff, emptyString, 40);
		CopyMemory((PVOID)buff, uniqueString, 40);
		ReleaseSemaphore(canPrint, 1, NULL);

	}

	CloseHandle(closeClient);
	CloseHandle(canPrint);
	CloseHandle(canWrite);
	UnmapViewOfFile(buff);
	CloseHandle(conn);

	return 0;
}

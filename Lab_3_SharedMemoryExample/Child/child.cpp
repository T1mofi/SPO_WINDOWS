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
	HANDLE close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, semaphoreID);

	HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Print");
	HANDLE reset = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Reset");
	

	HANDLE conn = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		"FileMap");

	LPVOID buff = MapViewOfFile(conn,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		40);

	char emptyString[40] = "";
	char * stri = (char*)calloc(40, 1);
	stri = _itoa(GetCurrentProcessId(), stri, 10);

	while (true) {

		for (int i = 0; i < 3; ++i)
		{
			CopyMemory((PVOID)buff, stri, 40);
			ReleaseSemaphore(print, 1, NULL);
			WaitForSingleObject(reset, INFINITE);
		}

		CopyMemory((PVOID)buff, emptyString, 40);
		ReleaseSemaphore(print, 1, NULL);
		WaitForSingleObject(reset, INFINITE);

		if (WaitForSingleObject(close, 0) == WAIT_OBJECT_0)
		{
			CloseHandle(close);
			UnmapViewOfFile(buff);
			CloseHandle(conn);

			return 0;
		}
	}

	

	return 0;
}

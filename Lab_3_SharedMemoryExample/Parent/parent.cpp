#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>

using namespace std;


int main(int argc, char* argv[])
{
	char string[40];

	HANDLE print = CreateSemaphore(NULL, 0, 1, "Print");
	HANDLE reset = CreateSemaphore(NULL, 0, 1, "Reset");
	
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
		WaitForSingleObject(print, INFINITE);

		cout << (char*)buff << endl;
		Sleep(1000);

		ReleaseSemaphore(reset, 1, NULL);
	}


	UnmapViewOfFile(buff);
	CloseHandle(conn);

	return 0;
}

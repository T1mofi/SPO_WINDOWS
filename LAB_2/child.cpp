#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	char eventID[30];
	strcpy(eventID, argv[1]);
	HANDLE exitEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventID);
	HANDLE FreeProcessorEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "FreeProcessor");
	char *message = const_cast<char*>("Number of process ");
	while (true)
	{
		if (WaitForSingleObject(exitEvent, 0) == WAIT_OBJECT_0)
		{
			CloseHandle(exitEvent);
			system("cls");
			SetEvent(FreeProcessorEvent);
			return 0;
		}

		if (WaitForSingleObject(FreeProcessorEvent, 1) == WAIT_OBJECT_0)
		{
			for (int i = 0; i < 18; ++i) {
				if (WaitForSingleObject(exitEvent, 0) == WAIT_OBJECT_0)
				{
					CloseHandle(exitEvent);
					system("cls");
					SetEvent(FreeProcessorEvent);
					return 0;
				}
				cout << message[i];
				Sleep(100);
			}
			cout << GetCurrentProcessId() << endl;
			SetEvent(FreeProcessorEvent);
		}
	}
	return 0;
}
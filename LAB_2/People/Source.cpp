#include <iostream>
#include <Windows.h>
#include <chrono>
#include <ctime>   

#define GOOD 0
#define LATTER 1
#define PARCEL 2
#define MONEY 3
#define LIMIT 4


using namespace std;

void sendSomething(int TYPE) {
	cout << "How much money can you give? ";
	int money = 0; 
	cin >> money;
	cout << endl;

	char sum[10];
	_itoa(money, sum, 10);
	char str[80];
	ZeroMemory(str, sizeof(char) * 80);
	strcat(str, "mail");
	strcat(str, " ");
	strcat(str, sum);
	strcat(str, " ");
	char type[2];
	_itoa(TYPE, type, 10);
	strcat(str, type);

	STARTUPINFO startupInfo;						// инфа о текущем окружении для создания нового процесса, ну точнее её здесь пока нет(((
	PROCESS_INFORMATION procInfo;                   // тут будет инфа о новом процессе 
	ZeroMemory(&startupInfo, sizeof(startupInfo));  // заполняет память по адресу нулями
	startupInfo.cb = sizeof(startupInfo);			// установка размера структуры StartupInfo
	ZeroMemory(&procInfo, sizeof(procInfo));

	if (!CreateProcess(NULL,
		str,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&startupInfo,
		&procInfo)) {
		cout << "Postal office is closed at the moment (((" << endl;
	};

	WaitForSingleObject(procInfo.hProcess, INFINITE);
	DWORD exitCode = 0;
	GetExitCodeProcess(procInfo.hProcess, &exitCode);
	if (exitCode == GOOD) {
		cout << "Congratulations! It is a success! " << endl << endl;
		
	}
	else if (exitCode == LIMIT) {
		cout << "Sorry, the mail is overloaded." << endl << endl;
	}
	else if (exitCode == MONEY) {
		cout << "Insufficient funds. " << endl << endl;
	}
	else {
		cout << "Billy, all is lost..." << endl << endl;
	}
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
}

int main(int argc, char* argv[]) {
	HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MyEvent");
	while (true)
	{
		WaitForSingleObject(Event, INFINITE);

		cout << GetCurrentProcessId() << endl;

		cout << "Please, select an action:" << endl;
		cout << "1) Send a letter" << endl;
		cout << "2) Send a parcel" << endl;
		cout << "3) Nothing" << endl;
		cout << "Your choice: ";
		int choise = 0;
		cin >> choise;
		cout << endl;
		switch (choise)
		{
		case 1: {
			sendSomething(LATTER);
			break;
		}
		case 2: {
			sendSomething(PARCEL);
			break;
		}
		case 3: {
			cout << "After all that was between us? (((" << endl;
			break;
		}
		default:
			cout << "PIZDEC NAHOI BLYAT!!! Wipe your eyes!!!" << endl;
			break;
		}
		Sleep(1000);
		system("cls");
		SetEvent(Event);
	}
}
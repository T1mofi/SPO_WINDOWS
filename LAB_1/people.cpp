#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <ctime>   

#include "C:\Users\T1mofi\Desktop\SPO\Lab1_postoffice\Postoffice\mail\constants.h"



using namespace std;

int postRequest(string departureType, string money);
string decryptExitCode(int exitCode);

int main(int argc, char* argv[]) {

	while (true) {
		cout << "Please, select an action:" << endl;
		cout << "1) Send a letter(10 BYN)." << endl;
		cout << "2) Send a parcel(50 BYN)." << endl;
		cout << "3) End work." << endl << endl;
		cout << "Your choice: ";
		int choice = 0;
		cin >> choice;
		cout << endl;

		string departureType;

		switch (choice)
		{
			case 1: {
				departureType = "letter";
				break;
			}
			case 2: {
				departureType = "parcel";
				break;
			}
			case 3: {
				cout << "Goodbye!" << endl;
				return 0;
			}
			default:
				cout << "Goodbye!" << endl;
				return 0;
		}

		cout << "Please, deposit money: ";
		string money;
		cin >> money;

		int exitCode = postRequest(departureType, money);
		cout << decryptExitCode(exitCode);
	
		cin.get();
		char c = 0;
		while (c != 10) {
			c = cin.get();
		}
		system("cls");
	}
	return 0;
}


int postRequest(string departureType, string money) {
	
	string commandLine;
	commandLine = "mail " + money + " " + departureType + " ";

	STARTUPINFO StartupInfo;						
	PROCESS_INFORMATION ProcInfo;                   
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));  
	StartupInfo.cb = sizeof(StartupInfo);
	ZeroMemory(&ProcInfo, sizeof(ProcInfo));

	if (!CreateProcess(NULL,
		(char*)commandLine.c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&StartupInfo,
		&ProcInfo)) {
		cout << "Create process error." << endl;
	};

	SYSTEMTIME lt;

	while (WaitForSingleObject(ProcInfo.hProcess, 50)) {
		GetLocalTime(&lt);
		printf("%02d:%02d:%02d p\r", lt.wHour, lt.wMinute, lt.wSecond);
	}

	DWORD exitCode = 0;
	GetExitCodeProcess(ProcInfo.hProcess, &exitCode);

	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);

	return exitCode;
}

string decryptExitCode(int exitCode) {

	switch (exitCode) {
	case 0:
		return "Departure sended.";

	case LACK_MONEY:
		return "Not enough money.";

	case EXCEED_DEPARTURE_LIMIT:
		return "Exceed departure limit.";

	default:
		return "Something wrong.";
	}
}

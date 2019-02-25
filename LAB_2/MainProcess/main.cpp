#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <malloc.h>

using namespace std;
HANDLE Event;
vector<HANDLE> vec;

void createPr()
{

	char *str = const_cast<char*>("people.exe");
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;

	ZeroMemory(&stInfo, sizeof(STARTUPINFO));
	stInfo.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL,       // нмя исполняемого модуля
		str,        // командная строка
		NULL,       // атрибуты защиты процесса
		NULL,       // атрибуты защиты потока
		FALSE,      // признак наследования дескрипторов
		CREATE_NEW_CONSOLE,       // флаги создания процесса
		NULL,       // блок новой среды окружения
		NULL,       // текущий каталог
		&stInfo,    // вид главного окна
		&prInfo     // информация о процессе
	))
	{
		exit(0);
	}

	vec.push_back(prInfo.hProcess);
}

void closePr()
{
	//HANDLE Event = CreateEvent(NULL,FALSE,FALSE,"MyEvent");
	//HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MyEvent");

	if (Event == NULL)
		exit(0);

	WaitForSingleObject(Event, INFINITE);

	TerminateProcess(vec[vec.size() - 1], 0);
	vec.pop_back();

	SetEvent(Event);
}

void closeAllPr()
{
	//HANDLE Event = CreateEvent(NULL,FALSE,FALSE,"MyEvent");
	//HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MyEvent");

	if (Event == NULL)
		exit(0);
	WaitForSingleObject(Event, INFINITE);

	while (vec.size() != 0)
	{
		TerminateProcess(vec[vec.size() - 1], 0);
		vec.pop_back();
	}
}

int main(int argc, char *argv[])
{
        Event = CreateEvent(NULL,        // атрибут защиты
		FALSE,       // тип события ( TRUE -> ручной сброс )
		FALSE,       // начальное состояние события ( TRUE -> нач. состояние сигнальное )
		"MyEvent"    // имя события
	);

	if (Event != NULL){
		Sleep(1000);
		SetEvent(Event);
	}
	else
	{
		return GetLastError();
	}
	while (true)
	{
		char var = ' ';
		cout << "+/-/q : " << endl;
		cin >> var;
		switch (var)
		{
		case '+': {
			createPr();
			break;
		}
		case '-': {
			if (vec.size() != 0)
				closePr();
			else
				cout << "Clear" << endl;
			break;
		}
		case 'q': {
			closeAllPr();
			return 0;
		}
				  break;
		}
	}
	system("pause");
	return 0;
}
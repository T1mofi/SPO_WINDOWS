#include <iostream>
#include <fstream>
#include <Windows.h>

#define GOOD 0
#define LATTER 1
#define PARCEL 2
#define MONEY 3
#define LIMIT 4

using namespace std;

int main(int argc, char* argv[]) {
	cout << "I was born" << endl;

	fstream base("..\\mail\\base.txt", ios::in | ios::out);
	if (!base)
		return -1;

	int latter;
	int parcel;
	base >> latter;
	base >> parcel;
	base.close();

	int sum = atoi(argv[1]);
	if (atoi(argv[2]) == LATTER)
		if(sum>10)
			if (latter < 10)
				latter++;
			else {
				system("pause");
				exit(LIMIT);
			}
		else {
			system("pause");
			exit(MONEY);
		}

	if (atoi(argv[2]) == PARCEL)
		if (sum > 50)
			if (parcel < 5)
				parcel++;
			else {
				system("pause");
				exit(LIMIT);
			}
		else {
			system("pause");
			exit(MONEY);
		}

	base.open("..\\mail\\base.txt");
	if (!base)
		return - 1;
	base << latter << " " << parcel;
	base.close();

	system("pause");
	return GOOD;
}
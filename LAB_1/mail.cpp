#include <iostream>
#include <string.h>
#include <fstream>
#include <Windows.h>

#include "C:\Users\T1mofi\Desktop\SPO\Lab1_postoffice\Postoffice\mail\constants.h"


using namespace std;

void waitExit();

int main(int argc, char* argv[]) {

	cout << "Departure processing..." << endl;
	string filePath = "C:\\Users\\T1mofi\\Desktop\\SPO\\Lab1_postoffice\\Postoffice\\mail\\PostStatus.txt";
	fstream file(filePath, ios::in);
	if (!file) {
		return -1;
	}

	int quantitySendLetters;
	int quantitySendParcels;
	int credit;

	file >> quantitySendLetters
	     >> quantitySendParcels
	     >> credit;

	file.close();

    cout << "You deposit: " << argv[1] << endl;
	credit += atoi(argv[1]);
    cout << "Credit: " << credit << endl;
	string departureType = argv[2];

	if (departureType == "letter") {
		if (quantitySendLetters < 10) {
			if (credit >= 10) {
				quantitySendLetters++;
				credit -= 10;
				cout << "Sending letter..." << endl;
			} else {
				waitExit();
				return LACK_MONEY;
			}
		} else {
			waitExit();
			return EXCEED_DEPARTURE_LIMIT;
		}
	}
	if (departureType == "parcel") {
		if (quantitySendParcels < 5) {
			if (credit >= 50) {
				quantitySendParcels++;
				credit -= 50;
				cout << "Sending parcel..." << endl;
			} else {
				waitExit();
				return LACK_MONEY;
			}
		} else {
			waitExit();
			return EXCEED_DEPARTURE_LIMIT;
		}
	}

	file.open(filePath, ios::out);
	if (!file) {
		return -1;
	}

	file << quantitySendLetters << " " 
		 << quantitySendParcels << " " 
		 << credit;

	file.close();

	waitExit();

	return 0;
}

void waitExit() {
	cout << "End processing" << endl;
	cout << "Press 'Q' for exit";

	char c = 0;
	while (c != 'q') {
		c = cin.get();
	}
}

/*
Program:	BMC stats for league of legends
Author:		Bmckalip
Version:	0.1
*/

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <ctime>
#include <string>
#include <map>
#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>

#define MINIMUM_GAME_LENGTH_MINUTES 20
using namespace std;

void keyPressed(int key);
void updateScreen();
void reset();
int sumActions();
void saveGame();

map <int, short> currentState;
map <int, short> previousState;
map <int, int>   actionCounter;
map <int, int>::iterator it;

unsigned long spaceHeld = 0, loopCount = 1;
double spacePercent = 0.0;

time_t startTime, updateTime;
int secondsElapsed;

int main() {
	reset();

	while (1) {
		loopCount++;

		keyPressed(VK_LBUTTON);
		keyPressed(VK_RBUTTON);
		keyPressed(0x51);
		keyPressed(0x57);
		keyPressed(0x45);
		keyPressed(0x52);
		keyPressed(0x44);
		keyPressed(0x46);
		keyPressed(0x31);
		keyPressed(0x32);
		keyPressed(0x33);
		keyPressed(0x34);
		keyPressed(0x35);
		keyPressed(0x36);
		keyPressed(0x37);
		keyPressed(VK_F1);
		keyPressed(VK_F2);
		keyPressed(VK_F3);
		keyPressed(VK_F4);
		keyPressed(VK_F5);
		keyPressed(VK_TAB);
		keyPressed(0x41);
		keyPressed(0x53);
		//pause
		keyPressed(VK_DELETE);
		//reset
		keyPressed(VK_END);

		if (GetAsyncKeyState(VK_SPACE)) {
			spaceHeld++;
		}

		//if one second has passed since updateScreen() has been called, call it.
		if (difftime(time(0), updateTime) > 0) {
			updateScreen();
		}
		Sleep(50);
	}
	return 0;
}

void keyPressed(int key) {
	if (GetAsyncKeyState(key)) {
		currentState[key] = 1;
	}else {
		currentState[key] = 0;
	}

	if ((previousState[key] != currentState[key])) {
		if (currentState[key]) {
			switch(key) {
				case VK_DELETE:
					cout << "Paused - Press Escape to unpause";
					while (1) {
						if ((GetKeyState(VK_ESCAPE) & 0x8000) != 0) {
							startTime = time(0) - secondsElapsed;
							break;
						}
					}
					break;

				case VK_END:
					cout << "Press ENTER to Save and Reset, or ESCAPE to cancel" << endl;
					while (1) {
						startTime = time(0) - secondsElapsed;
						if ((GetKeyState(VK_RETURN) & 0x8000) != 0) {
							if (secondsElapsed >= (60 * MINIMUM_GAME_LENGTH_MINUTES)) {
								saveGame();
								
							}
							else {
								cout << "Game must be at least " << MINIMUM_GAME_LENGTH_MINUTES << " minutes long to save" << endl;
								Sleep(3000);
							}
							reset();
							break;
						}
						else if ((GetKeyState(VK_ESCAPE) & 0x8000) != 0) {
							break;
						}
						startTime = time(0) - secondsElapsed;
					}
					break;
				default:
					actionCounter[key]++;
					updateScreen();
			}
		}
			previousState[key] = currentState[key];	
	}
}

void updateScreen() {
	time(&updateTime);
	secondsElapsed = difftime(updateTime, startTime);
	
	int actionCount;
	double minutesElapsed = secondsElapsed / 60.0;
	actionCount = sumActions();
	int apm = ((minutesElapsed != 0) ? (actionCount / minutesElapsed) : 0);
	spacePercent = ((double)spaceHeld / loopCount) * 100;

	system("CLS");
	
	cout << "       Welcome to BMC Stats for League of Legends!" << endl;

	cout << "-----------------------------------------------------------------" << endl;
	cout << "Locked Screen:\t" << (int)spacePercent << "%\t|\t" << "Clock:\t" << (secondsElapsed / 3600) << ':' << (secondsElapsed / 60) << ':' << secondsElapsed % 60 << endl;
	cout << "-----------------------------------------------------------------" << endl;
	cout << "Left Click:\t"    << actionCounter[VK_LBUTTON]	<< "\t|\t" << "D:\t"   << actionCounter[0x44] << endl;
	cout << "Right Click:\t"   << actionCounter[VK_RBUTTON]	<< "\t|\t" << "F:\t"   << actionCounter[0x46] << endl;
	cout << "\t\t\t|"																    << endl;
	cout << "Q:\t\t"		   << actionCounter[0x51] << "\t|\t" << "F1:\t"  << actionCounter[VK_F1] << endl;
	cout << "W:\t\t"		   << actionCounter[0x57] << "\t|\t" << "F2:\t"  << actionCounter[VK_F2] << endl;
	cout << "E:\t\t"		   << actionCounter[0x45] << "\t|\t" << "F3:\t"  << actionCounter[VK_F3] << endl;
	cout << "R:\t\t"		   << actionCounter[0x52] << "\t|\t" << "F4:\t"  << actionCounter[VK_F4] << endl;
	
	cout << "\t\t\t|\tF5:\t"   << actionCounter[VK_F5]											      << endl;
	cout << "1:\t\t"		   << actionCounter[0x31] << "\t|\t"									  << endl;
	cout << "2:\t\t"		   << actionCounter[0x32] << "\t|\t" << "Tab:\t" << actionCounter[VK_TAB] << endl;
	cout << "3:\t\t"		   << actionCounter[0x33] << "\t|\t" << "A:\t"   << actionCounter[0x41]	  << endl;
	cout << "4:\t\t"		   << actionCounter[0x34] << "\t|\t" << "S:\t"	 <<	actionCounter[0x53]	  << endl;
	cout << "5:\t\t"		   << actionCounter[0x35] << "\t|\t"									  << endl;
	cout << "6:\t\t"		   << actionCounter[0x36] << "\t|\t"									  << endl;
	cout << "7:\t\t"		   << actionCounter[0x37] << "\t|\t"									  << endl;
	cout << "-----------------------------------------------------------------" << endl;
	cout << "Total Actions:\t" << actionCount << "\t|\t";
	cout << "APM:\t" << apm << endl;
	cout << "-----------------------------------------------------------------" << endl;

	cout << "Press DELETE to Pause   | Press END to save and reset the Counter" << endl;

}

void reset() {
	time(&startTime);
	secondsElapsed = 1;
	spaceHeld = 0;
	loopCount = 1;
	for (it = actionCounter.begin(); it != actionCounter.end(); ++it) {
		it->second = 0;
	}
	updateScreen();
	return;
}

int sumActions() {
	int actionCount = 0;
	
	for ( it = actionCounter.begin(); it != actionCounter.end(); ++it) {
		actionCount += it->second;
	}
	return actionCount;
}

void saveGame() {
	time(&updateTime);
	secondsElapsed = difftime(updateTime, startTime);

	// convert now to tm struct for UTC
	struct tm localTime;
	localtime_s(&localTime, &updateTime);
	char timestamp[50], filename[50];

	strftime(timestamp, 50, "%c", &localTime);
	strftime(filename, 50, "Game Stats/%m-%d-%Y_Game#", &localTime);
	//strcat_s(filename, ".txt");
	ofstream outputFile;		
	
	//outputFile.open(filename);
	
	int gameNumber = 1;
	string gameNumberChar;
		
	while (1) {
		//convert int to char*
		gameNumberChar = to_string(gameNumber);

		//check if the file exists. If not, create it.
		ifstream infile(filename + gameNumberChar + ".txt");
		if (!infile) {
			outputFile.open(filename + gameNumberChar + ".txt");
			break;
		}else {
			gameNumber++;
		}	
	}
	
	int actionCount;
	double minutesElapsed = secondsElapsed / 60.0;
	actionCount = sumActions();
	int apm = ((minutesElapsed != 0) ? (actionCount / minutesElapsed) : 0);
	spacePercent = ((double)spaceHeld / loopCount) * 100;
	string clock = to_string(secondsElapsed / 3600) + ':' + to_string(secondsElapsed / 60) + ':' + to_string(secondsElapsed % 60);

	outputFile << "Timestamp:\t" << timestamp << endl;
	outputFile << "Game Length:\t" << clock << endl;
	outputFile << "Locked Screen:\t" << (int)spacePercent << endl;
	outputFile << "Left Click:\t\t" << actionCounter[VK_LBUTTON] << endl;
	outputFile << "Right Click:\t" << actionCounter[VK_RBUTTON] << endl << endl;

	outputFile << "Q:\t\t" << actionCounter[0x51] << endl;
	outputFile << "W:\t\t" << actionCounter[0x57] << endl;
	outputFile << "E:\t\t" << actionCounter[0x45] << endl;
	outputFile << "R:\t\t" << actionCounter[0x52] << endl;
	outputFile << "D:\t\t" << actionCounter[0x44] << endl;
	outputFile << "F:\t\t" << actionCounter[0x46] << endl;
	outputFile << "1:\t\t" << actionCounter[0x31] << endl;
	outputFile << "2:\t\t" << actionCounter[0x32] << endl;
	outputFile << "3:\t\t" << actionCounter[0x33] << endl;
	outputFile << "4:\t\t" << actionCounter[0x34] << endl;
	outputFile << "5:\t\t" << actionCounter[0x35] << endl;
	outputFile << "6:\t\t" << actionCounter[0x36] << endl;
	outputFile << "7:\t\t" << actionCounter[0x37] << endl;
	outputFile << "A:\t\t" << actionCounter[0x41] << endl;
	outputFile << "S:\t\t" << actionCounter[0x53] << endl;
	outputFile << "Tab:\t" << actionCounter[VK_TAB] << endl;
	outputFile << "F1:\t\t" << actionCounter[VK_F1] << endl;
	outputFile << "F2:\t\t" << actionCounter[VK_F2] << endl;
	outputFile << "F3:\t\t" << actionCounter[VK_F3] << endl;
	outputFile << "F4:\t\t" << actionCounter[VK_F4] << endl;
	outputFile << "F5:\t\t" << actionCounter[VK_F5] << endl<< endl;

	outputFile << "Total:\t" << actionCount << endl;
	outputFile << "APM:\t" << apm << endl;

	outputFile.close();

}
#include "stdafx.h"
#include <algorithm>
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
#include <vector>
#include <regex>

#include "Display.h"
#include "KeyPress.h"
#include "Config.h"

KeyPress::KeyPress(Config config, vector<int> keyCodes, string name) {
	mainSettings = config;

	//record the time the object is created
	startTime = time(0);
	loopCount = 1;
	spaceHeld = 1;
	secondsElapsed = 0;
	reset = false;
	keyName = name;
	registerKeys(keyCodes);
}

bool KeyPress::isPressed(int key) {

	//continue only if the key is being pressed, not held.
	if (GetAsyncKeyState(key)) {
		keyState[key]["current"] = 1;
	} else {
		keyState[key]["current"] = 0;
		keyState[key]["previous"] = 0;
		return false;
	}

	if (keyState[key]["current"] && !keyState[key]["previous"]) {
		//check if the object is tracking this key
		if (!containsKey(key)) 	return false;

		switch (key) {
		case VK_DELETE:
			pauseLoop();
			break;
		case VK_DOWN:
			break;
		case VK_UP:
			break;
		case VK_RETURN:
			break;
		case VK_END:
			requestReset();
			break;
		default:
			actionCounter[key]++;
			latestKeyPressTime = time(0);
		}
		
		keyState[key]["previous"] = keyState[key]["current"];
		return true;
	}
	return false;
}

void KeyPress::isHeld(int key) {
	if (GetAsyncKeyState(key)) {
		spaceHeld++;
	} else {
		spaceHeldTime = time(0);
	}
}

string KeyPress::getActionCount(int keyName) {
	return to_string(actionCounter[keyName]);
}

string KeyPress::getTotalActionCount() {
	for (auto const &count : actionCounter) {
		totalActionCount += count.second;
	}
	return to_string(totalActionCount);
}

string KeyPress::getApm() {
	int apm = (int)(60 * totalActionCount / (difftime(time(0), startTime)));

	if (apm >= 0) {
		return to_string(apm);
	} else {
		return "0";
	}
}

bool KeyPress::timedUpdate(int seconds) {
	//triggers an update after x # of seconds have passed
	if(difftime(time(0), latestKeyPressTime) >= seconds) {
		latestKeyPressTime = time(0);
		return true;
	}
	return false;
}


string KeyPress::getElapsedTime() {
	
	int secondsElapsed = difftime(time(0), startTime);
	string timestamp =  to_string(secondsElapsed / 3600) + ":" + 
						to_string(secondsElapsed / 60) + ":" +
						to_string(secondsElapsed % 60);
	return timestamp;
}

string KeyPress::getLockedScreenPercent() {
	return to_string((int)(100 * ((float)spaceHeld / (float)loopCount)));
}

void KeyPress::incrementLoopCount() {
	loopCount++;
}

int KeyPress::getSecondsLocked() {
   return difftime(time(0), spaceHeldTime);
}

void KeyPress::requestReset() {
	reset = true;
}

bool KeyPress::checkIfRequiresReset() {
	return reset;
}

void KeyPress::saveToFile() {

	//create timestamps
	time_t now = time(0);
	// convert now to tm struct for UTC
	struct tm localTime;
	localtime_s(&localTime, &now);
	char timestamp[50], filename[50];
	strftime(timestamp, 50, "%c", &localTime);
	strftime(filename, 50, "Game Stats/%m-%d-%Y_Game#", &localTime);
	
	//intiate required variables for output file
	ofstream outputFile;
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
		} else {
			gameNumber++;
		}
	}

	//insert data into output file
	string actionCount = getTotalActionCount();
	string apm = getApm();
	string spacePercent = getLockedScreenPercent();
	string clock = getElapsedTime();

	outputFile << "Timestamp:\t" << timestamp << endl;
	outputFile << "Game Length:\t" << clock << endl;
	outputFile << "Locked Screen:\t" << spacePercent << endl;
	outputFile << "Left Click:\t\t" << getActionCount(VK_LBUTTON) << endl;
	outputFile << "Right Click:\t" << getActionCount(VK_RBUTTON) << endl << endl;

	outputFile << "Q:\t\t" << getActionCount(0x51) << endl;
	outputFile << "W:\t\t" << getActionCount(0x57) << endl;
	outputFile << "E:\t\t" << getActionCount(0x45) << endl;
	outputFile << "R:\t\t" << getActionCount(0x52) << endl;
	outputFile << "D:\t\t" << getActionCount(0x44) << endl;
	outputFile << "F:\t\t" << getActionCount(0x46) << endl;
	outputFile << "1:\t\t" << getActionCount(0x31) << endl;
	outputFile << "2:\t\t" << getActionCount(0x32) << endl;
	outputFile << "3:\t\t" << getActionCount(0x33) << endl;
	outputFile << "4:\t\t" << getActionCount(0x34) << endl;
	outputFile << "5:\t\t" << getActionCount(0x35) << endl;
	outputFile << "6:\t\t" << getActionCount(0x36) << endl;
	outputFile << "7:\t\t" << getActionCount(0x37) << endl;
	outputFile << "A:\t\t" << getActionCount(0x41) << endl;
	outputFile << "S:\t\t" << getActionCount(0x53) << endl;
	outputFile << "Tab:\t" << getActionCount(VK_TAB) << endl;
	outputFile << "F1:\t\t" << getActionCount(VK_F1) << endl;
	outputFile << "F2:\t\t" << getActionCount(VK_F2) << endl;
	outputFile << "F3:\t\t" << getActionCount(VK_F3) << endl;
	outputFile << "F4:\t\t" << getActionCount(VK_F4) << endl;
	outputFile << "F5:\t\t" << getActionCount(VK_F5) << endl << endl;

	outputFile << "Total:\t" << actionCount << endl;
	outputFile << "APM:\t" << apm << endl;

	//close the file
	outputFile.close();

}

void KeyPress::pauseLoop() {
	cout << "Paused - Press PAGE UP to Save and Reset, or PAGE DOWN to unpause" << endl;
	//track how long the loop has been paused
	pausedTime = time(0);
	while (1) {
		startTime += time(0) - pausedTime;
		if ((GetKeyState(VK_PRIOR) & 0x8000) != 0) {
			if ((difftime(time(0), startTime) / 60) >= mainSettings.getSetting("minGameLengthMinutes", 1)) {
				saveToFile();
				cout << "Saving . . .";
				Sleep(2000);
			} else {
				cout << "Game must be at least " << mainSettings.getSetting("minGameLengthMinutes", 1) << " minutes long to save" << endl;
				Sleep(2000);
			}
			requestReset();
			break;
		} else if ((GetKeyState(VK_NEXT) & 0x8000) != 0) {
			cout << "Unpausing . . .";
			Sleep(2000);
			break;
		}
		pausedTime = time(0);
		Sleep(10);
	}
}

string KeyPress::getName() {
	return keyName;
}

void KeyPress::registerKeys(vector<int> keyCodes) {
	
	for (int i = 0; i < keyCodes.size(); i++) {
		keyRegistry.push_back(keyCodes.at(i));
	}
	
}

bool KeyPress::containsKey(int keyCode) {
	for (int i = 0; i < keyRegistry.size(); i++) {
		if (keyRegistry.at(i) == keyCode) 
			return true;
	}
	return false;
}

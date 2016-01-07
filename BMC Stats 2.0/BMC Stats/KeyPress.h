#pragma once

#include "Config.h"

using namespace std;

class Display;

class KeyPress {
private:
	//private variables
	Config mainSettings;
	vector<int> keyRegistry;
	map<int,int> actionCounter;
	map<int, map<string, bool>> keyState;
	time_t startTime, latestKeyPressTime, spaceHeldTime, pausedTime;
	double secondsElapsed;
	int totalActionCount, spaceHeld, loopCount;
	bool reset;
	string keyName;

	//private functions
	void requestReset();
	void saveToFile();
	bool containsKey(int keyCode);
	void registerKeys(vector<int> keyCodes);

public:
	//constructor
	KeyPress(Config config, vector<int> keyCodes, string name);

	//trigger updates
	bool isPressed(int key);
	void isHeld(int key);
	bool timedUpdate(int seconds);

	//informational
	string getActionCount(int keyName);
	string getTotalActionCount();
	string getApm();
	string getElapsedTime();
	string getLockedScreenPercent();
	string getName();
	int getSecondsLocked();


	//utility
	void incrementLoopCount();
	bool checkIfRequiresReset();
	void pauseLoop();

};


#pragma once

#include "Config.h"

using namespace std;

class KeyPress;

class Notification {
private:
	Config mainSettings;
	int getSpacePercent(int loopCount, int spaceHeld);
	time_t lastBeepTime;
	int secondsSinceLastBeep;
public:
	Notification(Config config);
	void lockedScreen(int lockedScreenPercent, int lockedScreenTime);
};


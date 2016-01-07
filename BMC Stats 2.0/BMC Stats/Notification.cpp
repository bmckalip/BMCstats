#include "stdafx.h"
#include "Notification.h"

#include <windows.h>
#include <ctime>

#include "Display.h"
#include "Config.h"
#include "KeyPress.h"

Notification::Notification(Config config) {
	mainSettings = config;
}

void Notification::lockedScreen(int lockedScreenPercent, int secondsLocked) {
	
	//is this notification enabled?
	if (!mainSettings.getSetting("lockedScreenBeep_Enabled", 1)) {
		return;
	}

	secondsSinceLastBeep = difftime(time(0), lastBeepTime);


	//what percent will trigger a beep?
	if ((lockedScreenPercent < mainSettings.getSetting("lockedScreenBeep_%Threshold", 1))) {
		return;
	}
	
	//how long player has held space
	if ((secondsSinceLastBeep < mainSettings.getSetting("lockedScreenBeep_TimeHeldSpaceDelay", 1))) {
		return;
	}

	if (!GetAsyncKeyState(VK_SPACE)) {
		return;
	}

	//scale frequency with lockedScreenPercent?
	if (mainSettings.getSetting("lockedScreenBeep_FreqModulation", 1)) {
		Beep(((10 * lockedScreenPercent) + 50), 100);
		lastBeepTime = time(0);
	} else {
		//beep A
		Beep(523, 100);
		lastBeepTime = time(0);
	}
	
}
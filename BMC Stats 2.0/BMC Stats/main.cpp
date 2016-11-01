// BMC Stats.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <map>

#include "Display.h"
#include "KeyPress.h"
#include "Config.h"
#include "Notification.h"

#define BUFFER_SIZE 80
bool checkKeys(KeyPress key, Display counter, Notification notification, vector<int> keyCodeArray, bool isMenu = 0);

int main() {
	//register keys to track
	
	vector<int> keyCodes {	VK_LBUTTON, VK_RBUTTON, VK_TAB,
							VK_DELETE,
							VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,
							0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
							0x41, 0x44, 0x45, 0X46,
							0x51, 0x52, 0x53, 0X57 
						  };

	vector<int> menuKeyCodes{ VK_END, VK_HOME, VK_DOWN, VK_UP, VK_RETURN

	};

	//load config and notifications
	Config config("config.txt");
	Notification notification(config);

	while (true) {
		system("CLS");
		//display the menu
		KeyPress menuKeys(config, menuKeyCodes, "menuKeys");
		Display mainMenu(config, BUFFER_SIZE, menuKeys);
		checkKeys(menuKeys, mainMenu, notification, menuKeyCodes, 1);
		
		//ckeck for keys and display the counter screen
		KeyPress keyLogger(config, keyCodes, "keyLogger");
		Display counter(config, BUFFER_SIZE, keyLogger);
		checkKeys(keyLogger, counter, notification, keyCodes);
		
	}

	//exit main
	return 0;
}

bool checkKeys(KeyPress key, Display display, Notification notification, vector<int> keyCodeArray, bool isMenu) {
	display.updateDisplay(key);
	system("CLS");
	cout << display.toString();

	while (true) {
		for (int i = 0; i < keyCodeArray.size(); i++) {
			if (key.isPressed(keyCodeArray.at(i))) {
				display.updateDisplay(key);
				system("CLS");
				cout << display.toString();
			};
		}

		//extra logic for if the display is not a menu:
		if (!isMenu) {
			if (key.timedUpdate(1)) {
				display.updateDisplay(key);
				system("CLS");
				cout << display.toString();
			}

			key.isHeld(VK_SPACE);
			notification.lockedScreen(stoi(key.getLockedScreenPercent()), key.getSecondsLocked());
			key.incrementLoopCount();
		}

		//reset counter and key presses
		if (key.checkIfRequiresReset()) break;
		Sleep(10);
	}
	return 0;
}


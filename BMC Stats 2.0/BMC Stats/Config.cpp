#include "stdafx.h"
#include "Config.h"

#include <regex>
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>
#include <iostream>
using namespace std;

Config::Config() {
	
}

Config::Config(string filename) {
	if (!loadConfig(filename)) {
		cout << "  The config file is missing! The program will now exit.";
		Sleep(2000);
	}
}

bool Config::loadConfig(string file) {
	ifstream config(file);
	string line;
	vector <string> lines;

	if (config) {
		if (!config.is_open()) {
			config.open(file);
		}
	} else {
		return 0;
	}

	//get lines from the config file
	while (getline(config, line)) {
		lines.push_back(line);
	}

	settings = createSettingsMap(lines);
	return true;
}

map<string, string> Config::createSettingsMap(vector<string> lines) {

	vector <string> settingVector, settingLabels, settingValues;
	map<string, string> settings;

	for (int i = 0; i < lines.size(); i++) {
		settingVector = split(lines.at(i), ':');

		for (int i = 0; (i < settingVector.size()); i++) {
			(i % 2) ? settingValues.push_back(settingVector.at(i)) : settingLabels.push_back(settingVector.at(i));
		}

		//map the vectors together
		for (int i = 0; i < settingLabels.size(); ++i) {
			settings[settingLabels.at(i)] = settingValues.at(i);
		}
	}
	return settings;

}

vector<string> Config::split(string str, char delimiter) {
	vector<string> internal;

	//strips whitespace
	str = regex_replace(str, regex("\\s+"), "");

	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

string Config::getSetting(string settingName) {
	return settings[settingName];
}

int Config::getSetting(string settingName, bool isInt) {
	if (isInt) {
		return stoi(settings[settingName]);
	} else {
		return -1;
	}
}

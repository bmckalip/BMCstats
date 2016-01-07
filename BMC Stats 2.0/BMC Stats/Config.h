#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

class Config {
private:
	map<string, string> settings;
	vector<string> split(string str, char delimiter);
	
	map<string, string> createSettingsMap(vector<string> lines);
public:
	Config();
	Config(string filename);
	
	bool loadConfig(string file);
	string getSetting(string settingName);
	int getSetting(string settingName, bool isInt);
};


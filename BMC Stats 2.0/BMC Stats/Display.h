#pragma once

#include <string>
#include <vector>

#include "config.h"


using namespace std;


class KeyPress;

class Display {
private:
	Config mainSettings;
	vector<string> lines;
	int bufferSize;
	void createDivider();
	string getLine(int LineNumber);
	bool isValid(string text);
	void addHeader(string appended = "");
	void emptyDisplay();
	int numOfLines;
public:
	Display(Config config, int lineBufferSize, KeyPress key);
	string toString();
	void addLine(string text);
	void addLine(string text, int position);
	void removeLine(int position);
	void setLine(int position, string text);
	void updateDisplay(KeyPress key);
	int getNumOfLines();
};                                                                                                                                                                                                                                                                                                                                                                                                                                                     
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <map>
using namespace std;

vector<string> split(string line, char delimiter);
void gotoxy(int x, int y);
void printsquare(int xi, int yi, int width, double height);
void printline(int xi, int yi, int width, double height);
void resetTextColor();
void setTextColorRed();
void setTextColorGreen();
int pauseExecution();
void clearScreen();
class exchangedata {
	friend class candlestick;
	string timestamp;
	string base_curr, quote_curr;
	string currency;
	string side;
	double price, amount;
	double value;
public:
	string getname();
	void print();
	friend ifstream& operator>>(ifstream& fin, vector <exchangedata>& C);
	void sortit(vector<exchangedata>& exdt);
	friend ostream& operator<<(ostream& fout, vector <exchangedata>& C);
};
class candlestick
{
	string date;
	string currency;
	string side;
	double high, low, open, close;
	string name;
public:
	string getname();
	vector<candlestick> computecandlestickData(vector <exchangedata>& exdt);
	friend ostream& operator<<(ostream& fout, vector<candlestick> cdst);
	void printcandles(vector <candlestick> cdst, string side, string currency);
};

/*This code is responsible for generating and printing candlestick charts based on exchangedata.
It provides functionalities to compute candlestick data from exchangedata and display the charts.
The candlestick class represents a single candlestick with attributes such as date, open, high, low, close, and name.
The exchangedata class represents data about a single exchange with attributes like timestamp, currency, side, price, and amount.

The operator<< overloads the output stream operator for vector<exchangedata> and vector<candlestick> to facilitate printing the data.

The exchangedata class has a sortit() function that sorts the exchangedata objects based on currency, side, and timestamp.

The candlestick class has a computecandlestickData() function that takes a vector of exchangedata objects and computes the candlestick data.
It iterates through the exchangedata objects and groups them based on date, currency, and side to calculate the high, low, open, and close values for each candlestick.

The getname() function in the exchangedata and candlestick classes returns the name of the respective object.

The printcandles() function in the candlestick class prints the candlestick chart based on the computed candlestick data.
It uses the Windows console functions to position the output and prints the candlesticks and price scale.

The code also includes some auxiliary/utility functions such as printline() and printsquare() to draw the candlestick shapes on the console.
Overall, the code provides a way to process exchangedata and generate visual representations in the form of candlestick charts.*/
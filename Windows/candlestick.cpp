#include "candlestick.h"

// //Function to split a string into tokens based on a delimiter
vector<string> split(string line, char delimiter) {
    vector<string> tokens;
    int i = 0;
    while (i < line.size()) {
        if (line[i] == delimiter) {
            tokens.push_back(line.substr(0, i));
            line = line.substr(i + 1);
            i = 0;
        } else {
            i++;
        }
    }

    // Add the last token.
    tokens.push_back(line);

    return tokens;
}

// Function to set the cursor position in the console
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Function to print a line in the text-based plot
void printline(int xi, int yi, int width, double height) {
    xi += width / 2;
    if (height < 0) {
        setTextColorRed();; // Set text color to red
        height *= (-1);
    } else {
        setTextColorGreen(); // Set text color to green
    }
    if (height < 1 && height >= 0) {
        height = 1;
    }

    gotoxy(xi, yi);  // Set the initial position
    for (int i = 0; i < height; i++) {
        gotoxy(xi, yi - i);  // Move the cursor to the appropriate position
        cout << '|';  // Print a vertical line character
    }

    resetTextColor(); // Set text color back to white
}

// Function to print a square in the text-based plot
void printsquare(int xi, int yi, int width, double height) {
    if (height < 0) {
        setTextColorRed();; // Set text color to red
        height *= -1;
    } else {
        setTextColorGreen(); // Set text color to green
    }

    if (height < 1 && height >= 0) {
        height = 1;
    }

    for (int y = yi; y > yi - height; y--) {
        gotoxy(xi, y);
        for (int x = xi; x < xi + width; x++) {
            cout << char(178); // Print a filled rectangle character
        }
    }

resetTextColor(); // Set text color back to white
}

// Overloaded insertion operator to output vector of exchangedata objects to an output stream
ostream& operator<<(ostream& fout, vector<exchangedata>& C) {
    for (int i = 0; i < C.size(); i++)
        fout << C[i].timestamp << "  " << C[i].currency << "  " << C[i].side << endl;
    return fout;
}

// Overloaded extraction operator to read vector of exchangedata objects from an input stream
ifstream& operator>>(ifstream& fin, vector<exchangedata>& C) {
    string line;
    exchangedata c;
    while (getline(fin, line)) {
        // Split the line into tokens.
        vector<string> tokens = split(line, ',');

        // Assign the values to the exchangedata object.
        c.timestamp = tokens[0];
        c.currency = tokens[1];

        size_t slashPos = c.currency.find('/');
        if (slashPos != string::npos) {
            c.base_curr = c.currency.substr(0, slashPos);
            c.quote_curr = c.currency.substr(slashPos + 1);
        }

        c.side = tokens[2];
        c.price = stod(tokens[3]);
        c.amount = stod(tokens[4]);
        c.value = c.amount * c.price;
        // Add the exchangedata object to the vector.
        C.push_back(c);

    }
    C[0].sortit(C);
    return fin;
}

// Member function to print exchangedata object
void exchangedata::print() {
    cout << "This is Exchangedata Print()\n";
}

// Member function to sort vector of exchangedata objects
void exchangedata::sortit(vector<exchangedata>& exdt) {
    // Sort based on currency, side, and timestamp
    sort(exdt.begin(), exdt.end(), [](const exchangedata& a, const exchangedata& b) {
        if (a.currency != b.currency)
            return a.currency < b.currency;
        if (a.side != b.side)
            return a.side < b.side;
        return a.timestamp < b.timestamp;
        });
}

// Member function to get name of exchangedata object
string exchangedata::getname() {
    return (side + " for " + currency);
}

// Overloaded insertion operator to output vector of candlestick objects to an output stream
ostream& operator<<(ostream& fout, vector<candlestick> cdst) {
    fout << "\nDate\t\tOpen\tHigh\tLow\tClose\t\t" << endl;
    for (int i = 0; i < cdst.size(); i++)
        fout << cdst[i].date << "\t" << cdst[i].open << "\t" << cdst[i].high << "\t" << cdst[i].low << "\t" << cdst[i].close << "\t\t" << cdst[i].name << endl;
    return fout;
}
// Function to compute candlestick data from vector of exchangedata objects
vector<candlestick> candlestick::computecandlestickData(vector<exchangedata>& exdt) {
    vector<candlestick> c;
    exdt[0].sortit(exdt);
    for (int i = 0; i < exdt.size() - 1; ) {
        candlestick D;
        // Calculate High, Low, Open, and Close from the data, divided into sections based on unique date, currency, and side
        string temp_date, temp_currency, temp_side;

        temp_date = exdt[i].timestamp.substr(0, 10);
        temp_currency = exdt[i].currency;
        temp_side = exdt[i].side;
        D.name = exdt[i].side + " for " + exdt[i].currency;
        D.high = D.low = exdt[i].price;

        double pri = exdt[i].price;
        int bids = 0;

        for (int j = i + 1; j < exdt.size(); j++, bids++) {
            if (exdt[j].timestamp == exdt[j - 1].timestamp)
                pri += exdt[j].price;
            else
                break;
        }

        D.open = pri / (bids + 1);

        D.date = exdt[i].timestamp.substr(0, 10);
        D.currency = exdt[i].currency;
        D.side = exdt[i].side;

        for (; temp_date == exdt[i].timestamp.substr(0, 10) && temp_currency == exdt[i].currency && temp_side == exdt[i].side; i++) {
            if (exdt[i].price > D.high)
                D.high = exdt[i].price;
            if (exdt[i].price < D.low)
                D.low = exdt[i].price;
            if (i == (exdt.size() - 1))
                break;
        }

        D.close = exdt[i - 1].price;
        if (i == exdt.size() - 1) {
            D.close = exdt[i].price;
            c.push_back(D);
            break;
        }
        c.push_back(D);
    }
    return c;
}

string candlestick::getname() {
    return name;
}

void candlestick::printcandles(vector<candlestick> cdst, string side, string currency) {
    vector<candlestick> candles;
    for (int i = 0; i < cdst.size(); i++) {
        if (cdst[i].currency == currency && cdst[i].side == side)
            candles.push_back(cdst[i]);
    }

    if (candles.size() == 0) {
        cout << "No Data Available to Show\n";
        return;
    }

    clearScreen();

    // Printing horizontal lines
    for (int i = 0; i < 25; i++) {
        gotoxy(20, i + 1);
        cout << char(178);
    }

    // Printing separator lines
    for (int i = 0; i < 5; i++) {
        gotoxy(21, (i * 6) + 1);
        cout << "-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -";
        resetTextColor();
    }

    int gap = (100 / (2 * candles.size()));
    cout << endl;
    int x = 23, y = 24;
    double max_high = candles[0].high;
    double min_low = candles[0].low;

    // Finding the maximum high and minimum low values
    for (int i = 0; i < candles.size(); i++) {
        if (candles[i].high > max_high)
            max_high = candles[i].high;
        if (candles[i].low < min_low)
            min_low = candles[i].low;
    }

    double val = (max_high - min_low) / 24;
    int u = x;
    int v = y;

    for (int i = 0; i < (2 * candles.size()); i++) {
        // Scaling and printing the candlestick chart
        gotoxy(u, v);
        if (i % 2 == 0 && val != 0) {
            if (candles[i / 2].open < candles[i / 2].close) {
                printline(u, v - ((candles[i / 2].low - min_low) / val), gap, ((candles[i / 2].high - candles[i / 2].low) / val));
                printsquare(u, v - ((candles[i / 2].open - min_low) / val), gap, ((candles[i / 2].open - candles[i / 2].close) / -val));
            }
            else {
                printline(u, v - ((candles[i / 2].low - min_low) / val), gap, -((candles[i / 2].high - candles[i / 2].low) / val));
                printsquare(u, v - ((candles[i / 2].close - min_low) / val), gap, ((candles[i / 2].open - candles[i / 2].close) / -val));
            }

            // SetConsoleTextAttribute(h, 14);
            gotoxy(u, v + 3);
            cout << candles[i / 2].date.substr(5);
            resetTextColor();
        }

        u += (gap);
    }

    // Printing the bottom line
    gotoxy(21, 25);
    for (int i = 0; i < 100; i++)
        cout << char(178);

    // Printing the price scale on the left side
    // SetConsoleTextAttribute(h, 14);
    for (double i = 1, j = 1; i >= 0; i -= 0.25, j += 6) {
        gotoxy(1, j);
        cout << "$" << ((max_high - min_low) * i) + min_low;
    }

    // Printing data description and candlestick data
    gotoxy(0, 29);
    cout << "Data Description:  ";
    cout << endl << candles;
    resetTextColor();
}

void setTextColorRed() {
    printf("\033[0;31m"); // Set text color to red
}

void resetTextColor() {
    printf("\033[0m"); // Reset text color to default
}

void setTextColorGreen() {
    printf("\033[0;32m"); // Set text color to green
}
int pauseExecution() {
    int c;
    cout<<"Enter 0 to continue, else exit: ";
    cin>>c;
    return c;
}
void clearScreen() {
    printf("\033[2J");  // ANSI escape sequence to clear the entire screen
    printf("\033[H");   // ANSI escape sequence to move the cursor to the top-left position
}

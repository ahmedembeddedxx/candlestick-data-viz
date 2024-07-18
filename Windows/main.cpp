#include "candlestick.h"

using namespace std;

int main() {
    vector<exchangedata> exdt;
    ifstream fin("data.csv");
    if (!fin) {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    fin >> exdt;

    candlestick cd;
    vector<candlestick> cdst = cd.computecandlestickData(exdt);

    vector<string> trade;
    for (int i = 0; i < exdt.size(); i++)
	{
		string temp;
		temp = exdt[i].getname();

		bool isFirstOccurrence = true;
		for (int j = 0; j < trade.size(); j++)
			if (temp == trade[j])
			{
				isFirstOccurrence = false;
				break;
			}

		if (isFirstOccurrence)
			trade.push_back(temp);
	}

    for (int i = trade.size() - 1, k = 1; i >= 0; i--, k++) {
        cout << k << ". " << trade[i] << endl;
    }
    cout << "0. Exit" << endl;

    while (true) {
        int c;
        cin>>c;
        if (c == 0)
            break;

        c %= trade.size();
        c += trade.size();
        c %= trade.size();

        cd.printcandles(cdst, trade[c].substr(0, 3), trade[c].substr(8));

        if(pauseExecution())
            break;
        clearScreen();
        for (int i = trade.size() - 1, k = 1; i >= 0; i--, k++) {
            cout << k << ". " << trade[i] << endl;
        }
        cout << "0. Exit" << endl;
    }

    return 0;
}


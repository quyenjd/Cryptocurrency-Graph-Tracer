/** Cryptocurrency's Trace Graph by @quyenjd (compiled with MinGW - GNU C++11). Powered by Bitfinex. **/

// Copyright 2017 quyenjd (Quyen Dinh)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "header/graph.h"
#include "header/prices.h"

using namespace std;

/* Crypto's ISO 4217 name. */
string crypto, cname;

/* Period between two consequent updates (in milliseconds). */
unsigned int tupdate;

/* Usage. */
string Usage() {
    ostringstream f;
    f << "Usage:" << endl;
    f << "------" << endl;
    f << "# cryptograph [crypto-symbol]" << endl;
    f << "Note: [crypto-symbol] stands for your want-to-trace cryptocurrency plus the suffix \"usd\"." << endl;
    f << "-> Example: cryptograph btcusd" << endl;
    f << "------" << endl;
    f << "List of allowed crypto-symbol:" << endl;
    f << "\"btcusd\",\"ltcusd\",\"ethusd\",\"etcusd\",\"rrtusd\",\"zecusd\",\"xmrusd\",\"dshusd\",\"bccusd\"," << endl;
    f << "\"bcuusd\",\"xrpusd\",\"iotusd\",\"eosusd\",\"sanusd\",\"omgusd\",\"bchusd\",\"neousd\",\"etpusd\"," << endl;
    f << "\"qtmusd\",\"bt1usd\",\"bt2usd\",\"avtusd\",\"edousd\",\"btgusd\",\"datusd\",\"qshusd\",\"yywusd\"." << endl;
    f << "For usage the symbols, you should have some knowledge about your crypto's 3-letter (ISO 4217) name." << endl;
    f << "You can Google it or find it on your trading platform. It should appear along with your crypto's name." << endl;
    f << "------" << endl;
    return f.str();
}

/* Get arguments. */
void Get (int argc, char* argv[]) {
    if (argc != 2) {
        cout << Usage();
        halt(1, "Cryptograph: Invalid usage of cryptograph!");
    }
    if (bfind(__bitfinex_api_symbols__, string(argv[1])) == -1) {
        cout << Usage();
        halt(1, "Cryptograph: Invalid cryptocurrency's symbol!");
    }
    c_logf = crypto = argv[1];
}

/* Translate your crypto's name. */
void TransCryptoName() {
    cname = __bitfinex_api_names__[bfind(__bitfinex_api_symbols__, crypto)];
}

void Status() {
    SetPosition(0, 41);
    textcolor(WHITE);
    cout << "Watching ";
    textcolor(GREEN);
    cout << cname;
    textcolor(WHITE);
    cout << " ..." << endl;
}

/* Get user's config. */
bool GetUserConfig() {
    ifstream f(__userconfig__);
    if (!f.is_open())
        halt(1, rfmt("Cryptograph: %s not found!", __userconfig__.c_str()));
    string s;
    getline(f, s);
    if (atoi(s.c_str()) == tupdate)
        return 0;
    tupdate = atoi(s.c_str());
    f.close();
    return 1;
}

/* How to use. */
void About (bool update = 0) {
    if (update) {
        SetPosition(0, 44);
        for (unsigned int i = 0; i <= CONSOLE_MAXWIDTH; ++i)
            cout << ' ';
        cout << "It will automatically update the graph every ";
        textcolor(RED); cout << (tupdate / 1000); textcolor(WHITE);
        cout << " second(s) and adjust it to a highly readable and noticable column graph with colors." << endl;
    }
    SetPosition(0, 43);
    textcolor(RED); cout << "How to use?" << endl; textcolor(WHITE);
    cout << "-----------" << endl;
    cout << "It will automatically update the graph every ";
    textcolor(RED); cout << (tupdate / 1000); textcolor(WHITE);
    cout << " second(s) and adjust it to a highly readable and noticable column graph with colors." << endl;
    cout << "A ";
    textcolor(GREEN); cout << "GREEN"; textcolor(WHITE);
    cout << " column stands for an increasing: the present value is greater than the previous value." << endl;
    cout << "A ";
    textcolor(YELLOW); cout << "YELLOW"; textcolor(WHITE);
    cout << " column stands for a decreasing: the present value is smaller than the previous value." << endl;
    cout << "Otherwise, it should be a ";
    textcolor(DARKTEAL); cout << "DARKTEAL"; textcolor(WHITE);
    cout << " column." << endl;
    cout << "The ";
    textcolor(PINK); cout << "higher"; textcolor(WHITE);
    cout << " the column is, the more ";
    textcolor(PINK); cout << "valuable"; textcolor(WHITE);
    cout << " your cryptocurrency is!" << endl;
    cout << "The horizontal \"time\" axis has the form [hh:mm:ss] informing you the actual times of having the prices." << endl;
    cout << "All the height, width, size, etc. are set to default and unchangeable." << endl;
    cout << "Don't worry, I have many auto tools which can beautify your graph!" << endl;
    cout << endl;
}

int main (int argc, char* argv[]) {
    system("mode con: cols=150 lines=9001");
    system("cls");
    textcolor(WHITE);
    Get(argc, argv);
    TransCryptoName();
    if (FindWindow(NULL, _T(rfmt("Cryptocurrency's Trace Graph by quyenjd (Powered by Bitfinex's API) - Watching %s ...", cname.c_str()).c_str())))
        halt(1, "Cryptograph: A cryptocurrency can only be watched by one instance!");
    SetConsoleTitle(_T(rfmt("Cryptocurrency's Trace Graph by quyenjd (Powered by Bitfinex's API) - Watching %s ...", cname.c_str()).c_str()));
    Status();
    About();
    GetMaxWidthConsole();

    // Init the graph.
    cag a(1, 1, 35, 98, 11, 8, "USD", 0, 1, 1, 1);
    cap b(crypto);

    // An endless loop getting data.
    while (1) {
        if (GetUserConfig())
            About(1);
        Log(rfmt("[%s] Updating data ...", dt().c_str()));
        b.Update();
        a.AddData(dt(1), b.GetLast());
        a.Print();
        Sleep(tupdate);
    }

    return 0;
}

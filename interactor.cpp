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

#include "header/alarm.h"
#include "header/reader.h"
#include "header/ping.h"
#include "header/qEngine.h"
#include <conio.h>

using namespace std;

/* ISO 4217 supported cryptos by 30rates. */
const vector<string> __iso4217__ = {"btcusd", "bchusd", "btgusd",
                                    "dshusd", "ethusd", "etcusd",
                                    "iotusd", "ltcusd", "xmrusd",
                                    "neousd", "xrpusd", "zecusd"};

/* 30rates links. */
const vector<string> __30rates_prediction__ = {
    "http://30rates.com/btc-to-usd-forecast-today-dollar-to-bitcoin",
    "http://30rates.com/bitcoin-cash-price-prediction-for-tomorrow-week-month",
    "http://30rates.com/bitcoin-gold-price-prediction-tomorrow-week-month-btg-forecast",
    "http://30rates.com/dash-price-prediction-tomorrow-week-month",
    "http://30rates.com/ethereum-price-prediction-tomorrow-week-month-eth-forecast",
    "http://30rates.com/ethereum-classic-price-prediction-tomorrow-week-month-etc-forecast",
    "http://30rates.com/iota-price-prediction-tomorrow-week-month-iot-forecast",
    "http://30rates.com/litecoin-price-prediction-tomorrow-week-month-ltc-forecast",
    "http://30rates.com/monero-price-prediction-tomorrow-week-month-xmr-forecast",
    "http://30rates.com/neo-price-prediction-tomorrow-week-month-neo-forecast",
    "http://30rates.com/ripple-price-prediction-tomorrow-week-month-xrp-forecast",
    "http://30rates.com/zcash-price-prediction-for-tomorrow-week-month-zec-today"
};

/* Test string's default length. */
const size_t TEST_DEFAULT = 3;

/* Cryptocurrency's name and symbol. */
string crypto, cname;

/* TUPDATE. */
unsigned int tupdate;

/* Alarm. */
cac alarm(inf, 0);

/* Last day the prediction is required. */
string lastpday;

/* Previous price. */
double pre;

/* Usage. */
string Usage() {
    ostringstream f;
    f << "Usage:" << endl;
    f << "------" << endl;
    f << "# interactor [crypto-symbol]" << endl;
    f << "Note: [crypto-symbol] stands for your want-to-trace cryptocurrency plus the suffix \"usd\"." << endl;
    f << "-> Example: interactor btcusd" << endl;
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
        halt(1, "Interactor: Invalid usage of interactor!");
    }
    if (bfind(__bitfinex_api_symbols__, string(argv[1])) == -1) {
        cout << Usage();
        halt(1, "Interactor: Invalid cryptocurrency's symbol!");
    }
    c_logf = crypto = argv[1];
}

/* Translate your crypto's name. */
void TransCryptoName() {
    cname = __bitfinex_api_names__[bfind(__bitfinex_api_symbols__, crypto)];
}

/* Get prediction from 30rates. */
void RangePrediction() {
    int pos = bfind(__iso4217__, crypto);
    if (pos == -1) {
        textcolor(WHITE);
        cout << "30rates.com: ";
        textcolor(TEAL); cout << "Prediction is not available!" << endl; textcolor(WHITE);
        return;
    }
    cad a(__30rates_prediction__[pos], "30rates_" + crypto);
    a.StartDownload();
    car b("30rates_" + crypto);
    string tday = split(split(b.line(crypto == "btcusd" ? 196 : 193), '>')[1], '<')[0],
           tmin = split(split(b.line(crypto == "btcusd" ? 202 : 199), '>')[1], '<')[0],
           tmax = split(split(b.line(crypto == "btcusd" ? 205 : 202), '>')[1], '<')[0];
    textcolor(WHITE);
    cout << rfmt("30rates.com: Range prediction for tommorrow (%s) is ", tday.c_str());
    textcolor(YELLOW); cout << rfmt("%s-%s", tmin.c_str(), tmax.c_str()); textcolor(WHITE);
    cout << '.' << endl;
}

/* Get user's config. */
void GetUserConfig() {
    ifstream f(__userconfig__);
    if (!f.is_open())
        halt(1, rfmt("Interactor: %s not found!", __userconfig__.c_str()));
    string s;
    getline(f, s);
    tupdate = atoi(s.c_str());
    f.close();
}

/* Write config.cfg. */
void WriteConfig (unsigned int TUPDATE) {
    ofstream f(__userconfig__);
    if (!f.is_open())
        halt(1, "Interactor: config file is being used!");
    f << TUPDATE << endl;
    f.close();
}

/* Random a string with lowercase letters. */
string _rsl (size_t len) {
    string rep;
    for (size_t i = 0; i < len; ++i)
        rep += 'a' + (int)(Rand() * 25);
    return rep;
}

/* Wake up. */
void WakeUp (double price) {
    // Wakeup confirmation.
    string wakeup = _rsl(TEST_DEFAULT), test;
    char c;

    system("cls");
    textcolor(RED); cout << "WAKE UP!!!" << endl; textcolor(WHITE);
    cout << "The present price (";
    textcolor(GREEN); cout << '$' << price; textcolor(WHITE);
    cout << ") has met your wanted price (";
    textcolor(YELLOW); cout << '$' << alarm.Price(); textcolor(WHITE);
    cout << ')' << endl;
    textcolor(RED); cout << "HURRY UP!!!" << endl; textcolor(WHITE);
    cout << "---" << endl;
    cout << "Type ";
    textcolor(PINK); cout << wakeup; textcolor(WHITE);
    cout << " to stop the alarm!   ";
    textcolor(TEAL);

    // User enters test string.
    do {
        test.clear();
        while (1) {
            Clear(29 + TEST_DEFAULT, 4);
            SetPosition(29 + TEST_DEFAULT, 4);
            cout << test;
            c = getch();

            // If 'Enter' is pressed, break.
            if (c == 13)
                break;

            if (c == 8) {
                if (!test.empty())
                    test.pop_back();
            } else {
                test += c;
            }
        }
    } while (test != wakeup);

    // Reset the alarm.
    alarm.Reset(inf, 0);
    alarm.WokenUp();
}

/* Convert string to double. */
double strflt (const std::string& s) {
    stringstream r;
    double t;
    r << s;
    r >> t;
    return t;
}

/* User set an alarm. */
void Alarm() {
    // Pop-up.

    // Wanted price.
    double wanted = 0;
    char c;
    string s;
    bool dot = 0;

    system("cls");
    cout << "You can choose your MP3 and rename it to ";
    textcolor(GREEN);
    cout << "\"default.mp3\"";
    textcolor(WHITE);
    cout << " then copy it to where" << endl;
    cout << "you save this Interactor for changing the alarm sound." << endl;
    cout << "Note: If your price is less than the currently price, the alarm will go off" << endl;
    cout << "when the currently price is ";
    textcolor(BLUE); cout << "lower or equal"; textcolor(WHITE);
    cout << " to your price and otherwise." << endl;
    cout << "Now, let me know your price: ";
    textcolor(YELLOW); cout << '$';
    while (1) {
        Clear(30, 4);
        SetPosition(30, 4);
        cout << s;
        c = getch();
        if (c == 13)
            break;
        if (c == 8) {
            if (!s.empty()) {
                if (s.back() == '.')
                    dot = 0;
                s.pop_back();
            }
        } else {
            if (c != '.') {
                if (!isdigit(c))
                    continue;
            } else {
                if (dot)
                    continue;
                dot = 1;
            }
            s += c;
        }
        wanted = strflt(s);
    }

    alarm.Reset(wanted, wanted < pre);
}

/* User change update frequency. */
void UpdateFreq() {
    // New tupdate value.
    unsigned int newtupdate = 0;
    string s;
    char c;

    GetUserConfig();
    system("cls");
    cout << "Current update frequency is ";
    textcolor(RED); cout << rfmt("%ds", tupdate / 1000); textcolor(WHITE);
    cout << " per update." << endl;
    cout << "Want to change update frequency? Type new value here (in seconds) : ";
    textcolor(YELLOW);
    while (1) {
        Clear(68, 1);
        SetPosition(68, 1);
        cout << s;
        c = getch();
        if (c == 13)
            break;
        if (c == 8) {
            if (!s.empty())
                s.pop_back();
        } else {
            if (!isdigit(c))
                continue;
            s += c;
        }
        newtupdate = strflt(s);
    }
    textcolor(WHITE);

    WriteConfig(newtupdate * 1000);
}

/* User want a detailed analysis. */
void Analysis() {
}

/* Is my graph working? */
bool GraphCheck() {
    return FindWindow(NULL, _T(rfmt("Cryptocurrency's Trace Graph by quyenjd (Powered by Bitfinex) - Watching %s ...", crypto.c_str()).c_str()));
}

/* Main updating void. */
void Updating() {
    lastpday.clear();
    GetUserConfig();
    unsigned int Ping;
    bool gooff = 0;

    while (1) {
        // Init (Waiting state).
        Clear(34, 3);
        SetPosition(34, 3);
        textcolor(BLUE); cout << "Waiting"; textcolor(WHITE);
        Clear(76, 6);
        SetPosition(76, 6);
        textcolor(BLUE); cout << "Waiting"; textcolor(WHITE);
        cout << ").";

        // Get tupdate.
        GetUserConfig();
        cat a(bitf_ip, tupdate, "ping_" + crypto);

        // Bitfinex's ping.
        Clear(34, 2);
        SetPosition(34, 2);
        Ping = a.GetPing();
        textcolor((Ping <= tupdate / 5) ? GREEN : (Ping <= tupdate / 2) ? YELLOW : RED);
        cout << rfmt("%dms", Ping);
        if (Ping > (tupdate / 2))
            cout << " - Lag!";
        textcolor(WHITE);

        // The present price and check the alarm.
        double price = alarm.CheckAlarm(crypto, gooff, !GraphCheck());
        Clear(34, 3);
        SetPosition(34, 3);
        textcolor(price > pre ? GREEN : price == pre ? DARKTEAL : RED);
        cout << '$' << price;
        pre = price;
        textcolor(WHITE);
        if (alarm.Price() != inf) {
            cout << " (An alarm was set at ";
            textcolor(PINK);
            cout << '$' << alarm.Price();
            textcolor(WHITE);
            cout << ')';
        }
        if (gooff) {
            WakeUp(price);
            break;
        }

        // Currently tupdate.
        Clear(76, 6);
        SetPosition(76, 6);
        textcolor(RED); cout << (tupdate / 1000) << 's'; textcolor(WHITE);
        cout << ").";

        // Prediction.
        string tmp = dt(0, 1);
        if (tmp != lastpday) {
            Clear(0, 10);
            SetPosition(0, 10);
            RangePrediction();
            lastpday = tmp;
        }

        // User actions.
        bool reload = 0;
        clock_t sav = clock();
        while (clock() - sav <= tupdate) {
            while (_kbhit()) _getch();
            if (KeyPressed(17) && KeyPressed(16)) {
                if (KeyPressed('A')) {
                    Alarm();
                    reload = 1;
                    break;
                }
                if (KeyPressed('U')) {
                    UpdateFreq();
                    reload = 1;
                    break;
                }
                if (KeyPressed('D')) {
                    Analysis();
                    reload = 1;
                    break;
                }
            }
        }
        if (reload)
            break;
    }
}

/* Main void. */
void Interactor() {
    system("cls");
    textcolor(WHITE);
    cout << "Welcome to Interactor of ";
    textcolor(YELLOW); cout << "Cryptocurrency's Trace Graph"; textcolor(WHITE);
    cout << " by quyenjd!" << endl;
    cout << "---" << endl;
    cout << "Delay time on Bitfinex's server : " << endl;
    cout << rfmt("[%s] Price", cname.c_str());
    for (unsigned int i = cname.length() + 8; i < 32; ++i)
        cout << ' ';
    cout << ": " << endl;
    cout << "---" << endl;
    cout << "-> Press ";
    textcolor(TEAL); cout << "[Ctrl+Shift+A]"; textcolor(WHITE);
    cout << " anywhere to set an alarm." << endl;
    cout << "-> Press ";
    textcolor(TEAL); cout << "[Ctrl+Shift+U]"; textcolor(WHITE);
    cout << " anywhere to change the update frequency (currently: " << endl;
    cout << "-> Press ";
    textcolor(TEAL); cout << "[Ctrl+Shift+D]"; textcolor(WHITE);
    cout << " anywhere to require a detailed analysis based on data (not available)." << endl;
    textcolor(RED); cout << "Note: Interactor's shortcuts can be used anywhere!" << endl; textcolor(WHITE);
    cout << "---" << endl;
    Updating();
}

int main (int argc, char* argv[]) {
    system("mode con: cols=120 lines=9001");
    system("cls");
    textcolor(WHITE);
    Get(argc, argv);
    TransCryptoName();
    if (FindWindow(NULL, _T(rfmt("Cryptocurrency's Interactor by quyenjd (Powered by 30rates) - Tracing %s ...", cname.c_str()).c_str())))
        halt(1, "Cryptograph: A cryptocurrency can only be watched by one instance!");
    SetConsoleTitle(_T(rfmt("Cryptocurrency's Interactor by quyenjd (Powered by 30rates) - Tracing %s ...", cname.c_str()).c_str()));
    GetMaxWidthConsole();
    while (1)
        Interactor();
    return 0;
}

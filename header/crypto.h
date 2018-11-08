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

#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <windows.h>
#include <stdnoreturn.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <tchar.h>
#include <ctype.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <random>

/* Supported cryptocurrencies' symbols. */
const std::vector<std::string> __bitfinex_api_symbols__ = {"btcusd", "ltcusd", "ethusd",
                                                           "etcusd", "rrtusd", "zecusd",
                                                           "xmrusd", "dshusd", "bccusd",
                                                           "bcuusd", "xrpusd", "iotusd",
                                                           "eosusd", "sanusd", "omgusd",
                                                           "bchusd", "neousd", "etpusd",
                                                           "qtmusd", "bt1usd", "bt2usd",
                                                           "avtusd", "edousd", "btgusd",
                                                           "datusd", "qshusd", "yywusd"};

/* Supported cryptocurrencies' names. */
const std::vector<std::string> __bitfinex_api_names__ = {"Bitcoin", "Litecoin", "Ethereum",
                                                         "Ethereum Classic", "Recovery Right Tokens", "Zcash",
                                                         "Monero", "Dashcoin", "Bitconnect",
                                                         "Bitconnect Unlimited", "Ripple", "Iota",
                                                         "EOS", "Santiment", "OmiseGO",
                                                         "Bitcoin Cash", "NEO", "Metaverse ETP",
                                                         "Quantum", "Bitcoin BT1", "Bitcoin hardfork 2x",
                                                         "Aventus", "Eidoo", "Bitcoin Gold",
                                                         "Datum", "QASH", "YOYOW"};

/* I use Bitfinex API for getting currency's exchange rate. */
const std::string prefix  = "https://api.bitfinex.com/v1/ticker/";
const std::string bitf_ip = "104.16.175.181"; // Bitfinex's IP address.

/* A log for saving price. */
std::string c_logf  = "log.txt";

/* Infinity value. */
const double inf = 0xffffffffffff;

/* Maximum width of console. */
unsigned int CONSOLE_MAXWIDTH;

/* Log line numbering. */
unsigned int wlog;

/* WLOG_START defines a start value for wlog. */
const unsigned int WLOG_START = 54;

/* User config's filename. */
const std::string __userconfig__ = "config.cfg";

/* Default tupdate value. */
const unsigned int TUPDATE_DEFAULT = 5000;

/* Prediction. */
const std::string PREDICT = "30rates.com";

/* I use Mersenne Twister Engine for random. */
std::mt19937_64 gen(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1));

/* Colors. */
enum CAG_COLOR {
    DARKBLUE   = 1,
    DARKGREEN  = 2,
    DARKTEAL   = 3,
    DARKRED    = 4,
    DARKPINK   = 5,
    DARKYELLOW = 6,
    GRAY       = 7,
    DARKGRAY   = 8,
    BLUE       = 9,
    GREEN      = 10,
    TEAL       = 11,
    RED        = 12,
    PINK       = 13,
    YELLOW     = 14,
    WHITE      = 15
};

/* Change text color. */
void textcolor (CAG_COLOR cinp) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cinp);
}

/* Split string by token. */
std::vector<std::string> split (const std::string& s, const char& token) {
	std::vector<std::string> rep;
	rep.push_back("");
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] != token)
			rep.back() += s[i];
		else
			rep.push_back("");
	}
	return rep;
}

/* Convert ints to strings. */
template<typename T>
inline std::string to_string (const T& t) {
    std::string r;
    std::stringstream s;
    s << t;
    s >> r;
    return r;
}

/* Reformat string with args, work like printf. */
std::string rfmt (const char* fmt...) {
    std::stringstream r;
    va_list args;
    va_start(args, fmt);
    while (*fmt != '\0') {
        char x = *fmt;
        if (x == '%') {
            ++fmt;
            if (*fmt == 'd') {
                int t = va_arg(args, int);
                r << t;
            } else
            if (*fmt == 'c') {
                int t = va_arg(args, int);
                r << char(t);
            } else
            if (*fmt == 's') {
                char* t = va_arg(args, char*);
                r << t;
            } else
            if (*fmt == 'f') {
                double t = va_arg(args, double);
                r << t;
            } else r << x << *fmt;
        } else r << x;
        ++fmt;
    }
    return r.str();
}

/* Date and Time. */
std::string dt (bool graph = 0, bool dayonly = 0) {
    // Get time from Windows.
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Variables.
    int mon  = ltm->tm_mon + 1,
        day  = ltm->tm_mday,
        hour = ltm->tm_hour,
        minn = ltm->tm_min,
        sec  = ltm->tm_sec;

    // If it is used for graph, return the form [mm:ss].
    if (graph)
        return (hour < 10 ? "0" : "") + to_string(hour) + ":" +
               (minn < 10 ? "0" : "") + to_string(minn) + ":" +
               (sec < 10 ? "0" : "") + to_string(sec);

	// If you want to get the day only.
	if (dayonly)
		return to_string(day);

    // Return the form [YYYY/MM/DD hh:mm:ss].
    return to_string(1900 + ltm->tm_year) + "/" +
           (mon < 10 ? "0" : "") + to_string(mon) + "/" +
           (day < 10 ? "0" : "") + to_string(day) + " " +
           (hour < 10 ? "0" : "") + to_string(hour) + ":" +
           (minn < 10 ? "0" : "") + to_string(minn) + ":" +
           (sec < 10 ? "0" : "") + to_string(sec);
}

/* Halt the program with message and exitcode. */
[[noreturn]] void halt (int exitcode, const std::string& message = "") {
    textcolor(WHITE);
    std::cerr << rfmt("[%s] Exitcode %d was returned with message: \"%s\"", dt().c_str(), exitcode, message.c_str()) << std::endl;
    exit(exitcode);
}

/* Count number of digits. */
template<typename T>
unsigned int cdigit (const T& n) {
    std::stringstream t;
    t << n;
    return t.str().length();
}

/* Set cursor position. */
void SetPosition (SHORT x, SHORT y) {
    COORD pos     = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!SetConsoleCursorPosition(output, pos))
        halt(1, rfmt("SetConsoleCursorPosition() went wrong! Error code: %s", GetLastError()));
}

/* Find an element in a vector. Return -1 if not found.
   I use simple linear search algorithm.
*/
template<typename T>
int bfind (const std::vector<T>& a, const T& b) {
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] == b)
            return i;
    return -1;
}

/* Write logs. */
void Log (const std::string& s) {
    if (!wlog) {
        SetPosition(0, WLOG_START);
        textcolor(TEAL); std::cout << "Logs:" << std::endl; textcolor(WHITE);
        std::cout << "-----";
        wlog = WLOG_START + 2;
    }
    textcolor(WHITE);
    SetPosition(0, wlog++);
    std::cout << s << std::endl;
}

/* Get random value in range [0, 1]. */
double Rand() {
	std::uniform_real_distribution<double> dis(0, 1);
    return dis(gen);
}

/* Get maximum width of console. */
void GetMaxWidthConsole() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    CONSOLE_MAXWIDTH = csbi.dwSize.X - 1;
}

/* Clear line. */
void Clear (unsigned int x, unsigned int y) {
    for (unsigned int i = x; i < CONSOLE_MAXWIDTH; ++i) {
        SetPosition(i, y);
        std::cout << ' ';
    }
}

/* Keypressed. */
bool KeyPressed (int key) {
    return GetAsyncKeyState(key) & 0x8000;
}

#endif /* __CRYPTO_H__ */

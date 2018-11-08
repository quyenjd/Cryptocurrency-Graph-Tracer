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

#include "header/crypto.h"
#include <conio.h>

using namespace std;

/* Prefix of getting input's line. */
const string pf = "Let's start by informing me your cryptocurrency: ";

/* Your cryptocurrency's name. */
string crypto;

/* Search priority cryptocurrency's name. */
string Search() {
    if (crypto.empty())
        return "";
    string cryptotmp = crypto;
    for (size_t i = 0; i < cryptotmp.length(); ++i)
        cryptotmp[i] = tolower(cryptotmp[i]);
    for (size_t i = 0; i < __bitfinex_api_names__.size(); ++i) {
        string tmp = __bitfinex_api_names__[i].substr(0, crypto.length());
        for (size_t j = 0; j < tmp.length(); ++j)
            tmp[j] = tolower(tmp[j]);
        if (cryptotmp == tmp)
            return __bitfinex_api_names__[i];
    }
    return "-1";
}

/* Get user's cryptocurrency (with suggestions). */
void GetInput (unsigned int line) {
    string suggestion;
    textcolor(GREEN);

    // A char for getchar().
    char c;
    while (1) {
        SetPosition(pf.length() + crypto.length(), line);

        // Get a char.
        c = getch();
        if (suggestion != "-1" && c == 13)
            crypto = suggestion;
        if (c == 13)
            break;

        Clear(pf.length(), line);

        // Add.
        if (c == 8) {
            if (!crypto.empty())
                crypto.pop_back();
        } else {
            crypto += c;
        }

        // Get suggestion.
        suggestion = Search();

        // Not found.
        if (suggestion == "-1") {
            SetPosition(pf.length(), line);
            cout << crypto << "   [not found]" << endl;
            SetPosition(pf.length() + crypto.length(), line);
            continue;
        }

        // Show suggestion.
        if (!crypto.empty()) {
            SetPosition(pf.length(), line);
            cout << suggestion << "   [suggested]" << endl;
            SetPosition(pf.length() + crypto.length(), line);
        }
    }

    textcolor(WHITE);

    if (bfind(__bitfinex_api_names__, crypto) != -1)
        return;

    for (size_t i = 0; i < crypto.length(); ++i)
        crypto[i] = tolower(crypto[i]);
    int tmp = bfind(__bitfinex_api_symbols__, crypto);
    if (tmp != -1) {
        crypto = __bitfinex_api_names__[tmp];
        return;
    }

    Clear(pf.length(), line);
    SetPosition(pf.length(), line);
    textcolor(RED); cout << "Your requested cryptocurrency is not supported!" << endl; textcolor(WHITE);
    halt(1, "Launcher: Unsupported cryptocurrency!");
}

/* Write config.cfg. */
void WriteConfig (unsigned int TUPDATE) {
    ofstream f(__userconfig__);
    if (!f.is_open())
        halt(1, "Launcher: config file is being used!");
    f << TUPDATE << endl;
    f.close();
}

/* Check config.cfg. */
bool CheckConfig() {
    bool ret = 0;
    ifstream f(__userconfig__);
    if (!f.is_open())
        ret = 1;
    f.close();
    if (ret)
        WriteConfig(TUPDATE_DEFAULT);
    return ret;
}

/* Is my interactor still fine? */
bool InteractorCheck() {
    return FindWindow(NULL, _T(rfmt("Cryptocurrency's Interactor by quyenjd (Powered by 30rates) - Tracing %s ...", crypto.c_str()).c_str()));
}

/* Welcome! */
void Welcome() {
    system("cls");
    textcolor(WHITE);
    crypto = "";
    cout << "Welcome to Launcher of ";
    textcolor(YELLOW); cout << "Cryptocurrency's Trace Graph"; textcolor(WHITE);
    cout << " by quyenjd!" << endl;
    cout << "---" << endl;
    cout << "I provide you a smart tool for tracing your cryptocurrency's price." << endl;
    cout << "This includes an automatic graph, an alarm and a detailed analysis (in development)." << endl;
    cout << "You can customize the update frequency, set an alarm so it can wake you up when" << endl;
    cout << "the price is as your need and do an analysis for determining a GOOD prize." << endl;
    cout << "Additionally, you will also have an online prediction from 30rates." << endl;
    cout << pf;
    GetInput(7);
    SetPosition(0, 8);
    cout << "---" << endl;
    textcolor(PINK); cout << "You have chosen ";
    textcolor(GREEN); cout << crypto;
    textcolor(PINK); cout << " as your cryptocurrency." << endl;
    textcolor(WHITE);
    cout << "---" << endl;
    cout << "Checking userconfig ... ";
    if (CheckConfig()) {
        textcolor(RED);
        cout << rfmt("[First-time run detection: TUPDATE is set to %d by default!]", TUPDATE_DEFAULT);
        textcolor(WHITE);
    } else
        cout << "Done!";
    cout << endl;
    cout << "Starting graph ... ";
    system(rfmt("start /max \"\" \"cryptograph\" \"%s\"", __bitfinex_api_symbols__[bfind(__bitfinex_api_names__, crypto)].c_str()).c_str());
    cout << "Done!" << endl;
    textcolor(BLUE); cout << "Switching to Interactive mode ... "; textcolor(WHITE);
    system(rfmt("start \"\" \"interactor\" \"%s\"", __bitfinex_api_symbols__[bfind(__bitfinex_api_names__, crypto)].c_str()).c_str());
    textcolor(BLUE); cout << "Done!" << endl; textcolor(WHITE);
    cout << endl;
    while (!InteractorCheck());
    textcolor(TEAL);
    cout << "-> Want to change your cryptocurrency? Press [Ctrl+Shift+0]   ";
    while (1) {
        if (!InteractorCheck()) {
            textcolor(RED); cout << "InteractorCheck fail, self-terminate mode is activated!"; textcolor(WHITE);
            break;
        }
        if (KeyPressed(17) && KeyPressed(16) && KeyPressed(48)) {
            textcolor(TEAL); cout << "User-terminate mode is activated!"; textcolor(WHITE);
            break;
        }
    }
    cout << endl;
    cout << endl;
    textcolor(RED); cout << "Terminating child processes ... "; textcolor(WHITE);
    system(rfmt("taskkill /f /fi \"WINDOWTITLE eq Cryptocurrency's Trace Graph by quyenjd (Powered by Bitfinex's API) - Watching %s ...\" >nul 2>nul", crypto.c_str()).c_str());
    system(rfmt("taskkill /f /fi \"WINDOWTITLE eq Cryptocurrency's Interactor by quyenjd (Powered by 30rates) - Tracing %s ...\" >nul 2>nul", crypto.c_str()).c_str());
}

int main() {
    system("mode con: cols=120 lines=9001");
    SetConsoleTitle(_T("Cryptocurrency's Trace Graph launcher by quyenjd"));
    GetMaxWidthConsole();
    while (1)
        Welcome();
    return 0;
}

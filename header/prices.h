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

#ifndef __PRICES_H__
#define __PRICES_H__

#include "crypto.h"
#include "downloader.h"

/* New class for getting currency's prices. */
class __currency_alarm_prices__ {
private:
    /* Cryptocurrency's name. */
    std::string cur;
    /* Mid, Bid, Ask and Last price. */
    double mid, bid, ask, las;

    /* Read double function.
       I will not handle any error case so you should give me a clean input.
    */
    double _readft (const std::vector<std::string>& t, size_t pos, size_t st) {
        // Error handling.
        if (!isdigit(t[pos][st]))
            halt(1, "CAP: Something wrong with the json data!");

        // The number you need, its fractional part and its number of precision.
        double rep = 0, frac = 0, cfrac = 1;

        // Get the integral part.
        while (st < t[pos].length() && isdigit(t[pos][st]))
            rep = rep * 10 + t[pos][st++] - '0';

        // If fractional part does not exist, stop here!
        if (t[pos][st] != '.')
            return rep;

        // Otherwise, get the fractional part and combine it.
        // Also ++st to skip the dot.
        ++st;
        while (st < t[pos].length() && isdigit(t[pos][st])) {
            frac   = frac * 10 + t[pos][st] - '0';
            cfrac *= 10;
            ++st;
        }
        return rep + frac / cfrac;
    }
public:
    /* Constructor. */
    __currency_alarm_prices__ (const std::string& cur) {
        this->cur = cur;
    }

    /* Main void to get the prices and split them. */
    void Update (bool update = 1) {
		// Check if log file is exist.
		std::ifstream f(c_logf);
		if (!f.is_open())
			update = 1;
		f.close();

		// Call Downloader to get information from market.
		if (update) {
			cad a(prefix + cur, c_logf);
			a.StartDownload();
		}

        // Fetch data.
        std::ifstream p(c_logf);
        if (!p.is_open())
            halt(1, "CAP: Cannot fetch data!");
        std::string s;
        p >> s;
        std::vector<std::string> t = split(s, ':');

        // Get Mid.
        mid = _readft(t, 1, 1);

        // Get Bid.
        bid = _readft(t, 2, 1);

        // Get Ask.
        ask = _readft(t, 3, 1);

        // Get Last price.
        las = _readft(t, 4, 1);
    }

    /* Get Mid. */
    double GetMid() {
        return mid;
    }

    /* Get Bid. */
    double GetBid() {
        return bid;
    }

    /* Get Ask. */
    double GetAsk() {
        return ask;
    }

    /* Get Last price. */
    double GetLast() {
        return las;
    }
};

typedef __currency_alarm_prices__ cap;

#endif /* __PRICES_H__ */

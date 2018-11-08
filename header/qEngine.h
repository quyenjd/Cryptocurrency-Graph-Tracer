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

#ifndef __QENGINE_H__
#define __QENGINE_H__

#include "crypto.h"

/* Max qEngine's data count. */
const size_t QE_MAXDAYS = 366; // 1 leap year.

/* A new engine for analyzing currency's increasing and decreasing periods. */
class __currency_alarm_qEngine__ {
private:
    std::vector<double> data[QE_MAXDAYS];
    unsigned int QE_DAYSCNT;
public:
    /* Constructor. */
    __currency_alarm_qEngine__() {
        for (size_t i = 0; i < QE_MAXDAYS; ++i)
            data[i].clear();
        QE_DAYSCNT = 0;
    }

    /* Is the price increasing? */
    bool Increasing() {
        size_t n = data[QE_DAYSCNT].size();
        return n > 1 ? data[QE_DAYSCNT - 1][n - 1] > data[QE_DAYSCNT - 1][n - 2] : 0;
    }

    /* Is the price decreasing? */
    bool Decreasing() {
        size_t n = data[QE_DAYSCNT].size();
        return n > 1 ? data[QE_DAYSCNT - 1][n - 1] < data[QE_DAYSCNT - 1][n - 2] : 0;
    }

    /* Add new entry. */
    void AddEntry (double las, bool newday = false) {
        data[QE_DAYSCNT += newday].push_back(las);
    }

    /* Analyze. */
    void Analyze() {
        // TODO:
        // 1. Get all past values and calculate the MIN and MAX values.
        // 2. Find the best growth rate of the day,
        // 3. Compare it with the best growth rate of the past days, weeks and months.
        // 4. Calculate the must-need price to get the user's willing price,
        // 5. Calculate the possibility of having user's willing price for the rest of the day,
        // 6. Calculate the possibility of having the price as a must-care price,
        // 7. Give diagnostics to user.
    }
};

typedef __currency_alarm_qEngine__ qEngine;

#endif /* __QENGINE_H__ */

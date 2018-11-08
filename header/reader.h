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

#ifndef __READER_H__
#define __READER_H__

#include "crypto.h"

/* A new class for accessing lines in a file. */
class __currency_alarm_reader__ {
private:
    std::vector<std::string> f;
public:
    /* Constructor. */
    __currency_alarm_reader__ (const std::string& des) {
        std::string s;
        std::ifstream p(des);
        if (!p.is_open())
            halt(1, "CAR: Cannot read file!");
        while (getline(p, s))
            f.push_back(s);
        p.close();
    }

    /* Get line by number (from 0). */
    std::string line (size_t t) {
        if (t >= f.size())
            return "";
        return f[t];
    }
};

typedef __currency_alarm_reader__ car;

#endif /* __READER_H__ */

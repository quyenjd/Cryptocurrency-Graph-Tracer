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

#ifndef __PING_H__
#define __PING_H__

#include "crypto.h"

/* Default ping log filename. */
const std::string __pinglog__ = "svping.cfg";

/* New class for pinging an IP address for getting the delay time. */
class __currency_alarm_ping__ {
private:
    std::string url, log;
	DWORD tout;

public:
    /* Constructor. */
    __currency_alarm_ping__ (const std::string& url, DWORD tout, const std::string log = __pinglog__) {
        if (url.empty())
            halt(1, "CAT: URL cannot be empty!");
        this->url  = url;
		this->tout = tout;
		this->log  = log;
    }

    /* Get ping times (using Windows' ping). */
    unsigned int GetPing() {
        system(rfmt("ping \"%s\" -w %d -n 1 >\"%s\"", url.c_str(), tout, log.c_str()).c_str());
        car a(log);
        if (split(a.line(2), '=').size() == 1)
            return tout;
        std::string tmp = split(a.line(2), '=')[2];
        unsigned int rep = 0;
        size_t i = 0;
        while (isdigit(tmp[i]))
            rep = rep * 10 + tmp[i++] - '0';
        return rep;
    }
};

typedef __currency_alarm_ping__ cat;

#endif /* __PING_H__ */

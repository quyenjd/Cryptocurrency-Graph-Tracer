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

#include "player.h"
#include "prices.h"

/* Alarm log file. */
const std::string alarmlog = "alarmlog.txt";

/* Default alarm sound. */
const std::string DEFAULT_ALARMSOUND = "default.mp3";

/* A new class for setting and going off alarms. */
class __currency_alarm_clock__ {
private:
    double price;
    cam noti = cam(DEFAULT_ALARMSOUND);
	bool less;

public:
    /* Reset the price. */
    void Reset (double price, bool less) {
        this->price = price;
		this->less  = less;
    }
	
    /* Constructor. */
    __currency_alarm_clock__ (double price, bool less) {
        Reset(price, less);
    }

    /* Set the alarm sound. */
    void SetAlarmSound (const std::string& des) {
        noti.SetSound(des);
    }
	
	double Price() {
		return price;
	}

    /* Check alarm. */
    double CheckAlarm (const std::string& crypto, bool& gooff, bool update) {
        cap a(crypto);
        a.Update(update);
        double tmp = a.GetLast();
        if (less ? tmp < price : tmp >= price) {
			gooff = 1;
            noti.Play();
		}
        return tmp;
    }

    /* User has woken up. */
    void WokenUp() {
        noti.Stop();
    }
};

typedef __currency_alarm_clock__ cac;

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

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "crypto.h"

/* A mini MP3-player struct using mciapi. */
struct __currency_alarm_player__ {
    /* Set new sound. */
    void SetSound (const std::string& des) {
        if (des.empty())
            halt(1, "CAM: Destination is invalid!");
        if (des.substr(des.size() - 3, 3) != "mp3")
            halt(1, "CAM: Invalid file type!");
        mciSendString("close mp3", NULL, 0, NULL);
        mciSendString(rfmt("open \"%s\" type mpegvideo alias mp3", des.c_str()).c_str(), NULL, 0, NULL);
    }

    /* Send file to mciapi. */
    __currency_alarm_player__ (const std::string& des) {
        SetSound(des);
    }

    /* Play sound. */
    void Play() {
        mciSendString("play mp3 from 0", NULL, 0, NULL);
    }

    /* Stop sound. */
    void Stop() {
        mciSendString("stop mp3", NULL, 0, NULL);
    }
};

typedef __currency_alarm_player__ cam;

#endif /* __PLAYER_H__ */

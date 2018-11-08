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

#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include "crypto.h"
#include <sdkdefs.h>
#include <urlmon.h>
#include <wininet.h>

/* New class for updating currency's last prices.
   I use Bitfinex json source to get currency's last prices.
*/
class __currency_alarm_downloader__ {
private:
    /* URL to get. */
    std::string url;
    /* Destination. */
    std::string des;

public:
    /* Constructor & Destructor. */
    __currency_alarm_downloader__ (const std::string& url, const std::string& des) {
        this->url = url;
        this->des = des;
    }

    /* Main void for performing downloading. */
    unsigned int StartDownload() {
        DeleteUrlCacheEntry((TCHAR*)url.c_str());
        HRESULT hr = URLDownloadToFile(NULL, (TCHAR*)url.c_str(), (TCHAR*)des.c_str(), 0, NULL);

        /* Check errors and return. */
        if (SUCCEEDED(hr))
            return 0;
        return hr;
    }
};

typedef __currency_alarm_downloader__ cad;

#endif /* __DOWNLOADER_H__ */

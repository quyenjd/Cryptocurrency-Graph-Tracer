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

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "crypto.h"

/* Max Graph's number of columns. */
const size_t CAG_MAXNUMCOL = 100;

/* My own column graph representation on Windows Console. */
class __currency_alarm_graph__ {
private:
    /* A struct for saving graph data. */
    struct __cag_db__ {
        std::string x;
        double y;
        __cag_db__() {
            x = "";
            y = 0;
        }
        __cag_db__ (std::string _x, double _y): x(_x), y(_y) {}
    };
    /* Upper-left graph's starting point. */
    unsigned int CAG_START_x, CAG_START_y;
    /* Graph's length and width. */
    unsigned int CAG_YLEN, CAG_XLEN;
    /* Graph's column's width. */
    unsigned int CAG_COL_WIDTH;
    /* Number of columns in graph. */
    unsigned int CAG_NUMCOL;
    /* Note of y-axis. x-axis is set to "time" constantly. */
    std::string yaxis, xaxis = "time";
    /* Your input graph (number of columns used and its data). */
    unsigned int CAG_COLCNT;
    __cag_db__ data[CAG_MAXNUMCOL];
    /* Minimum, maximum and step of y-axis. */
    int MIN_yaxis, MAX_yaxis, mSTEP_yaxis, STEP_yaxis;
    /* Step of x-axis. */
    int STEP_xaxis;
    /* Previous value for comparison. */
    double PRE_CMP;
    /* Previous value for CAG_XLEN. */
    unsigned int PRE_XLEN;
public:
    /* A void used for initalizing or cleaning graph data. */
    void ClearData() {
        for (size_t i = 0; i < CAG_MAXNUMCOL; ++i)
            data[i] = __cag_db__();
        CAG_COLCNT  = 0;
        PRE_CMP     = -inf;
        PRE_XLEN    = CONSOLE_MAXWIDTH;
        STEP_yaxis  = mSTEP_yaxis;
    }

    /* Add new data. */
    void AddData (std::string x, double y) {
        PRE_CMP = -inf;
        if (CAG_COLCNT == CAG_NUMCOL) {
            PRE_CMP = data[0].y;
            for (size_t i = 1; i < CAG_NUMCOL; ++i)
                data[i - 1] = data[i];
            --CAG_COLCNT;
        }
        data[CAG_COLCNT++] = __cag_db__(x, y);
    }

    /* Constructor. */
    __currency_alarm_graph__ (unsigned int CAG_START_x,
                              unsigned int CAG_START_y,
                              unsigned int CAG_YLEN,
                              unsigned int CAG_XLEN,
                              unsigned int CAG_COL_WIDTH,
                              unsigned int CAG_NUMCOL,
                              const std::string& yaxis,
                              int MIN_yaxis,
                              int MAX_yaxis,
                              int mSTEP_yaxis,
                              int STEP_xaxis) {
        if (!CAG_START_x || !CAG_START_y)
            halt(1, "CAG: Start position must have x-coor and y-coor greater than or equal to 1!");
        if (!STEP_yaxis || !STEP_xaxis)
            halt(1, "CAG: STEP must be greater than 1!");
        this->CAG_START_x   = CAG_START_x;
        this->CAG_START_y   = CAG_START_y + 2;
        this->CAG_YLEN      = CAG_YLEN;
        this->CAG_XLEN      = CAG_XLEN + 1;
        this->CAG_COL_WIDTH = CAG_COL_WIDTH;
        this->CAG_NUMCOL    = CAG_NUMCOL;
        this->yaxis         = yaxis;
        this->MIN_yaxis     = MIN_yaxis;
        this->MAX_yaxis     = MAX_yaxis;
        this->mSTEP_yaxis   = mSTEP_yaxis;
        this->STEP_xaxis    = STEP_xaxis;
        ClearData();
    }

    /* Re-adjust the MIN_yaxis and MAX_yaxis value for adapting graph requirements.
       If the graph is somehow out of range, the function will automatically fix the STEP_yaxis value.
       Successively, I will beautify your graph by fixing the graph depending on your average data value.
       Note that: This function will be automatically called before printing out the graph.
    */
    void FixYAxis() {
        // TODO:
        // 1. Get the sum of data values.
        // 2. Calculate the average value.
        // 3. Choose a center position on y-axis and update two MIN and MAX values.
        // 4. Fixing the STEP_yaxis so it will fit all data.

		// Empty data.
		if (!CAG_COLCNT)
			halt(1, "CAG: Empty data!");

        // Average value.
        double sumofdata = 0,
               minofdata = inf,
               maxofdata = -inf;
        for (size_t i = 0; i < CAG_COLCNT; ++i) {
            sumofdata += data[i].y;
            minofdata  = std::min(minofdata, data[i].y);
            maxofdata  = std::max(maxofdata, data[i].y);
        }
        sumofdata /= CAG_COLCNT;

        // Centering.
        int center = CAG_YLEN / 2;
        int cetost = center,
            cetoma = CAG_YLEN - center;

        // Adjust STEP_yaxis.
        int old_STEP_yaxis = STEP_yaxis;
        STEP_yaxis = mSTEP_yaxis;
        int minyneedv = floor((sumofdata - floor(minofdata) + STEP_yaxis - 1) / STEP_yaxis);
        while (minyneedv >= cetost) {
            ++STEP_yaxis;
            minyneedv = floor((sumofdata - floor(minofdata) + STEP_yaxis - 1) / STEP_yaxis);
        }
        int maxyneedv = floor((ceil(maxofdata) - sumofdata + STEP_yaxis - 1) / STEP_yaxis);
        while (maxyneedv >= cetoma) {
            ++STEP_yaxis;
            maxyneedv = floor((ceil(maxofdata) - sumofdata + STEP_yaxis - 1) / STEP_yaxis);
        }

        // New value!
        bool ret = 0;
        if (int(sumofdata - cetost * STEP_yaxis) != MIN_yaxis)
            ret = 1;
        if (int(sumofdata + cetoma * STEP_yaxis) != MAX_yaxis)
            ret = 1;

        if (ret)
            Log(rfmt("[%s] Auto-fix vertical border: min=%d->%d, max=%d->%d, step=%d->%d",
                     dt().c_str(),
                     MIN_yaxis,
                     int(sumofdata - cetost * STEP_yaxis),
                     MAX_yaxis - 1,
                     int(sumofdata + cetoma * STEP_yaxis) - 1,
                     old_STEP_yaxis,
                     STEP_yaxis));
        MIN_yaxis = sumofdata - cetost * STEP_yaxis;
        MAX_yaxis = sumofdata + cetoma * STEP_yaxis;
    }

    /* Check for out of bounds on x-axis. */
    void CheckXAxis() {
		// Empty data.
		if (!CAG_COLCNT)
			halt(1, "CAG: Empty data!");

        unsigned int need = CAG_COLCNT * (CAG_COL_WIDTH + STEP_xaxis) - STEP_xaxis;
        if (need > CAG_XLEN)
            halt(1, rfmt("CAG: Out of bound on x-axis [need:%d, allowed:%d]", need - 1, CAG_XLEN - 1));
    }

    /* Print out the graph. */
    void Print() {
        // TODO:
        // 1. Clear space for the graph,
        // 2. Create the borders,
        // 3. Create Oxy axis,
        // 4. Write numbers and notes on x-axis,
        // 5. Write numbers on y-axis, draw columns and write numbers on each column.

        // Preparation.
        CheckXAxis();
        FixYAxis();

        // Initalization.
        unsigned int msize       = std::max(cdigit(MAX_yaxis), cdigit(MIN_yaxis));
        unsigned int CAG_XLEN    = this->CAG_XLEN + xaxis.length();

        // Clear space (except borders).
        textcolor(WHITE);
        for (unsigned int i = CAG_START_x + 1; i <= PRE_XLEN; ++i)
            for (unsigned int j = CAG_START_y - 2; j <= CAG_START_y + CAG_YLEN + 2; ++j) {
                SetPosition(i, j);
                std::cout << ' ';
            }

        // Set previous CAG_XLEN.
        if (CAG_START_x + CAG_XLEN + msize + 3 != PRE_XLEN)
            Log(rfmt("[%s] Auto-fix width: old=%d, new=%d", dt().c_str(), PRE_XLEN, CAG_START_x + CAG_XLEN + msize + 3));
        PRE_XLEN = CAG_START_x + CAG_XLEN + msize + 3;

        // Borders.
        textcolor(RED);
        for (unsigned int i = CAG_START_x; i <= CAG_START_x + CAG_XLEN + msize + 3; ++i) {
            SetPosition(i, CAG_START_y - 3);
            std::cout << '_';
            SetPosition(i, CAG_START_y + CAG_YLEN + 1);
            std::cout << '_';
        }
        for (unsigned int i = CAG_START_y - 2; i <= CAG_START_y + CAG_YLEN + 1; ++i) {
            SetPosition(CAG_START_x, i);
            std::cout << '|';
            SetPosition(CAG_START_x + CAG_XLEN + msize + 3, i);
            std::cout << '|';
        }

        unsigned int CAG_START_x = this->CAG_START_x + msize + 1;

        // Create the Oxy.
        textcolor(WHITE);
        for (unsigned int i = CAG_START_x + 2; i <= CAG_START_x + CAG_XLEN; ++i) {
            SetPosition(i, CAG_START_y + CAG_YLEN - 1);
            std::cout << '_';
        }
        for (unsigned int i = CAG_START_y; i <= CAG_START_y + CAG_YLEN - 1; ++i) {
            SetPosition(CAG_START_x + 1, i);
            std::cout << '|';
        }

        // Write notes.
        SetPosition(CAG_START_x + 1 - yaxis.length() / 2, CAG_START_y - 1);
        std::cout << yaxis;
        SetPosition(CAG_START_x + CAG_XLEN - xaxis.length() + 1, CAG_START_y + CAG_YLEN);
        std::cout << xaxis;

        // Write numbers on y-axis.
        int j = MIN_yaxis;
        for (unsigned int i = CAG_START_y + CAG_YLEN - 1; j <= MAX_yaxis && i >= CAG_START_y; --i, j += STEP_yaxis) {
            std::string t = to_string(j);
            SetPosition(CAG_START_x - t.length() + 1, i);
            std::cout << t;
        }

        // Put in graph data and finish.
        j = CAG_START_x + 1;
        for (size_t i = 0; i < CAG_COLCNT; ++i, j += CAG_COL_WIDTH + STEP_xaxis) {
            // Increasing, decreasing or equal color.
            if (data[i].y > PRE_CMP)
                textcolor(GREEN);
            else if (data[i].y == PRE_CMP)
                textcolor(DARKTEAL);
            else
                textcolor(YELLOW);

            // Write numbers on x-axis.
            SetPosition(j + (CAG_COL_WIDTH - data[i].x.length()) / 2, CAG_START_y + CAG_YLEN);
            std::cout << data[i].x;

            // Check for errors.
            if (data[i].y < MIN_yaxis || data[i].y > MAX_yaxis)
                halt(1, "CAG: Data must be in given range [MIN, MAX].");

            // Calculate the height.
            unsigned int height = round((data[i].y - MIN_yaxis) / STEP_yaxis);

            // Build horizontal borders.
            for (unsigned int k = j + (i == 0); k <= j + CAG_COL_WIDTH - 1; ++k) {
                SetPosition(k, CAG_START_y + CAG_YLEN - height - 1);
                std::cout << '_';
                SetPosition(k, CAG_START_y + CAG_YLEN - 1);
                std::cout << '_';
            }

            // Build vertical borders.
            for (unsigned int k = 1; k <= height; ++k) {
                unsigned int l = CAG_START_y + CAG_YLEN - k;
                if (i) {
                    SetPosition(j, l);
                    std::cout << '|';
                }
                SetPosition(j + CAG_COL_WIDTH - 1, l);
                std::cout << '|';
            }

            // Color the column.
            for (unsigned int k = 1; k <= height; ++k) {
                unsigned int l = CAG_START_y + CAG_YLEN - k;
                for (unsigned int z = j + 1; z <= j + CAG_COL_WIDTH - 2; ++z) {
                    SetPosition(z, l);
                    std::cout << '/';
                }
            }

            // Write number on column.
            SetPosition(j + (CAG_COL_WIDTH - cdigit(data[i].y)) / 2, CAG_START_y + CAG_YLEN - height - 1);
            std::cout << data[i].y;

            // Reset the PRE_CMP.
            PRE_CMP = data[i].y;
        }

        // Reset the color.
        textcolor(WHITE);
    }
};

typedef __currency_alarm_graph__ cag;

#endif /* __GRAPH_H__ */

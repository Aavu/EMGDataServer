//
// Created by Raghavasimhan Sankaranarayanan on 7/16/20.
//

#ifndef EMGDATASERVER_TIME_H
#define EMGDATASERVER_TIME_H

#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std::chrono_literals;


class Time {
public:
    static const uint8_t timeStampSize = 20;

    [[maybe_unused]] static void sleep(int seconds) {
        std::this_thread::sleep_for(::seconds(seconds));
    }

    [[maybe_unused]] static void msleep(int milli) {
        std::this_thread::sleep_for(::milliseconds(milli));
    }

    [[maybe_unused]] static void usleep(int micro) {
        std::this_thread::sleep_for(::microseconds(micro));
    }

    static std::string getTimeStamp() {
        auto now = ::system_clock::now();
        auto ms = ::duration_cast<::milliseconds>(now.time_since_epoch()) % 1000;
        auto in_time_t = ::system_clock::to_time_t(now);
        auto outTime = std::localtime(&in_time_t);
        sprintf(timeStamp, "%04d%02d%02d%02d%02d%02d%04lld", outTime->tm_year, outTime->tm_mon, outTime->tm_mday, outTime->tm_hour, outTime->tm_min, outTime->tm_sec, ms.count());
        return std::string(timeStamp);
    }

    static void getTimeStamp(uint16_t* piTimeStamp) {
        auto now = ::system_clock::now();
        auto ms = ::duration_cast<::milliseconds>(now.time_since_epoch()) % 1000;
        auto in_time_t = ::system_clock::to_time_t(now);
        auto outTime = std::localtime(&in_time_t);
        piTimeStamp[0] = outTime->tm_year;
        piTimeStamp[1] = outTime->tm_mon;
        piTimeStamp[2] = outTime->tm_mday;
        piTimeStamp[3] = outTime->tm_hour;
        piTimeStamp[4] = outTime->tm_min;
        piTimeStamp[5] = outTime->tm_sec;
        piTimeStamp[6] = ms.count();
        piTimeStamp[7] = 0;
    }

private:
    inline static char timeStamp[timeStampSize];

};
#endif //EMGDATASERVER_TIME_H

//
// Created by Raghavasimhan Sankaranarayanan on 7/16/20.
//

#ifndef EMGDATASERVER_TIME_H
#define EMGDATASERVER_TIME_H

#include <chrono>

using namespace std::chrono;
using namespace std::chrono_literals;


class Time {
public:
    static const uint8_t timeStampSize = 12;

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
        sprintf(timeStamp, "%02d%02d%02d%02d%03lld", outTime->tm_mday, outTime->tm_hour, outTime->tm_min, outTime->tm_sec, ms.count());
        return std::string(timeStamp);
    }

private:
    inline static char timeStamp[timeStampSize];

};
#endif //EMGDATASERVER_TIME_H

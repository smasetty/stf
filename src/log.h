#include <iostream>
#include <chrono>
#include <ctime>
#include "test_framework.h"

/*! \enum logLevel
 *
 *  Detailed description
 */
enum logLevel {
    LOG_HIGH = 0x0,
    LOG_DBEUG = 0x1,
    LOG_LOW = 0x2,
    LOG_VERBOSE = 0x4
};

class Logger {
public:

    void SetOutputStream(std::ostream *out) {
        outputStream = out;
    }

    explicit Logger() {};

    const char* LogLevelToString(int logLevel) 
    {
        if (logLevel == LOG_HIGH)
            return "HIGH";
        else if (logLevel == LOG_LOW)
            return "LOW";
        else if (logLevel == LOG_DBEUG)
            return "DEBUG";
        else if (logLevel == LOG_VERBOSE)
            return "VERBOSE";
        else
            return "UNKNOWN";
    }

    std::ostream& GetLog(int logLevel);
    ~Logger() {};

private:
    std::ostream *outputStream;
    Logger(const Logger&);
    Logger operator=(const Logger&);
};

std::ostream& Logger::GetLog(int logLevel)
{
    std::ostream& out = *outputStream;
    auto timeNow = std::chrono::system_clock::now();
    std::time_t timeStamp = std::chrono::system_clock::to_time_t(timeNow);

    std::string timeStampStr(std::ctime(&timeStamp));
    timeStampStr.pop_back();

    out <<  timeStampStr << " - " << LogLevelToString(logLevel) << " - ";
    return *outputStream;
}

/**
 * @file pitchMsgFactory.cpp
 * @author Derek Zhang
 * @brief PITCH Message Factory class
 * @version 0.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "pitchMsgFactory.h"

#include <chrono>

PitchMessage PitchMsgFactory::createPitchMsg(MSG_TYPE msgType)
{
    switch (msgType) {
        case MSG_TYPE::ADD:
            return PitchMessage("A", addOrderFields);
            break;
        case MSG_TYPE::CANCEL:
            return PitchMessage("X", cancelOrderFields);
            break;
        case MSG_TYPE::EXECUTE:
            return PitchMessage("E", execOrderFields);
            break;
        case MSG_TYPE::TRADE:
            return PitchMessage("P", tradeMsgFields);
            break;
    }
}

std::string PitchMsgFactory::getTimestampStr()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local = *std::localtime(&time);

    long msSinceMidnight =
        local.tm_hour * 3600000 +
        local.tm_min  * 60000 +
        local.tm_sec  * 1000 +
        duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << msSinceMidnight;
    return oss.str();
}

std::string PitchMsgFactory::getPriceStr(double price)
{
    long long scaledPrice = static_cast<long long>(std::round(price * 10000));
    std::ostringstream oss;
    oss << std::setw(10) << std::setfill('0') << scaledPrice;
    return oss.str();
}

std::string PitchMsgFactory::getSharesStr(int shares)
{
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << shares;
    return oss.str();
}
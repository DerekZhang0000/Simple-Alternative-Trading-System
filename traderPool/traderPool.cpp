/**
 * @file traderPool.cpp
 * @author Derek Zhang
 * @brief The Trade Pool is responsible for simulating multiple traders placing orders
 * @version 0.1
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "traderPool.h"

#include <stdexcept>

TraderPool::TraderPool(std::vector<TRADER_TYPE> traderBots, std::vector<std::unordered_map<std::string, std::string>> paramList)
{
    
}

void TraderPool::incrementBase36(std::string & s)
{
    if (s.length() != 12) {
        throw std::invalid_argument("String must be 12 characters long");
    }

    for (int i = 11; i >= 0; --i) {
        char &c = s[i];
        if (c >= '0' && c <= '8') {
            c += 1;
            return;
        } else if (c == '9') {
            c = 'A';
            return;
        } else if (c >= 'A' && c <= 'Y') {
            c += 1;
            return;
        } else if (c == 'Z') {
            c = '0';
        } else {
            throw std::invalid_argument("Invalid base-36 character in string");
        }
    }

    throw std::overflow_error("Base-36 string overflow");
}

std::string TraderPool::getNextOrderID()
{
    incrementBase36(orderId);
    return orderId;
}
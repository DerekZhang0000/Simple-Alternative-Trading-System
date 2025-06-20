/**
 * @file traderBot.cpp
 * @author The Trader Bot class will simulate a trader
 * @brief 
 * @version 0.1
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "traderBot.h"

/**
 * @brief TraderBot (parent class) functions
 * 
 */

void TraderBot::removeNullOrders()
{
    orders.remove_if([](Order* order) {
        return order == nullptr;
    });
}

TraderBot& TraderBot::setParameter(std::string key, double value)
{
    params[key] = value;
    return *this;
}
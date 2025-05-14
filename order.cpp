/**
 * @file order.cpp
 * @author Derek Zhang
 * @brief The Order class will represent outstanding orders on a matching engine's order book
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "order.h"

Order::Order(int newShares, int newPrice, char newSide, int newTimestamp)
{
    shares = newShares;
    price = newPrice;
    side = newPrice;
    timestamp = newTimestamp;
}

int Order::tradeShares(int shareDelta)
{
    shares -= shareDelta;
    if (shares == 0) {
        delete this;
        return 0;
    } else if (shares < 0) {
        delete this;
        return -shares;
    }
}

bool Order::operator<(const Order& otherOrder) const
{
    return timestamp > otherOrder.timestamp;    // Greater timestamps are younger, and have lower priority
}
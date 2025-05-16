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
    orderShares = newShares;
    orderPrice = newPrice;
    orderSide = newPrice;
    orderTimestamp = newTimestamp;
}

int Order::tradeShares(int shareDelta)
{
    orderShares -= shareDelta;
    if (orderShares == 0) {
        return 0;
    } else if (orderShares < 0) {
        return -orderShares;
    }
}

bool Order::operator<(const Order& otherOrder) const
{
    return orderTimestamp > otherOrder.orderTimestamp;    // Greater timestamps are younger, and have lower priority
}

int Order::price() const
{
    return orderPrice;
}

int Order::shares() const
{
    return orderShares;
}

char Order::side() const
{
    return orderSide;
}

int Order::timestamp() const
{
    return orderTimestamp;
}
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

Order::Order(std::string newSymbol, int newShares, double newPrice, char newSide, std::string newId)
{
    orderSymbol = newSymbol;
    orderShares = newShares;
    orderPrice = newPrice;
    orderSide = newSide;
    orderId = newId;
}

void Order::tradeShares(int shareDelta)
{
    orderShares -= shareDelta;
}

std::string Order::symbol() const
{
    return orderSymbol;
}

double Order::price() const
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

std::string Order::id() const
{
    return orderId;
}
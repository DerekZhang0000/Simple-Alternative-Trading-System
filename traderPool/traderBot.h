/**
 * @file traderBot.h
 * @author Derek Zhang
 * @brief Trader Bot header file
 * @version 0.1
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef TRADERBOT_H
#define TRADERBOT_H

#include "order.h"

#include <string>
#include <list>
#include <functional>

class TraderBot {
    protected:
    std::list<Order*> orders;
    std::function<void(double, double)> tradingStrategy;

    public:
    /**
     * @brief Removes null pointers in the Order list
     * 
     */
    void removeNullOrders();
};

class GaussianBot : public TraderBot {

};

class MarketMakerBot : public TraderBot {

};

class SpooferBot : public TraderBot {

};

#endif
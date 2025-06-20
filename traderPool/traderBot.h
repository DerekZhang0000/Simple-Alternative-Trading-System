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
#include <unordered_map>

int BUY = 0;
int SELL = 1;

class TraderBot {
    protected:
    std::list<Order*> orders;
    std::unordered_map<std::string, double> params;

    public:
    TraderBot() = default;
    ~TraderBot() = default;

    /**
     * @brief Removes null pointers in the Order list
     * 
     */
    void removeNullOrders();

    /**
     * @brief Sets a parameter and returns itself
     * 
     * @param key 
     * @param value 
     * @return TraderBot& 
     */
    TraderBot& setParameter(std::string key, double value);

    /**
     * @brief Returns a list of Add Order messages
     * 
     * @param orderIDs 
     * @return std::string 
     */
    virtual std::vector<std::string> createOrderMessages(std::vector<std::string> orderIDs) = 0;
};

class GaussianBot : public TraderBot {
    public:
    std::vector<std::string> createOrderMessages(std::vector<std::string> orderIDs);
};

class MarketMakerBot : public TraderBot {
    public:
    std::vector<std::string> createOrderMessages(std::vector<std::string> orderIDs);
};

class TradeMessenger : public TraderBot {
    public:
    std::vector<std::string> createOrderMessages(std::vector<std::string> orderIDs);
};

#endif
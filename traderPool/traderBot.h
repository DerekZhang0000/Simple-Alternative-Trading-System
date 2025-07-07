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
#include "pitchMsgFactory.h"

#include <string>
#include <list>
#include <unordered_map>
#include <random>
#include <functional>

class TraderBot {
    protected:
    std::list<Order*> orders;
    std::unordered_map<std::string, std::string> params;
    PitchMsgFactory pitchMsgFactory = PitchMsgFactory();

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
     * @brief Gets a parameter
     * 
     * @param param 
     * @return std::string 
     */
    std::string getParameter(std::string param);

    /**
     * @brief Pushes an Order to the end of the list
     * 
     * @param order 
     */
    void pushOrder(Order* order);

    /**
     * @brief Returns a list of Add Order messages using the PITCH protocol
     * 
     * @param orderIDs 
     * @return std::string 
     */
    virtual std::vector<PitchMessage*> createPitchMsgs(std::vector<std::string> orderIDs) = 0;
};

class GaussianBot : public TraderBot {
    // Requires Mean, Deviation, Side, Shares, Symbol, Display
    private:
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<double> uniform{0.0, 1.0};
    std::function<double(void)> gauss_dist = nullptr;

    public:
    std::vector<PitchMessage*> createPitchMsgs(std::vector<std::string> orderIDs);
};

class MarketMakerBot : public TraderBot {
    // Requires Side, Shares, Symbol, Price, Display
    public:
    std::vector<PitchMessage*> createPitchMsgs(std::vector<std::string> orderIDs);
};

class TradeMessenger : public TraderBot {
    // Requires Mean, Deviation, Side, Shares, Symbol, Price
    private:
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::function<double(void)> gauss_dist = nullptr;

    public:
    std::vector<PitchMessage*> createPitchMsgs(std::vector<std::string> orderIDs);
};

#endif
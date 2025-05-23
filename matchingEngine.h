/**
 * @file matchingEngine.h
 * @author Derek Zhang
 * @brief Matching Engine header file
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef MATCHINGENGINE_H
#define MATCHINGENGINE_H

#include "order.h"
#include "orderQueue.h"
#include "pitchMessage.h"

#include <string>
#include <boost/container/flat_map.hpp>
#include <unordered_map>
#include <optional>

typedef std::unordered_map<std::string, boost::container::flat_map<double, OrderQueue>> Book;

class MatchingEngine {
    private:
    Book buyBook = {};
    Book sellBook = {};
    std::unordered_map<std::string, Order*> idMap = {};

    public:
    MatchingEngine() = default;
    ~MatchingEngine();

    /**
     * @brief Ingests a message and performs a corresponding action
     * 
     * @param msg 
     */
    void ingestMessage(PitchMessage const &msg);

    /**
     * @brief Adds an order to the order book, then executes trades if possible
     * 
     * @param msg 
     */
    void addOrder(PitchMessage const &msg);
    
    /**
     * @brief Cancels a certain number of shares in an order
     * 
     * @param msg 
     */
    void cancelOrder(PitchMessage const &msg);

    /**
     * @brief Forwards a trade message to data collection service
     * 
     * @param msg 
     */
    void forwardTrade(PitchMessage const &msg);
    
    /**
     * @brief Attempts to make trades on a symbol for matching orders on the buy and sell sides, returning an Executed Order PITCH message if successful
     * 
     * @param symbol 
     * @return PitchMessage 
     */
    inline std::optional<PitchMessage> attemptTrade(Order* incomingOrder, std::string const &symbol); 
};

#endif
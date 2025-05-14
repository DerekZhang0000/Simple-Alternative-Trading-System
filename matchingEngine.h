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
#include <unordered_map>
#include <queue>

class MatchingEngine {
    private:
    std::unordered_map<std::string, std::priority_queue<OrderQueue>> buyBook = {};
    std::unordered_map<std::string, std::priority_queue<OrderQueue>> sellBook = {};

    public:
    /**
     * @brief Adds an order to the order book, then executes trades if possible
     * 
     * @param msg 
     */
    void ingestMessage(PitchMessage msg);
};

#endif
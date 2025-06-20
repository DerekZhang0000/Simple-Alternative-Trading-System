/**
 * @file traderPool.h
 * @author Derek Zhang
 * @brief Trader Pool header file
 * @version 0.1
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef TRADERPOOL_H
#define TRADERPOOL_H

#include "traderBot.h"

#include <string>
#include <vector>

class TraderPool {
    private:
    std::string orderId = "000000000000"; // Base 36 string that keeps track of unique execution IDs
    std::vector<TraderBot*> traders;

    /**
     * @brief Increments a base 64 string of length 12 in place
     * 
     * @param base36Str 
     */
    void incrementBase36(std::string & s);

    public:

    /**
     * @brief Gets the next Order ID
     * 
     * @return std::string 
     */
    std::string getNextOrderID();
};

#endif
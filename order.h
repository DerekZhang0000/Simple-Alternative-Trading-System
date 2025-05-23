/**
 * @file order.h
 * @author Derek Zhang
 * @brief Order class header file
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order
{
    private:
    int orderShares;
    int orderPrice;     // value in cents
    char orderSide;     // B or S
    int orderTimestamp; // milliseconds since midnight
    std::string id;

    public:
    /**
     * @brief Construct a new Order object
     * 
     * @param shares 
     * @param price
     * @param side 
     * @param timestamp 
     */
    Order(int shares, int price, char side);
    ~Order() = default;

    int price()         const;
    int shares()        const;
    char side()         const;
    int timestamp()     const;

    /**
     * @brief Modifies the share count, returning the difference if more shares were traded than exist
     * @param shareDelta 
     * @return int 
     */
    int tradeShares(int shareDelta);

};

#endif
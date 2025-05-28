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
    double orderPrice;      // value in cents
    char orderSide;         // B or S
    std::string orderId;    // id string

    public:
    /**
     * @brief Construct a new Order object
     * 
     * @param shares 
     * @param price 
     * @param side 
     * @param orderId 
     */
    Order(int shares, double price, char side, std::string orderId);
    ~Order() = default;

    double price()      const;
    int shares()        const;
    char side()         const;

    /**
     * @brief Decreases the Order share count
     * @param shareDelta 
     * @return int 
     */
    void tradeShares(int shareDelta);

};

#endif
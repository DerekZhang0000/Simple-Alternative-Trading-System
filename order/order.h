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
    std::string orderSymbol;
    int orderShares;
    double orderPrice;      // value in cents
    char orderSide;         // B or S
    std::string orderId;    // id string

    public:
    /**
     * @brief Construct a new Order object
     * 
     * @param symbol
     * @param shares 
     * @param price 
     * @param side 
     * @param orderId 
     */
    Order(std::string symbol, int shares, double price, char side, std::string orderId);
    ~Order() = default;

    std::string symbol()    const;
    double price()          const;
    int shares()            const;
    char side()             const;
    std::string id()        const;

    /**
     * @brief Decreases the Order share count
     * @param shareDelta 
     */
    void tradeShares(int shareDelta);

};

#endif
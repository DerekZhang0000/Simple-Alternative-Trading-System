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

class Order
{
    private:
    int shares;
    int price;      // value in cents
    char side;      // B or S
    int timestamp;  // milliseconds since midnight

    public:
    /**
     * @brief Construct a new Order object
     * 
     * @param shares 
     * @param price 
     * @param side 
     * @param timestamp 
     */
    Order(int shares, int price, char side, int timestamp);
    ~Order() = default;

    /**
     * @brief Modifies the share count, returning the difference if more shares were traded than exist
     * 
     * @param shareDelta 
     * @return int 
     */
    int tradeShares(int shareDelta);

    /**
     * @brief Overloaded comparison operator to sort by time priority
     * 
     * @param otherOrder 
     * @return true 
     * @return false 
     */
    bool operator<(const Order& otherOrder) const;

};

#endif
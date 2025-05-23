/**
 * @file orderQueue.h
 * @author Derek Zhang
 * @brief Order Queue header file
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include "order.h"
#include <vector>
#include <deque>

class OrderQueue
{
    private:
    char side;          // B or S
    double queuePrice;  // value in USD
    std::deque<Order*> orderQueue = {};

    public:
    /**
     * @brief Construct a new Order Queue object with the price
     * 
     * @param price
     */
    OrderQueue(double& queuePrice);
    ~OrderQueue() = default;

    /**
     * @brief Returns the price of the orders in the queue
     * 
     * @return int 
     */
    int price() const;

    /**
     * @brief Insert an order object into the queue
     * 
     * @param order 
     */
    void insertOrder(Order* orderPtr);

    /**
     * @brief Modify the number of shares on an outstanding order
     * 
     * @param orderId 
     */
    void modifyShares(std::string orderId, int newShares);

    /**
     * @brief Deletes an order, then deletes itself if queue is empty
     * 
     * @param orderId 
     */
    void deleteOrder(std::string orderId);

    /**
     * @brief Returns whether or not the queue is empty
     * 
     * @return int 
     */
    int empty() const;
};

#endif
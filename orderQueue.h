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
#include <queue>

class OrderQueue
{
    private:
    char side;      // B or S
    int queuePrice; // value in cents
    std::priority_queue<Order*> orderQueue = {};

    public:
    /**
     * @brief Construct a new Order Queue object with a new Order object
     * 
     * @param order 
     */
    OrderQueue(Order* orderPtr);

    /**
     * @brief Destroy the Order Queue object
     * 
     */
    ~OrderQueue();

    /**
     * @brief Returns the price of the orders in the queue
     * 
     * @return int 
     */
    int price() const;

    /**
     * @brief Overloaded comparison operator to sort by price priority
     * 
     * @param otherQueue 
     * @return true 
     * @return false 
     */
    bool operator<(const OrderQueue &otherQueue) const;

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
    int OrderQueue::empty() const;
};

#endif
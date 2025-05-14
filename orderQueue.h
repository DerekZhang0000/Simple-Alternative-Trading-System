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
    char side;  // B or S
    int price;  // value in cents
    std::priority_queue<Order> orderQueue = {};

    public:
    /**
     * @brief Overloaded comparison operator to sort by price priority
     * 
     * @param otherQueue 
     * @return true 
     * @return false 
     */
    bool operator<(const OrderQueue& otherQueue) const;

    /**
     * @brief Insert an order object into the queue
     * 
     * @param order 
     */
    void insert(Order order);

    /**
     * @brief Returns whether or not the queue is empty
     * 
     * @return int 
     */
    int OrderQueue::empty() const;
};

#endif
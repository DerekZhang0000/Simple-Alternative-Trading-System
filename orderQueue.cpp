/**
 * @file orderQueue.cpp
 * @author Derek Zhang
 * @brief This class represents the queue of outstanding orders for a specific price
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "orderQueue.h"
#include <stdexcept>

OrderQueue::OrderQueue(Order* orderPtr)
{
    insertOrder(orderPtr);
}

OrderQueue::~OrderQueue()
{
    while (!orderQueue.empty()) {
        Order* qTop = orderQueue.top();
        if (qTop != nullptr) {
            qTop->~Order();
        }
    }
}

bool OrderQueue::operator<(const OrderQueue &otherQueue) const
{
    if (side == 'B') {
        return queuePrice < otherQueue.queuePrice;    // Higher buy prices are prioritized for matching an incoming sell order
    } else if (side == 'S') {
        return queuePrice > otherQueue.queuePrice;    // Lower sell prices are prioritized for matching an incoming buy order
    }

    throw std::runtime_error("Invalid side for Order Queue object.");
}

void OrderQueue::insertOrder(Order* orderPtr)
{
    orderQueue.push(orderPtr);
}

int OrderQueue::empty() const
{
    return orderQueue.empty();
}

int OrderQueue::price() const
{
    return queuePrice;
}
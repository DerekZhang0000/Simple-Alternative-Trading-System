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

bool OrderQueue::operator<(const OrderQueue& otherQueue) const
{
    if (side == 'B') {
        return price < otherQueue.price;    
    } else if (side == 'S') {
        return price > otherQueue.price;
    }

    throw std::runtime_error("Invalid side for Order Queue object.");
}

void OrderQueue::insert(Order order)
{
    orderQueue.push(order);
}

int OrderQueue::empty() const
{
    return orderQueue.empty();
}
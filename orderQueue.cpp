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

OrderQueue::OrderQueue(double& price)
{
    this->queuePrice = price;
}

void OrderQueue::insertOrder(Order* orderPtr)
{
    orderQueue.push_back(orderPtr);
}

int OrderQueue::empty() const
{
    return orderQueue.empty();
}

int OrderQueue::price() const
{
    return queuePrice;
}
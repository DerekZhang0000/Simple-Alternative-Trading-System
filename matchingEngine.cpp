/**
 * @file matchingEngine.cpp
 * @author Derek Zhang
 * @brief The Matching Engine class is responsible for matching orders 
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "matchingEngine.h"
#include "order.h"

#include <stdexcept>

typedef std::unordered_map<std::string, std::priority_queue<OrderQueue>>::iterator BookIterator;

MatchingEngine::~MatchingEngine()
{
    for (auto & pair : idMap) {
        if (pair.second != nullptr) {
            pair.second->~Order();
        }
    }
    idMap.clear();
}

void MatchingEngine::addOrder(PitchMessage const &msg)
{
    std::string symbol = msg.symbol();
    char side = msg.side();
    Order* newOrder = new Order(msg.shares(), msg.price(), side, msg.timestamp());

    BookIterator bookIt;
    if (side == 'B') {
        bookIt = buyBook.find(symbol);
    } else if (side == 'S') {
        bookIt = sellBook.find(symbol);
    } else {
        throw std::runtime_error("Unexpected side for Matching Engine Add Order ingestion.");
    }

    if (bookIt == buyBook.end()) {
        buyBook.emplace(symbol, OrderQueue(newOrder));
    } else {
        buyBook[symbol].push(newOrder);
    }

    attemptTrade(newOrder, symbol);
}

std::optional<PitchMessage> MatchingEngine::attemptTrade(Order* incomingOrder, std::string const &symbol)
{
    char const &side = incomingOrder->side();
    BookIterator bookIt;
    if (side == 'B') {  // The opposite book is where the order will try to be matched, hence why lower sale prices and higher buy prices are prioritized
        bookIt = sellBook.find(symbol);
        if (bookIt == sellBook.end()) {
            return std::nullopt;
        }
        
        OrderQueue const &bestSellQueue(bookIt->second.top())
        int const &bestSellPrice = bestSellQueue.price();
        while (bestSellPrice >= incomingOrder->price() && incomingOrder->shares() > 0) {
            Order* firstSellOrder bookIt
        }
    } else {
        bookIt = buyBook.find(symbol);
        if (bookIt == buyBook.end()) {
            return std::nullopt;
        }
    }   // No error checking required since symbol is passed in after check
}

void MatchingEngine::cancelOrder(PitchMessage const &msg)
{

}

void MatchingEngine::forwardTrade(PitchMessage const &msg)
{

}

void MatchingEngine::ingestMessage(PitchMessage const &msg)
{
    char msgType = msg.type();
    switch (msgType)
    {
    case 'A':
        addOrder(msg);
        break;

    case 'X':
        cancelOrder(msg);
        break;

    // case 'E':
        // Execute order messages are sent, not received
        // break;

    case 'P':
        forwardTrade(msg);  // Trade messages are passed through
        break;

    default:
        throw std::runtime_error("Unexpected message type in PITCH message ingestion.");
        break;
    }
}


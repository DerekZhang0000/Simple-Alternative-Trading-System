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

MatchingEngine::~MatchingEngine()
{
    for (auto & pair : idMap) {
        if (pair.second != nullptr) {
            pair.second->~Order();
        }
    }
    idMap.clear();
}

BookIterator MatchingEngine::locateBook(std::string const & symbol, char side)
{
    if (side == 'B') {
        return buyBook.find(symbol);
    } else if (side == 'S') {
        return sellBook.find(symbol);
    } else [[unlikely]] {
        throw std::runtime_error("Unexpected side for book location.");
    }
}

void MatchingEngine::populateSymbols(std::vector<std::string> const & symbolList)
{
    for (std::string const & symbol : symbolList) {
        buyBook.emplace(new PriceMap());
        sellBook.emplace(new PriceMap());
    }
}

void MatchingEngine::addOrder(PitchMessage const & msg)
{
    std::string symbol = msg.symbol();
    char side = msg.side();
    double price = msg.price();
    Order* newOrder = new Order(msg.shares(), price, side, msg.id());

    std::optional<Order*> revisedOrder = attemptTrade(newOrder, symbol); // attempt trade, if no trade occurs or leftover shares, insert
    if (revisedOrder == std::nullopt) {
        return;
    }

    if (side == 'B') {
        buyBook[symbol][price].push_back(revisedOrder.value());
    } else if (side == 'S') {
        sellBook[symbol][price].push_back(revisedOrder.value());
    } else [[unlikely]] {
        throw std::runtime_error("Unexpected side for Add Order ingestion.");
    }
}

std::optional<PriceMapIterator> MatchingEngine::matchOrder(BookIterator const & oppositeBookIt, char const side, double const limitPrice)
{
    if (side == 'B') {
        auto queueIt = oppositeBookIt->second.lower_bound(0.0);
        while (queueIt != oppositeBookIt->second.end() && queueIt->first <= limitPrice) {
            if (!queueIt->second.empty()) {
                return queueIt;
            }
            ++queueIt;
        }
    } else if (side == 'S') {
        auto queueIt = oppositeBookIt->second.upper_bound(limitPrice);
        while (queueIt != oppositeBookIt->second.begin()) {
            --queueIt;
            if (queueIt->first >= limitPrice && !queueIt->second.empty()) {
                return queueIt;
            }
        }
    }

    return std::nullopt;
}

std::optional<Order*> MatchingEngine::attemptTrade(Order* incomingOrder, std::string const & symbol)
{
    char const side = incomingOrder->side();
    double const price = incomingOrder->price();
    char const oppositeSide = side == 'B' ? 'S' : 'B';
    Book & oppositeBook = side == 'B' ? sellBook : buyBook;
    BookIterator oppositeBookIt = locateBook(symbol, oppositeSide); // The opposite book is where the order will try to be matched, hence why lower sale prices and higher buy prices are prioritized
    PriceMap & oppositePriceMap = oppositeBook[symbol];
    
    if (oppositeBookIt == oppositeBook.end()) {
        return std::nullopt;
    }

    std::optional<PriceMapIterator> priceMapIt = matchOrder(oppositeBookIt, side, price);
    while (priceMapIt != std::nullopt) {
        std::deque<Order*> & orderQueue = priceMapIt.value()->second;
        while (orderQueue.size() > 0) {
            Order*& queuedOrder = orderQueue.at(0);
            int shareDelta = std::min(queuedOrder->shares(), incomingOrder->shares());
            queuedOrder->tradeShares(shareDelta);
            incomingOrder->tradeShares(shareDelta);

            if (queuedOrder->shares() == 0) {
                delete queuedOrder;
                orderQueue.erase(orderQueue.begin());
            } else [[unlikely]] {
                throw std::runtime_error("Negative share count result in trade attempt.");
            }

            if (incomingOrder->shares() == 0) {
                return std::nullopt;
            } else if (incomingOrder->shares() < 0) [[unlikely]] {
                throw std::runtime_error("Negative share count result in trade attempt.");
            }
        }

        oppositePriceMap.erase(priceMapIt.value());
        priceMapIt = matchOrder(oppositeBookIt, side, price);
    }

    return incomingOrder;
}

void MatchingEngine::cancelOrder(PitchMessage const & msg)
{

}

void MatchingEngine::forwardTrade(PitchMessage const & msg)
{

}

void MatchingEngine::ingestMessage(PitchMessage const & msg)
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


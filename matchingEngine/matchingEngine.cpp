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
#include <chrono>

MatchingEngine::MatchingEngine(DataService* newDataServicePtr)
{
    this->dataServicePtr = newDataServicePtr;
}

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
        buyBook.emplace(symbol, PriceMap());
        sellBook.emplace(symbol, PriceMap());
    }
}

void MatchingEngine::addOrder(PitchMessage const & msg)
{
    std::string symbol = msg.symbol();
    char side = msg.side();
    double price = msg.price();
    Order* newOrder = new Order(symbol, msg.shares(), price, side, msg.id());

    std::optional<Order*> revisedOrder = attemptTrade(newOrder, symbol); // attempt trade, if no trade occurs or there are leftover shares, insert order
    if (revisedOrder == std::nullopt) {
        return;
    }

    if (!buyBook.contains(symbol)) [[unlikely]] {
        throw std::runtime_error("Unknown symbol in message.");
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
        return incomingOrder;
    }

    std::optional<PriceMapIterator> priceMapIt = matchOrder(oppositeBookIt, side, price);
    while (priceMapIt != std::nullopt) {
        std::deque<Order*> & orderQueue = priceMapIt.value()->second;
        while (orderQueue.size() > 0) {
            Order*& queuedOrder = orderQueue.at(0);
            int shareDelta = std::min(queuedOrder->shares(), incomingOrder->shares());
            queuedOrder->tradeShares(shareDelta);
            incomingOrder->tradeShares(shareDelta);
            sendExecuteMessage(queuedOrder->id(), shareDelta);

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

OrderQueue MatchingEngine::locateOrderQueue(std::string const & orderId)
{
    auto idMapIt = idMap.find(orderId);
    if (idMapIt == idMap.end()) [[unlikely]] {
        throw std::runtime_error("Order ID not found in ID map.");
    }

    Order* order = idMapIt->second;
    char side = order->side();
    Book& book = side == 'B' ? buyBook : sellBook;
    OrderQueue& queue = book[order->symbol()][order->price()];

    return queue;
}

void MatchingEngine::cancelOrder(PitchMessage const & msg)
{
    std::string const & orderId = msg.id();
    OrderQueue queue = locateOrderQueue(orderId);
    for (auto queueIt = queue.begin(); queueIt != queue.end(); ++queueIt) {
        if ((*queueIt)->id() == orderId) {
            int cancelShares = msg.shares();
            (*queueIt)->tradeShares(cancelShares);

            if ((*queueIt)->shares() <= 0) {
                queue.erase(queueIt);
            }
            return;
        }
    }

    throw std::runtime_error("Order not found in queue.");
}

void MatchingEngine::forwardTrade(PitchMessage const & msg)
{
    // Forward to data collection service when implemented
}

std::string MatchingEngine::getTimestampStr() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local = *std::localtime(&time);

    uint64_t msSinceMidnight =
        local.tm_hour * 3600000 +
        local.tm_min  * 60000 +
        local.tm_sec  * 1000 +
        duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << msSinceMidnight;
    return oss.str();
}

std::string MatchingEngine::getExecutionID()
{
    dataServicePtr->getNextExecutionID();
}

void MatchingEngine::sendExecuteMessage(std::string const & orderID, int shareDelta)
{
    auto msg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::EXECUTE);
    msg.setParameter("Timestamp", getTimestampStr())
        .setParameter("OrderID", orderID)
        .setParameter("Shares", std::to_string(shareDelta))
        .setParameter("ExecutionID", getExecutionID());

    if (dataServicePtr == nullptr) [[unlikely]] {
        return;
    }

    dataServicePtr->getQueue()->push(&msg);
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

Book& MatchingEngine::getBook(char side)
{
    if (side == 'B') {
        return buyBook;
    } else if (side == 'C') {
        return sellBook;
    } else [[unlikely]] {
        throw std::runtime_error("Unexpected side for getting book.");
    }
}
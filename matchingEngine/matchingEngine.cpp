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

MatchingEngine::MatchingEngine(DataService* newDataServicePtr)
{
    this->dataServicePtr = newDataServicePtr;
}

MatchingEngine::~MatchingEngine()
{
    for (auto & book : {std::ref(buyBook), std::ref(sellBook)}) {
        for (auto & [symbol, priceMap] : book.get()) {
            for (auto & [price, orderQueue] : priceMap) {
                for (Order* order : orderQueue) {
                    delete order;
                }
            }
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

Order* MatchingEngine::addOrder(PitchMessage const & msg)
{
    std::string id = msg.id();
    std::string symbol = msg.symbol();
    char side = msg.side();
    double price = msg.price();
    Order* newOrder = new Order(symbol, msg.shares(), price, side, id);

    if (!buyBook.contains(symbol)) [[unlikely]] {
        delete newOrder;
        throw std::runtime_error("Unknown symbol in message.");
    }

    std::optional<Order*> revisedOrderOpt = attemptTrade(newOrder, symbol); // attempt trade, if no trade occurs or there are leftover shares, insert order
    if (revisedOrderOpt == std::nullopt) {
        return nullptr;
    }

    idMap.emplace(std::make_pair(id, revisedOrderOpt.value()));
    if (side == 'B') {
        buyBook[symbol][price].push_back(revisedOrderOpt.value());
        return revisedOrderOpt.value();
    } else if (side == 'S') {
        sellBook[symbol][price].push_back(revisedOrderOpt.value());
        return revisedOrderOpt.value();
    } else [[unlikely]] {
        delete revisedOrderOpt.value();
        throw std::runtime_error("Unexpected side for Add Order ingestion.");
    }
}

std::optional<PriceMapIterator> MatchingEngine::matchOrder(BookIterator const & oppositeBookIt, char const oppositeSide, double const limitPrice)
{
    auto & priceMap = oppositeBookIt->second;
    if (priceMap.empty()) { // if there are no opposite outstanding orders at any price
        return std::nullopt;
    }

    if (oppositeSide == 'B') {  // match highest buy offer if incoming order is selling
        auto priceMapIt = std::prev(priceMap.end());
        if (priceMapIt->first >= limitPrice && !priceMapIt->second.empty()) {
            return priceMapIt;
        }
    } else if (oppositeSide == 'S') {   // match lowest sell offer if incoming order is buying
        auto priceMapIt = priceMap.begin();
        if (priceMapIt->first <= limitPrice && !priceMapIt->second.empty()) {
            return priceMapIt;
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

    std::optional<PriceMapIterator> priceMapItOpt = matchOrder(oppositeBookIt, oppositeSide, price);
    while (priceMapItOpt != std::nullopt) {
        std::deque<Order*> & orderQueue = priceMapItOpt.value()->second;
        while (orderQueue.size() > 0) {
            Order*& queuedOrder = orderQueue.at(0);
            int shareDelta = std::min(queuedOrder->shares(), incomingOrder->shares());
            queuedOrder->tradeShares(shareDelta);
            incomingOrder->tradeShares(shareDelta);
            lastPriceMap[symbol] = queuedOrder->price();
            sendExecuteMessage(queuedOrder->id(), shareDelta);

            if (queuedOrder->shares() == 0) {
                idMap.erase(queuedOrder->id());
                delete queuedOrder;
                orderQueue.erase(orderQueue.begin());
            }

            if (incomingOrder->shares() == 0) {
                idMap.erase(incomingOrder->id());
                delete incomingOrder;
                return std::nullopt;
            } else if (incomingOrder->shares() < 0) [[unlikely]] {
                delete incomingOrder;
                throw std::runtime_error("Negative share count result in trade attempt.");
            }
        }

        oppositePriceMap.erase(priceMapItOpt.value());
        priceMapItOpt = matchOrder(oppositeBookIt, oppositeSide, price);
    }

    return incomingOrder;
}

OrderQueue& MatchingEngine::locateOrderQueue(std::string const & orderId)
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
    OrderQueue& queue = locateOrderQueue(orderId);
    for (auto queueIt = queue.begin(); queueIt != queue.end(); ++queueIt) {
        if ((*queueIt)->id() == orderId) {
            int cancelShares = msg.shares();
            (*queueIt)->tradeShares(cancelShares);

            if ((*queueIt)->shares() <= 0) {
                idMap.erase((*queueIt)->id());
                delete *queueIt;
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

std::string MatchingEngine::getExecutionID()
{
    if (dataServicePtr != nullptr) {
        return dataServicePtr->getNextExecutionID();
    } else {
        return "";
    }
}

void MatchingEngine::sendExecuteMessage(std::string const & orderID, int shareDelta)
{
    if (dataServicePtr == nullptr) [[unlikely]] {
        return;
    }

    auto msg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::EXECUTE);
    msg.setParameter("Timestamp", pitchMsgFactory.getTimestampStr())
        .setParameter("OrderID", orderID)
        .setParameter("Shares", std::to_string(shareDelta))
        .setParameter("ExecutionID", getExecutionID());

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
    } else if (side == 'S') {
        return sellBook;
    } else [[unlikely]] {
        throw std::runtime_error("Unexpected side for getting book.");
    }
}

void MatchingEngine::setDataService(DataService* newDataServicePtr)
{
    this->dataServicePtr = newDataServicePtr;
}

std::unordered_map<std::string, Order*>& MatchingEngine::getIDMap()
{
    return this->idMap;
}

const std::optional<double> MatchingEngine::getLastPrice(std::string const & symbol) const
{
    auto lastPriceIt = lastPriceMap.find(symbol);
    if (lastPriceIt == lastPriceMap.end()) [[unlikely]] {
        return std::nullopt;
    }

    return lastPriceMap.at(symbol);
}
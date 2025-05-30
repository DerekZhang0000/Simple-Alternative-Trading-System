/**
 * @file matchingEngine.h
 * @author Derek Zhang
 * @brief Matching Engine header file
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef MATCHINGENGINE_H
#define MATCHINGENGINE_H

#include "order.h"
#include "pitchMessage.h"

#include <string>
#include <boost/container/flat_map.hpp>
#include <unordered_map>
#include <optional>
#include <deque>

typedef std::deque<Order*> OrderQueue;
typedef std::deque<Order*>::iterator OrderQueueIterator;
typedef boost::container::flat_map<double, std::deque<Order*>> PriceMap;
typedef boost::container::vec_iterator<std::pair<double, std::deque<Order*>>*, false> PriceMapIterator;
typedef std::unordered_map<std::string, PriceMap> Book;
typedef Book::iterator BookIterator;

class MatchingEngine {
    private:
    Book buyBook = {};
    Book sellBook = {};
    std::unordered_map<std::string, Order*> idMap = {};

    public:
    MatchingEngine() = default;
    ~MatchingEngine() = default;

    /**
     * @brief Ingests a message and performs a corresponding action
     * 
     * @param msg 
     */
    void ingestMessage(PitchMessage const & msg);

    /**
     * @brief Locates an order book given a ticker symbol and buy/sell side
     * 
     * @param symbol 
     * @param side 
     * @return BookIterator 
     */
    BookIterator locateBook(std::string const & symbol, char side);

    /**
     * @brief Adds an order to the order book, then executes trades if possible
     * 
     * @param msg 
     */
    void addOrder(PitchMessage const & msg);

    /**
     * @brief Returns an iterator pointing to an order given its ID
     * 
     * @param orderId 
     * @return OrderQueueIterator 
     */
    OrderQueue locateOrderQueue(std::string const & orderId);

    /**
     * @brief Cancels a certain number of shares in an order
     * 
     * @param msg 
     */
    void cancelOrder(PitchMessage const & msg);

    /**
     * @brief Forwards a trade message to data collection service
     * 
     * @param msg 
     */
    void forwardTrade(PitchMessage const & msg);

    /**
     * @brief Returns an iterator pointing to the best order match for a given symbol, side, and price, if one exists
     * 
     * @param oppositeBookIt 
     * @param side 
     * @param limitPrice 
     * @return std::optional<Order*> 
     */
    std::optional<PriceMapIterator> matchOrder(BookIterator const & oppositeBookIt, char const side, double const limitPrice);

    /**
     * @brief Attempts to make trades on a symbol for matching orders on the buy and sell sides, returning an updated Order if there are any remaining shares
     * 
     * @param symbol
     * @return PitchMessage 
     */
    inline std::optional<Order*> attemptTrade(Order* incomingOrder, std::string const & symbol);

    /**
     * @brief Initializes empty order queues on the buy and sell books for provided symbols, ran once on spin up
     * 
     * @param symbolList 
     */
    void populateSymbols(std::vector<std::string> const & symbolList);
};

#endif
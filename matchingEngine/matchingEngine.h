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
#include "pitchMsgFactory.h"
#include "dataService.h"

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
    PitchMsgFactory pitchMsgFactory = PitchMsgFactory();
    DataService* dataServicePtr;
    std::unordered_map<std::string, double> lastPriceMap;

    /**
     * @brief Locates a Book Iterator given a ticker symbol and buy/sell side
     * 
     * @param symbol 
     * @param side 
     * @return BookIterator 
     */
    BookIterator locateBook(std::string const & symbol, char side);

    /**
     * @brief Returns an Order Queue reference based on an Order ID 
     * 
     * @param orderId 
     * @return OrderQueue& 
     */
    OrderQueue& locateOrderQueue(std::string const & orderId);

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
     * @brief Adds an order to the order book, then executes trades if possible
     * 
     * @param msg 
     */
    void addOrder(PitchMessage const & msg);

    /**
     * @brief Cancels a certain number of shares in an order
     * 
     * @param msg 
     */
    void cancelOrder(PitchMessage const & msg);

    /**
     * @brief Sends an Order Executed message to the data service queue
     * 
     * @param orderID 
     * @param shareDelta 
     * @param dataServiceQueue 
     */
    void sendExecuteMessage(std::string const & orderID, int shareDelta);

    /**
     * @brief Forwards a trade message to data collection service
     * 
     * @param msg 
     */
    void forwardTrade(PitchMessage const & msg);

    public:
    /**
     * @brief Construct a new Matching Engine object and assign a data service pointer
     * 
     * @param dataServicePtr 
     */
    MatchingEngine(DataService* dataServicePtr);

    /**
     * @brief Destroy the Matching Engine object
     * 
     */
    ~MatchingEngine();

    /**
     * @brief Ingests a message and performs a corresponding action
     * 
     * @param msg 
     */
    void ingestMessage(PitchMessage const & msg);

    /**
     * @brief Gets an 8-character string of milliseconds since midnight
     * 
     * @return std::string 
     */
    std::string getTimestampStr();

    /**
     * @brief Gets the next ExecutionID
     * 
     * @return std::string 
     */
    std::string getExecutionID();

    /**
     * @brief Initializes empty order queues on the buy and sell books for provided symbols, ran once on spin up
     * 
     * @param symbolList 
     */
    void populateSymbols(std::vector<std::string> const & symbolList);

    /**
     * @brief Gets the Book object for a specified side
     * 
     * @param side 
     * @return Book& 
     */
    Book& getBook(char side);

    /**
     * @brief Sets the Data Service pointer
     * 
     * @param dataServicePtr 
     */
    void setDataService(DataService* newDataServicePtr);

    /**
     * @brief Returns the ID Map
     * 
     * @return std::unordered_map<std::string, Order*>& 
     */
    std::unordered_map<std::string, Order*>& getIDMap();

    /**
     * @brief Gets the last price of a ticker if it has been traded
     * 
     * @param symbol 
     * @return std::optional<double> 
     */
    const std::optional<double> getLastPrice(std::string const & symbol) const;
};

#endif
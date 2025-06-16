/**
 * @file engineRouter.h
 * @author Derek Zhang
 * @brief Engine Router header file 
 * @version 0.1
 * @date 2025-06-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ENGINEROUTER_H
#define ENGINEROUTER_H

#include "matchingEngine.h"

#include <unordered_map>
#include <vector>

class EngineRouter {
    private:
    int engineCount;
    std::vector<MatchingEngine> matchingEngines;
    std::unordered_map<std::string, MatchingEngine*> engineMap;

    /**
     * @brief Splits an input symbol vector into evenly distributed vectors based on engine count
     * 
     * @param symbolList 
     * @param engineCount 
     * @return std::vector<std::vector<std::string>> 
     */
    std::vector<std::vector<std::string>> splitSymbolList(std::vector<std::string>& symbolList, int engineCount);

    public:
    /**
     * @brief Constructs a new Engine Router object with a specified number of matching engines and divides the symbol responsibility evenly
     * 
     * @param symbolList 
     * @param engineCount 
     */
    EngineRouter(std::vector<std::string>& symbolList, int newEngineCount);

    /**
     * @brief Construct a new Engine Router object with a Data Service pointer set
     * 
     * @param symbolList 
     * @param newEngineCount 
     * @param dataServicePtr 
     */
    EngineRouter(std::vector<std::string>& symbolList, int newEngineCount, DataService* dataServicePtr);

    /**
     * @brief Function to connect Data Service pointer to all matching engines
     * 
     * @param dataServicePtr 
     */
    void connectDataService(DataService* dataServicePtr);

    /**
     * @brief Routes an incoming message to the appropriate Matching Engine
     * 
     * @tparam OrderType 
     * @param orderMsg 
     */
    template <typename OrderType>
    void routeMessage(OrderType orderMsg);

    /**
     * @brief Routes and gets the last price for a ticker from the appropriate Matching Engine
     * 
     * @param symbol 
     * @return const std::optional<double> 
     */
    const std::optional<double> routeGetLastPrice(std::string const & symbol) const;

    /**
     * @brief Destroys the Engine Router object
     * 
     */
    ~EngineRouter();
};

#endif
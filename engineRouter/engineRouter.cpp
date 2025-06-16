/**
 * @file engineRouter.cpp
 * @author Derek Zhang
 * @brief The Engine Router class will be responsible for routing order messages to the appropriate Matching Engine
 * @version 0.1
 * @date 2025-06-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "engineRouter.h"

std::vector<std::vector<std::string>> EngineRouter::splitSymbolList(std::vector<std::string>& symbolList, int engineCount)
{
    std::vector<std::vector<std::string>> splitSymbols(engineCount);

    int totalSymbols = symbolList.size();
    int baseSize = totalSymbols / engineCount;
    int remainder = totalSymbols % engineCount;

    int index = 0;
    for (int i = 0; i < engineCount; ++i) {
        int currentSize = baseSize + (i < remainder ? 1 : 0);
        for (int j = 0; j < currentSize; ++j) {
            splitSymbols[i].push_back(symbolList[index++]);
        }
    }

    return splitSymbols;
}

EngineRouter::EngineRouter(std::vector<std::string>& symbolList, int newEngineCount)
{
    this->engineCount = newEngineCount;
    auto symbolLists = splitSymbolList(symbolList, engineCount);
    for (auto & symbolList : symbolLists) {
        matchingEngines.emplace_back(nullptr);
        MatchingEngine* engine = &matchingEngines.back();
        engine->populateSymbols(symbolList);
        for (auto symbol : symbolList) {
            engineMap.emplace(symbol, engine);
        }
    }
}

EngineRouter::EngineRouter(std::vector<std::string>& symbolList, int newEngineCount, DataService* dataServicePtr)
{
    this->engineCount = newEngineCount;
    auto symbolLists = splitSymbolList(symbolList, engineCount);
    for (auto & symbolList : symbolLists) {
        matchingEngines.emplace_back(dataServicePtr);
        MatchingEngine* engine = &matchingEngines.back();
        engine->populateSymbols(symbolList);
        for (auto symbol : symbolList) {
            engineMap.emplace(symbol, engine);
        }
    }
}

void EngineRouter::connectDataService(DataService* dataServicePtr)
{
    for (auto & engine : matchingEngines) {
        engine.setDataService(dataServicePtr);
    }
}

template <typename OrderType>
void EngineRouter::routeMessage(OrderType orderMsg)
{
    engineMap[orderMsg.symbol()]->ingestMessage(orderMsg);
}

const std::optional<double> EngineRouter::routeGetLastPrice(std::string const & symbol) const
{
    return engineMap.at(symbol)->getLastPrice(symbol);
}

EngineRouter::~EngineRouter()
{
    this->matchingEngines.clear();
    this->engineMap.clear();
}
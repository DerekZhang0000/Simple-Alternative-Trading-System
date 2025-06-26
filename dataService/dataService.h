/**
 * @file dataService.h
 * @author Derek Zhang
 * @brief Data Service header file
 * @version 0.1
 * @date 2025-06-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef DATASERVICE_H
#define DATASERVICE_H

#include "pitchMessage.h"

#include <boost/lockfree/queue.hpp>

typedef boost::lockfree::queue<PitchMessage*, boost::lockfree::capacity<128>> DataServiceQueue;

class DataService {
    private:
    DataServiceQueue* serviceQueue = new DataServiceQueue();
    std::string executionID = "000000000000";   // Base 36 string that keeps track of unique execution IDs

    /**
     * @brief Increments a base 64 string of length 12 in place
     * 
     * @param base36Str 
     */
    void incrementBase36(std::string& base36Str);

    public:
    DataService() = default;
    ~DataService() = default;

    /**
     * @brief Gets the next execution ID
     * 
     * @return std::string 
     */
    std::string getNextExecutionID();

    /**
     * @brief Get the Data Service Queue pointer
     * 
     * @return DataServiceQueue* 
     */
    DataServiceQueue* getQueue();
};

#endif
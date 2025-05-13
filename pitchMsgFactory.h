/**
 * @file pitchMsgFactory.h
 * @author Derek Zhang
 * @brief PITCH Message Factory header file
 * @version 0.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PITCHMSGFACTORY_H
#define PITCHMSGFACTORY_H

#include "pitchMessage.h"
#include <string>
#include <vector>

class PitchMsgFactory {
    private:
    std::vector<std::string> addOrderFields =       {"Timestamp", "MessageType", "OrderID", "Side", "Shares", "Symbol", "Price", "Display"};
    std::vector<std::string> cancelOrderFields =    {"Timestamp", "MessageType", "OrderID", "Shares"};
    std::vector<std::string> execOrderFields =      {"Timestamp", "MessageType", "OrderID", "Shares", "ExecutionID"};
    std::vector<std::string> tradeMsgFields =       {"Timestamp", "MessageType", "OrderID", "Side", "Shares", "Symbol", "Price", "ExecutionID"};

    public:
    enum MSG_TYPE {ADD = 'A', CANCEL = 'X', EXECUTE = 'E', TRADE = 'P'};

    PitchMsgFactory() = default;
    ~PitchMsgFactory() = default;
    
    /**
     * @brief Create a Pitch Msg object
     * 
     * @param msgType
     * @return PitchMessage 
     */
    PitchMessage createPitchMsg(MSG_TYPE msgType);
};

#endif
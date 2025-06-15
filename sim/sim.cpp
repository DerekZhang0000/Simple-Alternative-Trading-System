/**
 * @file sim.cpp
 * @author Derek Zhang
 * @brief The Sim class is responsible for running the entire exchange simulation
 * @version 0.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>

#include "pitchMsgFactory.h"
#include "matchingEngine.h"

auto factory = PitchMsgFactory();
PitchMessage createPitchAddMessage(std::string side="B", std::string shares="000100", std::string price="0001000000",
    std::string orderID="1234567890AB", std::string symbol="   SPY")
{
    auto msg = factory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
    msg.setParameter("Timestamp", "12345678")
        .setParameter("OrderID", orderID)
        .setParameter("Side", side)
        .setParameter("Shares", shares)
        .setParameter("Symbol", symbol)
        .setParameter("Price", price)
        .setParameter("Display", "Y");
    
    return msg;
}

PitchMessage createPitchCancelMessage(std::string shares="000100", std::string orderID="1234567890AB")
{
    auto msg = factory.createPitchMsg(PitchMsgFactory::MSG_TYPE::CANCEL);
    msg.setParameter("Timestamp", "12345678")
        .setParameter("OrderID", orderID)
        .setParameter("Shares", shares);
    
    return msg;
}

int main()
{
    std::cout << "compiled\n";

    auto engine = MatchingEngine(nullptr);
    engine.populateSymbols({"   SPY"});

    engine.ingestMessage(createPitchAddMessage("B", "000050", "0001010000", "000000000101")); // $101
    engine.ingestMessage(createPitchAddMessage("B", "000050", "0001000000", "000000000102")); // $100
    engine.ingestMessage(createPitchAddMessage("B", "000050", "0000990000", "000000000103")); // $99
    auto testMsg = createPitchAddMessage("S", "000090", "0000980000", "000000000104");
    engine.ingestMessage(testMsg);

    std::string str = "";
    std::cin >> str;
    return 0;
}
/**
 * @file matchingEngineTest.cpp
 * @author Derek Zhang
 * @brief This test file covers Matching Engine unit tests
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#define BOOST_TEST_MODULE MatchingEngineTests

#include <boost/test/included/unit_test.hpp>

#include "matchingEngine.h"
#include "pitchMsgFactory.h"
#include "dataService.h"

BOOST_AUTO_TEST_SUITE(matchingEngineTests)

auto factory = PitchMsgFactory();
PitchMessage createTestPitchMessage(PitchMsgFactory::MSG_TYPE msgType, std::string side="B",
    std::string shares="000100", std::string price="0001000000", std::string orderID="1234567890AB", 
    std::string symbol="   SPY")
{
    auto msg = factory.createPitchMsg(msgType);
    msg.setParameter("Timestamp", "12345678")
        .setParameter("OrderID", orderID)
        .setParameter("Side", side)
        .setParameter("Shares", shares)
        .setParameter("Symbol", symbol)
        .setParameter("Price", price)
        .setParameter("Display", "Y");
    
    return msg;
}

// Verifies Matching Engine symbol population
BOOST_AUTO_TEST_CASE(spinUpEngine)
{
    auto engine = MatchingEngine(nullptr);
    auto msg = createTestPitchMessage(PitchMsgFactory::MSG_TYPE::ADD);
    BOOST_REQUIRE_THROW(engine.ingestMessage(msg), std::runtime_error);
    engine.populateSymbols({"   SPY"});
    BOOST_REQUIRE_NO_THROW(engine.ingestMessage(msg));
}

// Verifies that adding an Order places it in the Order Book
BOOST_AUTO_TEST_CASE(ingestAddOrder)
{
    auto engine = MatchingEngine(nullptr);
    BOOST_REQUIRE(engine.getBook('B').empty());
    engine.populateSymbols({"   SPY"});
    BOOST_REQUIRE(!engine.getBook('B').empty());

    auto msg = createTestPitchMessage(PitchMsgFactory::MSG_TYPE::ADD);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"].empty());
    engine.ingestMessage(msg);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].front()->id() == "1234567890AB");
}

// Verifies the price-time priority of trades in the Matching Engine
BOOST_AUTO_TEST_CASE(priceTimeTrade)
{
    auto engine = MatchingEngine(nullptr);
    engine.populateSymbols({"   SPY"});
    BOOST_REQUIRE(engine.getBook('B')["   SPY"].empty());
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());

    auto buy100at100 = createTestPitchMessage(PitchMsgFactory::MSG_TYPE::ADD, "B", "000100", "0001000000", "000000000000");
    engine.ingestMessage(buy100at100);
    auto firstBuyAt100 = engine.getBook('B')["   SPY"][100].front();
    BOOST_REQUIRE(firstBuyAt100->id() == "000000000000");
    BOOST_REQUIRE(firstBuyAt100->price() == 100);
    BOOST_REQUIRE(firstBuyAt100->shares() == 100);
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());

    auto sell10at90 = createTestPitchMessage(PitchMsgFactory::MSG_TYPE::ADD, "S", "000010", "0000900000", "000000000001");
    engine.ingestMessage(sell10at90);
    BOOST_REQUIRE(firstBuyAt100->shares() == 90);
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());
}

BOOST_AUTO_TEST_SUITE_END()
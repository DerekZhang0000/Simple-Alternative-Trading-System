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

BOOST_AUTO_TEST_CASE(spinUpEngine)
{
    auto engine = MatchingEngine(nullptr);
    auto factory = PitchMsgFactory();
    auto msg = factory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
    msg.setParameter("Timestamp", "12345678")
        .setParameter("OrderID", "1234567890AB")
        .setParameter("Side", "B")
        .setParameter("Shares", "000100")
        .setParameter("Symbol", "   SPY")
        .setParameter("Price", "0001000000")
        .setParameter("Display", "Y");

    BOOST_REQUIRE_THROW(engine.ingestMessage(msg), std::runtime_error);
    engine.populateSymbols({"   SPY"});
    BOOST_REQUIRE_NO_THROW(engine.ingestMessage(msg));
}

BOOST_AUTO_TEST_CASE(ingestAddOrder)
{
    auto engine = MatchingEngine(nullptr);
    BOOST_REQUIRE(engine.getBook('B').empty());
    engine.populateSymbols({"   SPY"});
    BOOST_REQUIRE(!engine.getBook('B').empty());

    auto factory = PitchMsgFactory();
    auto msg = factory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
    msg.setParameter("Timestamp", "12345678")
        .setParameter("OrderID", "1234567890AB")
        .setParameter("Side", "B")
        .setParameter("Shares", "000100")
        .setParameter("Symbol", "   SPY")
        .setParameter("Price", "0001000000")
        .setParameter("Display", "Y");

    BOOST_REQUIRE(engine.getBook('B')["   SPY"].empty());
    engine.ingestMessage(msg);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].front()->id() == "1234567890AB");
}

BOOST_AUTO_TEST_SUITE_END()
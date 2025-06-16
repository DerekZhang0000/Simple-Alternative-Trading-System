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

// Verifies Matching Engine symbol population
BOOST_AUTO_TEST_CASE(spinUpEngine)
{
    auto engine = MatchingEngine(nullptr);
    auto msg = createPitchAddMessage();
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

    auto msg = createPitchAddMessage();
    BOOST_REQUIRE(engine.getBook('B')["   SPY"].empty());
    engine.ingestMessage(msg);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].front()->id() == "1234567890AB");
    BOOST_REQUIRE(engine.getLastPrice("   SPY") == std::nullopt);
}

// Verifies the price-time priority of trades in the Matching Engine
BOOST_AUTO_TEST_CASE(ingestBuyAndSellOrders)
{
    auto engine = MatchingEngine(nullptr);
    engine.populateSymbols({"   SPY"});
    BOOST_REQUIRE(engine.getBook('B')["   SPY"].empty());
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());

    // Add buy order for 100 shares at $100
    auto buy100at100 = createPitchAddMessage("B", "000100", "0001000000", "000000000000");
    engine.ingestMessage(buy100at100);
    auto buy100at100order = engine.getBook('B')["   SPY"][100].front();
    BOOST_REQUIRE(buy100at100order->id() == "000000000000");
    BOOST_REQUIRE(buy100at100order->price() == 100);
    BOOST_REQUIRE(buy100at100order->shares() == 100);
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());

    // Add sell order for 10 shares at $90 - 10 shares traded at $100, 90 buys at $100 remaining
    auto sell10at90 = createPitchAddMessage("S", "000010", "0000900000", "000000000001");
    engine.ingestMessage(sell10at90);
    BOOST_REQUIRE(buy100at100order->shares() == 90);
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());
    BOOST_REQUIRE(engine.getLastPrice("   SPY") == 100);

    // Add sell order for 110 shares at $90 - 90 shares traded at $100, 20 sells at $90 remaining
    auto sell110at90 = createPitchAddMessage("S", "000110", "0000900000", "000000000002");
    engine.ingestMessage(sell110at90);
    auto sell110at90order = engine.getBook('S')["   SPY"][90].front();
    BOOST_REQUIRE(engine.getBook('B')["   SPY"].empty());
    BOOST_REQUIRE(sell110at90order->id() == "000000000002");
    BOOST_REQUIRE(sell110at90order->price() == 90);
    BOOST_REQUIRE(sell110at90order->shares() == 20);
    BOOST_REQUIRE(engine.getLastPrice("   SPY") == 100);

    // Add buy order for 20 shares at $110 - 20 shares trade at $90, 0 shares outstanding
    auto buy20at110 = createPitchAddMessage("B", "000020", "0001000000", "000000000003");
    engine.ingestMessage(buy20at110);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].empty());
    BOOST_REQUIRE(engine.getBook('S')["   SPY"][90].empty());
    BOOST_REQUIRE(engine.getLastPrice("   SPY") == 90);
}

// Verifies correct cancel Order functionality
BOOST_AUTO_TEST_CASE(ingestCancelOrders)
{
    auto engine = MatchingEngine(nullptr);
    engine.populateSymbols({"   SPY"});

    // Add buy order for 100 shares at $100
    auto buy100at100 = createPitchAddMessage("B", "000100", "0001000000", "000000000000");
    engine.ingestMessage(buy100at100);

    // Cancel 10 shares of Order - 90 shares left
    auto cancel10 = createPitchCancelMessage("000010", "000000000000");
    engine.ingestMessage(cancel10);
    auto buyOrder = engine.getBook('B')["   SPY"][100].front();
    BOOST_REQUIRE(buyOrder->shares() == 90);

    // Cancel 80 shares of Order - 10 shares left
    auto cancel80 = createPitchCancelMessage("000080", "000000000000");
    engine.ingestMessage(cancel80);
    BOOST_REQUIRE(buyOrder->shares() == 10);

    // Cancel 10 shares of Order - 0 shares left / Order deleted
    engine.ingestMessage(cancel10);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].empty());

    // Cancel 10 shares of Order - Throws error
    BOOST_REQUIRE_THROW(engine.ingestMessage(cancel10), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(AIGenTest_1)
{
    auto engine = MatchingEngine(nullptr);
    engine.populateSymbols({"   SPY"});

    // Step 1: Add two buy orders at same price
    auto buy100 = createPitchAddMessage("B", "000100", "0001000000", "000000000010");
    auto buy50 = createPitchAddMessage("B", "000050", "0001000000", "000000000011");
    engine.ingestMessage(buy100);
    engine.ingestMessage(buy50);

    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].size() == 2);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].front()->id() == "000000000010");

    // Step 2: Add a sell order to partially fill both buy orders
    auto sell120 = createPitchAddMessage("S", "000120", "0000900000", "000000000012");
    engine.ingestMessage(sell120);
    BOOST_REQUIRE(engine.getLastPrice("   SPY") == 100);

    // Buy 100 should be filled, Buy 50 should have 30 remaining
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].size() == 1);
    auto partialBuy = engine.getBook('B')["   SPY"][100].front();
    BOOST_REQUIRE(partialBuy->id() == "000000000011");
    BOOST_REQUIRE(partialBuy->shares() == 30);

    // Step 3: Cancel remaining 30 shares
    auto cancel30 = createPitchCancelMessage("000030", "000000000011");
    engine.ingestMessage(cancel30);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].empty());

    // Step 4: Try canceling again — should throw (already deleted)
    BOOST_REQUIRE_THROW(engine.ingestMessage(cancel30), std::runtime_error);

    // Step 5: Confirm idMap is clean
    auto const& idMap = engine.getIDMap();  // assumes this is exposed or friend-tested
    BOOST_REQUIRE(idMap.find("000000000010") == idMap.end());
    BOOST_REQUIRE(idMap.find("000000000011") == idMap.end());

    // Step 6: Confirm no sellers left either
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());
}

BOOST_AUTO_TEST_CASE(AIGenTest_2)
{
    MatchingEngine engine(nullptr);
    engine.populateSymbols({"   SPY"});

    // Step 1: Add three buy orders at descending prices
    engine.ingestMessage(createPitchAddMessage("B", "000050", "0001010000", "000000000101")); // $101
    engine.ingestMessage(createPitchAddMessage("B", "000050", "0001000000", "000000000102")); // $100
    engine.ingestMessage(createPitchAddMessage("B", "000050", "0000990000", "000000000103")); // $99

    BOOST_REQUIRE(engine.getBook('B')["   SPY"][101].size() == 1);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].size() == 1);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][99].size() == 1);

    // Step 2: Sell order sweeps top 2 levels ($101 and $100)
    engine.ingestMessage(createPitchAddMessage("S", "000090", "0000980000", "000000000104"));
    BOOST_REQUIRE(engine.getLastPrice("   SPY") == 100);

    // $101 should be gone, $100 should have 10 shares remaining
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][101].empty());
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].size() == 1);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].front()->shares() == 10);
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][99].size() == 1);
    BOOST_REQUIRE(engine.getBook('S')["   SPY"].empty());

    // Step 3: Cancel remaining 10 shares at $100
    engine.ingestMessage(createPitchCancelMessage("000010", "000000000102"));
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][100].empty());

    // Step 4: Cancel unmatched low bid at $99
    engine.ingestMessage(createPitchCancelMessage("000050", "000000000103"));
    BOOST_REQUIRE(engine.getBook('B')["   SPY"][99].empty());

    // Step 5: Try canceling already-removed $101 order
    BOOST_REQUIRE_THROW(engine.ingestMessage(createPitchCancelMessage("000050", "000000000101")), std::runtime_error);

    // Step 6: Try canceling nonexistent order
    BOOST_REQUIRE_THROW(engine.ingestMessage(createPitchCancelMessage("000001", "DEADBEEF0001")), std::runtime_error);

    // Step 7: Confirm buy side is fully empty
    auto& buyBook = engine.getBook('B')["   SPY"];
    for (auto& [price, queue] : buyBook) {
        BOOST_REQUIRE(queue.empty());
    }

    // Step 8: Confirm idMap is empty (requires access)
    const auto& idMap = engine.getIDMap();  // Assumes access or friend-test
    BOOST_REQUIRE(idMap.empty());
}

BOOST_AUTO_TEST_SUITE_END()
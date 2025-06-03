/**
 * @file pitchMessageTest.cpp
 * @author Derek Zhang
 * @brief This test file covers PITCH Message unit tests
 * @version 0.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#define BOOST_TEST_MODULE PITCHMessageTests

#include <boost/test/unit_test.hpp>

#include "pitchMsgFactory.h"

BOOST_AUTO_TEST_SUITE(pitchMessageTests)

BOOST_AUTO_TEST_CASE(createAddOrder)
{
    auto factory = PitchMsgFactory();
    auto msg = factory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
    BOOST_REQUIRE(msg.getParameter("MessageType") == "A");
    BOOST_REQUIRE(msg.type() == 'A');

    BOOST_REQUIRE_THROW(msg.string(), std::runtime_error);  // Creating message string with missing parameters throws an error

    BOOST_REQUIRE_THROW(msg.getParameter("Timestamp"), std::runtime_error);
    msg.setParameter("Timestamp", "12345678");
    BOOST_REQUIRE(msg.getParameter("Timestamp") == "12345678");
    BOOST_REQUIRE(msg.timestamp() == 12345678);

    BOOST_REQUIRE_THROW(msg.getParameter("OrderID"), std::runtime_error);
    msg.setParameter("OrderID", "1234567890AB");
    BOOST_REQUIRE(msg.getParameter("OrderID") == "1234567890AB");
    BOOST_REQUIRE(msg.id() == "1234567890AB");

    BOOST_REQUIRE_THROW(msg.getParameter("Side"), std::runtime_error);
    msg.setParameter("Side", "B");
    BOOST_REQUIRE(msg.getParameter("Side") == "B");
    BOOST_REQUIRE(msg.side() == 'B');

    BOOST_REQUIRE_THROW(msg.getParameter("Shares"), std::runtime_error);
    msg.setParameter("Shares", "000100");
    BOOST_REQUIRE(msg.getParameter("Shares") == "000100");
    BOOST_REQUIRE(msg.shares() == 100);

    BOOST_REQUIRE_THROW(msg.getParameter("Symbol"), std::runtime_error);
    msg.setParameter("Symbol", "   SPY");
    BOOST_REQUIRE(msg.getParameter("Symbol") == "   SPY");
    BOOST_REQUIRE(msg.symbol() == "   SPY");

    BOOST_REQUIRE_THROW(msg.getParameter("Price"), std::runtime_error);
    msg.setParameter("Price", "0001000000");    // $100, price is in 1/10,000 dollars for PITCH messages
    BOOST_REQUIRE(msg.getParameter("Price") == "0001000000");
    BOOST_REQUIRE(msg.price() == 100.00);

    BOOST_REQUIRE_THROW(msg.getParameter("Display"), std::runtime_error);
    msg.setParameter("Display", "Y");
    BOOST_REQUIRE(msg.getParameter("Display") == "Y");
    BOOST_REQUIRE(msg.display() == 'Y');

    BOOST_REQUIRE(msg.string() == "S12345678A1234567890ABB000100   SPY0001000000Y");
}

BOOST_AUTO_TEST_CASE(createCancelOrder)
{
    auto factory = PitchMsgFactory();
    auto msg = factory.createPitchMsg(PitchMsgFactory::MSG_TYPE::CANCEL);
    BOOST_REQUIRE(msg.getParameter("MessageType") == "X");
    BOOST_REQUIRE(msg.type() == 'X');

    BOOST_REQUIRE_THROW(msg.string(), std::runtime_error);

    BOOST_REQUIRE_THROW(msg.getParameter("Timestamp"), std::runtime_error);
    msg.setParameter("Timestamp", "12345678");
    BOOST_REQUIRE(msg.getParameter("Timestamp") == "12345678");
    BOOST_REQUIRE(msg.timestamp() == 12345678);

    BOOST_REQUIRE_THROW(msg.getParameter("OrderID"), std::runtime_error);
    msg.setParameter("OrderID", "1234567890AB");
    BOOST_REQUIRE(msg.getParameter("OrderID") == "1234567890AB");
    BOOST_REQUIRE(msg.id() == "1234567890AB");

    BOOST_REQUIRE_THROW(msg.getParameter("Shares"), std::runtime_error);
    msg.setParameter("Shares", "000100");
    BOOST_REQUIRE(msg.getParameter("Shares") == "000100");
    BOOST_REQUIRE(msg.shares() == 100);

    BOOST_REQUIRE(msg.string() == "S12345678X1234567890AB000100");
}

BOOST_AUTO_TEST_SUITE_END()
/**
 * @file traderBot.cpp
 * @author The Trader Bot class will simulate a trader
 * @brief 
 * @version 0.1
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "traderBot.h"

#include <algorithm>

/**
 * @brief TraderBot (parent class) functions
 * 
 */

void TraderBot::removeNullOrders()
{
    orders.remove_if([](Order* order) { return order == nullptr; });
}

TraderBot& TraderBot::setParameter(std::string key, double value)
{
    params[key] = value;
    return *this;
}

std::string TraderBot::getParameter(std::string param)
{
    return params[param];
}

void TraderBot::pushOrder(Order* order)
{
    orders.push_back(order);
}

std::vector<PitchMessage*> GaussianBot::createPitchMsgs(std::vector<std::string> orderIDs)
{
    std::vector<PitchMessage*> pitchMessages;

    if (gauss_dist == nullptr) [[unlikely]] {
        std::normal_distribution<double> gaussian(std::stod(getParameter("Mean")), std::stod(getParameter("Deviation")));
        gauss_dist = [gaussian, this]() mutable { return gaussian(gen); };
    }

    for (std::string orderID : orderIDs) {
        auto pitchMsg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
        pitchMsg.setParameter("Timestamp", pitchMsgFactory.getTimestampStr())
                .setParameter("OrderID", orderID)
                .setParameter("Side", getParameter("Side"))
                .setParameter("Shares", getParameter("Shares"))
                .setParameter("Symbol", getParameter("Symbol"))
                .setParameter("Price", pitchMsgFactory.getPriceStr(std::max(gauss_dist(), 0.0)))    // price can not be negative
                .setParameter("Display", getParameter("Display"));
        pitchMessages.push_back(&pitchMsg);
    }

    removeNullOrders();
    for (Order* order : orders) {
        if (uniform(gen) < 0.05) {  // arbitrary 5% chance of cancelling a trade
        auto pitchMsg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::CANCEL);
            pitchMsg.setParameter("Timestamp", pitchMsgFactory.getTimestampStr())
                    .setParameter("OrderID", order->id())
                    .setParameter("Shares", getParameter("Shares"));
            pitchMessages.push_back(&pitchMsg);
        }
    }

    return pitchMessages;
}

std::vector<PitchMessage*> MarketMakerBot::createPitchMsgs(std::vector<std::string> orderIDs)
{
    std::vector<PitchMessage*> pitchMessages;

    auto lower = orders.front();
    if (lower->shares() < 100) {
        auto pitchMsg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
        pitchMsg.setParameter("Timestamp", pitchMsgFactory.getTimestampStr())
                .setParameter("OrderID", orderIDs.at(0))
                .setParameter("Side", getParameter("Side"))
                .setParameter("Shares", pitchMsgFactory.getSharesStr(std::stoi(getParameter("Shares")) - lower->shares()))
                .setParameter("Symbol", getParameter("Symbol"))
                .setParameter("Price", pitchMsgFactory.getPriceStr(std::max(std::stod(getParameter("Center")) - std::stod(getParameter("Spread")), 0.0)))    // price can not be negative
                .setParameter("Display", getParameter("Display"));
        pitchMessages.push_back(&pitchMsg);
    }

    auto higher = orders.back();
    if (higher->shares() < 100) {
        auto pitchMsg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::ADD);
        pitchMsg.setParameter("Timestamp", pitchMsgFactory.getTimestampStr())
                .setParameter("OrderID", orderIDs.at(0))
                .setParameter("Side", getParameter("Side"))
                .setParameter("Shares", pitchMsgFactory.getSharesStr(std::stoi(getParameter("Shares")) - higher->shares()))
                .setParameter("Symbol", getParameter("Symbol"))
                .setParameter("Price", pitchMsgFactory.getPriceStr(std::stod(getParameter("Center")) + std::stod(getParameter("Spread"))))
                .setParameter("Display", getParameter("Display"));
        pitchMessages.push_back(&pitchMsg);
    }

    return pitchMessages;
}
std::vector<PitchMessage*> TradeMessenger::createPitchMsgs(std::vector<std::string> orderIDs)
{
    std::vector<PitchMessage*> pitchMessages;

    if (gauss_dist == nullptr) [[unlikely]] {
        std::normal_distribution<double> gaussian(std::stod(getParameter("Mean")), std::stod(getParameter("Deviation")));
        gauss_dist = [gaussian, this]() mutable { return gaussian(gen); };
    }

    for (std::string orderID : orderIDs) {
        auto pitchMsg = pitchMsgFactory.createPitchMsg(PitchMsgFactory::MSG_TYPE::TRADE);
        pitchMsg.setParameter("Timestamp", pitchMsgFactory.getTimestampStr())
                .setParameter("OrderID", orderID)
                .setParameter("Side", getParameter("Side"))
                .setParameter("Shares", getParameter("Shares"))
                .setParameter("Symbol", getParameter("Symbol"))
                .setParameter("Price", pitchMsgFactory.getPriceStr(std::max(gauss_dist(), 0.0)))    // price can not be negative
                .setParameter("ExecutionID", orderID + orderID + "00");
        pitchMessages.push_back(&pitchMsg);
    }

    return pitchMessages;
}
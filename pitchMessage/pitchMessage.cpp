/**
 * @file pitchMessage.cpp
 * @author Derek Zhang
 * @brief PITCH Message class
 * @version 0.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "pitchMessage.h"
#include <stdexcept>

PitchMessage::PitchMessage(std::string newMsgType, std::vector<std::string> fieldList)
{
    msgFields = fieldList;
    setParameter("MessageType", newMsgType);
}

PitchMessage& PitchMessage::setParameter(std::string param, std::string value)
{
    params[param] = value;
    return *this;
}

std::string PitchMessage::getParameter(std::string param) const
{
    auto paramsIt = params.find(param);
    if (paramsIt == params.end()) [[unlikely]] {
        throw std::runtime_error("Parameter \"" + param + "\" not found.");
    }

    return paramsIt->second;
}

std::string PitchMessage::string() const
{
    std::string pitchString = "S";
    for (std::string fieldName : msgFields) {
        auto paramsPtr = params.find(fieldName);
        if (paramsPtr == params.end()) [[unlikely]] {
            throw std::runtime_error("Message missing required parameter " + fieldName + ".");
        }
        pitchString += paramsPtr->second;
    }

    return pitchString;
}

char PitchMessage::display() const
{
    return getParameter("Display").at(0);
}

std::string PitchMessage::execId() const
{
    return getParameter("ExecutionID");
}

char PitchMessage::side() const
{
    return getParameter("Side").at(0);
}

char PitchMessage::type() const
{
    return getParameter("MessageType").at(0);
}

double PitchMessage::price() const
{
    return std::stod(getParameter("Price")) / 10000;
}

int PitchMessage::shares() const
{
    return std::stoi(getParameter("Shares"));
}

int PitchMessage::timestamp() const
{
    return std::stoi(getParameter("Timestamp"));
}

std::string PitchMessage::id() const
{
    return getParameter("OrderID");
}

std::string PitchMessage::symbol() const
{
    return getParameter("Symbol");
}
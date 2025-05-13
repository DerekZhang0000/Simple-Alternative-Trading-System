/**
 * @file pitchMessage.cpp
 * @author Derek Zhang
 * @brief PITCH message class
 * @version 0.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "pitchMessage.h"

PitchMessage::PitchMessage(char msgType, std::vector<std::string> fieldList)
{
    msgFields = fieldList;
    std::string msgTypeStr{msgType};
    setParameter("Message Type", msgTypeStr);
}

PitchMessage PitchMessage::setParameter(std::string param, std::string value)
{
    params[param] = value;
    return *this;
}

bool PitchMessage::isValid()
{
    for (std::string fieldName : msgFields) {
        if (params.find(fieldName) == params.end()) {
            return false;
        }
    }

    return true;
}

std::string PitchMessage::toString()
{
    if (!isValid()) {
        return "";
    }

    std::string pitchString = "S";
    for (std::string fieldName : msgFields) {
        pitchString += params[fieldName];
    }

    return pitchString;
}
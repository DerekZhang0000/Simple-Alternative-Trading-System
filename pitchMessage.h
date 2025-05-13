/**
 * @file pitchMessage.h
 * @author Derek Zhang
 * @brief PITCH message header file
 * @version 0.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PITCHMESSAGE_H
#define PITCHMESSAGE_H

#include <string>
#include <vector>
#include <unordered_map>

class PitchMessage
{
    private:
    std::unordered_map<std::string, std::string> params;
    std::vector<std::string> msgFields;

    public:
    ~PitchMessage() = default;

    /**
     * @brief Construct a new Pitch Message object and sets the appropriate required field list
     * 
     * @param msgType 
     */
    PitchMessage(char msgType, std::vector<std::string> msgFields);

    /**
     * @brief Set the Parameter object, 
     * 
     * @param param 
     * @param value 
     * @return PitchMessage 
     */
    PitchMessage setParameter(std::string param, std::string value);

    /**
     * @brief Returns whether or not the PITCH message has all necessary information
     * 
     * @return true 
     * @return false 
     */
    bool isValid();

    /**
     * @brief Returns the PITCH message as a string
     * 
     * @return std::string 
     */
    std::string toString();
};

#endif
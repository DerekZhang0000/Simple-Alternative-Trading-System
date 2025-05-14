/**
 * @file pitchMessage.h
 * @author Derek Zhang
 * @brief PITCH Message header file
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
    char msgType;
    std::unordered_map<std::string, std::string> params;
    std::vector<std::string> msgFields;

    public:
    ~PitchMessage() = default;

    /**
     * @brief Construct a new PITCH Message object and sets the appropriate required field list
     * 
     * @param msgType 
     */
    PitchMessage(char msgType, std::vector<std::string> msgFields);

    /**
     * @brief Sets a parameter and returns self
     * 
     * @param param 
     * @param value 
     * @return PitchMessage 
     */
    PitchMessage setParameter(std::string param, std::string value);

    /**
     * @brief Gets a specified parameter
     * 
     * @param param 
     * @return std::string 
     */
    std::string getParameter(std::string param) const;

    /**
     * @brief Returns the PITCH message as a string
     * 
     * @return std::string 
     */
    std::string string() const;

    /**
     * @brief Returns the PITCH message type
     * 
     * @return char 
     */
    char type() const;
};

#endif
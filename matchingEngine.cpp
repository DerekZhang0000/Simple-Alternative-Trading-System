/**
 * @file matchingEngine.cpp
 * @author Derek Zhang
 * @brief The Matching Engine class is responsible for matching orders 
 * @version 0.1
 * @date 2025-05-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "matchingEngine.h"

#include <stdexcept>

void MatchingEngine::ingestMessage(PitchMessage msg)
{
    char msgType = msg.type();
    switch (msgType)
    {
    case 'A':
        // stuff
        break;

    case 'X':
        // stuff
        break;

    case 'E':
        // stuff
        break;

    case 'P':
        // stuff
        break;

    default:
        break;
    }
}
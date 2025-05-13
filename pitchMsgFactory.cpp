/**
 * @file pitchMsgFactory.cpp
 * @author Derek Zhang
 * @brief PITCH Message Factory class
 * @version 0.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "pitchMsgFactory.h"

PitchMessage PitchMsgFactory::createPitchMsg(MSG_TYPE msgType)
{
    switch (msgType){
        case MSG_TYPE::ADD:
            return PitchMessage('A', addOrderFields);
            break;
        case MSG_TYPE::CANCEL:
            return PitchMessage('X', cancelOrderFields);
            break;
        case MSG_TYPE::EXECUTE:
            return PitchMessage('E', execOrderFields);
            break;
        case MSG_TYPE::TRADE:
            return PitchMessage('P', tradeMsgFields);
            break;
    }
}
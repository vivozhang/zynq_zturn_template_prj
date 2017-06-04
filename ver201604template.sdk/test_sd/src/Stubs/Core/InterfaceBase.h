#ifndef __INTERFACEBASE_H__
#define __INTERFACEBASE_H__


#include <stdint.h>     // temporary until start using newest NaNRoutines from Core


struct cCommandMsgHeader
{
public:
    // These first 2 variables are common to all message types
    uint16_t  mType;                     // Specific Data Message Type (EGD, App ...)
    uint16_t  mHeaderLength;             // Length of the header only!
    // This part of the header is user defined for a particular message type
    uint16_t  mCmdMsgType;               // Defines the type of cmd message as defined by the interface

//    void FixEndianAndSwap(cCommandMsgHeader * Dest){
//        Dest->mType         = ENDIAN_LE16(mType);
//        Dest->mHeaderLength = ENDIAN_LE16(mHeaderLength);
//        Dest->mCmdMsgType   = ENDIAN_LE16(mCmdMsgType);
//    }
};



#endif  // __INTERFACEBASE_H__


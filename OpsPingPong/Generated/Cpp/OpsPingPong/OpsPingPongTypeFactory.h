//Auto generated OPS-code. DO NOT MODIFY!
#pragma once

#include "SerializableFactory.h"

#include "PingPong/PingData.h"
#include "PingPong/PongData.h"


namespace OpsPingPong {

class OpsPingPongTypeFactory : public ops::SerializableFactory
{
public:
    ops::Serializable* create(const ops::TypeId_T& type)
    {
        if(type == "PingPong.PingData")
        {
            return new PingPong::PingData();
        }
        if(type == "PingPong.PongData")
        {
            return new PingPong::PongData();
        }
        return nullptr;

    }
};

}


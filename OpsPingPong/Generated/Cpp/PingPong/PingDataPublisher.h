//Auto generated OPS-code. DO NOT MODIFY!
#pragma once

#include "Publisher.h"
#include "Topic.h"
#include "OPSObject.h"
#include "PingData.h"

namespace PingPong {

class PingDataPublisher : public ops::Publisher
{

public:
    explicit PingDataPublisher(ops::Topic t)
        : ops::Publisher(t)
    {
    }

    ~PingDataPublisher(void)
    {
    }

    bool write(PingData* data)
    {
        return ops::Publisher::write(data);
    }

    bool write(PingData& data)
    {
        return ops::Publisher::write(&data);
    }

};

}


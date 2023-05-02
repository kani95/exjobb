//Auto generated OPS-code. DO NOT MODIFY!
#pragma once

#include "Publisher.h"
#include "Topic.h"
#include "OPSObject.h"
#include "PongData.h"

namespace PingPong {

class PongDataPublisher : public ops::Publisher
{

public:
    explicit PongDataPublisher(ops::Topic t)
        : ops::Publisher(t)
    {
    }

    ~PongDataPublisher(void)
    {
    }

    bool write(PongData* data)
    {
        return ops::Publisher::write(data);
    }

    bool write(PongData& data)
    {
        return ops::Publisher::write(&data);
    }

};

}


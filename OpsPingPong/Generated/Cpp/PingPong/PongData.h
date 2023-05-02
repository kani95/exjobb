//Auto generated OPS-code. DO NOT MODIFY!
#pragma once

#include "OPSObject.h"
#include "ArchiverInOut.h"
#include <string.h>		// for memset() on Linux
#include <vector>



namespace PingPong {


class PongData :
	public ops::OPSObject
{
public:
  	static ops::TypeId_T getTypeName(){return ops::TypeId_T("PingPong.PongData");}

  	char PongData_version = PongData_idlVersion;

    static const char PongData_idlVersion = 0;

    int64_t filler[3];
    int64_t time;

    ///Default constructor.
    PongData()
        : ops::OPSObject()
        , time(0)
    {
        OPSObject::appendType(ops::TypeId_T("PingPong.PongData"));
        memset(&filler[0], 0, sizeof(filler));

    }

    ///Copy-constructor making full deep copy of a(n) PongData object.
    PongData(const PongData& __c)
       : ops::OPSObject()
        , time(0)
    {
        OPSObject::appendType(ops::TypeId_T("PingPong.PongData"));
        memset(&filler[0], 0, sizeof(filler));

        __c.fillClone(this);
    }

    ///Assignment operator making full deep copy of a(n) PongData object.
    PongData& operator = (const PongData& other)
    {
        if (this != &other) {
            other.fillClone(this);
        }
        return *this;
    }

    ///Move-constructor taking other's resources
    PongData(PongData&& other) : ops::OPSObject(std::move(other))
    {
        PongData_version = std::move(other.PongData_version);
        memcpy(&filler[0], &other.filler[0], sizeof(filler));
        time = std::move(other.time);

    }

    // Move assignment operator taking other's resources
    PongData& operator= (PongData&& other)
    {
        if (this != &other) {
            ops::OPSObject::operator=(std::move(other));
            PongData_version = other.PongData_version;
            memcpy(&filler[0], &other.filler[0], sizeof(filler));
            time = other.time;

        }
        return *this;
    }

    ///This method acceptes an ops::ArchiverInOut visitor which will serialize or deserialize an
    ///instance of this class to a format dictated by the implementation of the ArchiverInout.
    virtual void serialize(ops::ArchiverInOut* archive) override
    {
        ops::OPSObject::serialize(archive);
        if (idlVersionMask != 0) {
            archive->inout("PongData_version", PongData_version);
            ValidateVersion("PongData", PongData_version, PongData_idlVersion);
        } else {
            PongData_version = 0;
        }
        archive->inoutfixarr("filler", &filler[0], 3, sizeof(filler));
        archive->inout("time", time);

    }

    //Returns a deep copy of this object.
    virtual PongData* clone() override
    {
        PongData* ret = new PongData;
        fillClone(ret);
        return ret;

    }

    void fillClone(PongData* obj) const
    {
        if (this == obj) { return; }
        ops::OPSObject::fillClone(obj);
        obj->PongData_version = PongData_version;
        memcpy(&obj->filler[0], &filler[0], sizeof(filler));
        obj->time = time;

    }

    ///Destructor: Note that all aggregated data and vectors are completely deleted.
    virtual ~PongData(void)
    {

    }


};

}


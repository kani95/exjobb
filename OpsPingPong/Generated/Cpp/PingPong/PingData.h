//Auto generated OPS-code. DO NOT MODIFY!
#pragma once

#include "OPSObject.h"
#include "ArchiverInOut.h"
#include <string.h>		// for memset() on Linux
#include <vector>



namespace PingPong {


class PingData :
	public ops::OPSObject
{
public:
  	static ops::TypeId_T getTypeName(){return ops::TypeId_T("PingPong.PingData");}

  	char PingData_version = PingData_idlVersion;

    static const char PingData_idlVersion = 0;

    int64_t filler[3];
    int64_t time;

    ///Default constructor.
    PingData()
        : ops::OPSObject()
        , time(0)
    {
        OPSObject::appendType(ops::TypeId_T("PingPong.PingData"));
        memset(&filler[0], 0, sizeof(filler));

    }

    ///Copy-constructor making full deep copy of a(n) PingData object.
    PingData(const PingData& __c)
       : ops::OPSObject()
        , time(0)
    {
        OPSObject::appendType(ops::TypeId_T("PingPong.PingData"));
        memset(&filler[0], 0, sizeof(filler));

        __c.fillClone(this);
    }

    ///Assignment operator making full deep copy of a(n) PingData object.
    PingData& operator = (const PingData& other)
    {
        if (this != &other) {
            other.fillClone(this);
        }
        return *this;
    }

    ///Move-constructor taking other's resources
    PingData(PingData&& other) : ops::OPSObject(std::move(other))
    {
        PingData_version = std::move(other.PingData_version);
        memcpy(&filler[0], &other.filler[0], sizeof(filler));
        time = std::move(other.time);

    }

    // Move assignment operator taking other's resources
    PingData& operator= (PingData&& other)
    {
        if (this != &other) {
            ops::OPSObject::operator=(std::move(other));
            PingData_version = other.PingData_version;
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
            archive->inout("PingData_version", PingData_version);
            ValidateVersion("PingData", PingData_version, PingData_idlVersion);
        } else {
            PingData_version = 0;
        }
        archive->inoutfixarr("filler", &filler[0], 3, sizeof(filler));
        archive->inout("time", time);

    }

    //Returns a deep copy of this object.
    virtual PingData* clone() override
    {
        PingData* ret = new PingData;
        fillClone(ret);
        return ret;

    }

    void fillClone(PingData* obj) const
    {
        if (this == obj) { return; }
        ops::OPSObject::fillClone(obj);
        obj->PingData_version = PingData_version;
        memcpy(&obj->filler[0], &filler[0], sizeof(filler));
        obj->time = time;

    }

    ///Destructor: Note that all aggregated data and vectors are completely deleted.
    virtual ~PingData(void)
    {

    }


};

}


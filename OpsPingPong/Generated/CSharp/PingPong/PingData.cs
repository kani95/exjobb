//Auto generated OPS-code. DO NOT MODIFY!

using Ops;
using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace PingPong
{
    // Message


    [Serializable]
    public class PingData : OPSObject
    {
        private byte _PingData_version = PingData_idlVersion;
        public byte PingData_version
        {
            get { return _PingData_version; }
            set {
                if (value > PingData_idlVersion)
                {
                    throw new IdlVersionException(
                        "PingData: received version '" + value + "' > known version '" + PingData_idlVersion + "'");
                }
                _PingData_version = value;
            }
        }

        public const byte PingData_idlVersion = 0;
        private List<long> _filler = new List<long>();
        public List<long> filler { get { return  _filler; } set { _filler = value; } } 

        public long time { get; set; }



        public static new string GetTypeName() { return "PingPong.PingData"; }

        public PingData() : base()
        {
            AppendType(GetTypeName());
            for (int i = 0; i < 3; i++) _filler.Add(0);

        }

        public override void Serialize(IArchiverInOut archive)
        {
            base.Serialize(archive);
            if (IdlVersionMask != 0) {
                PingData_version = archive.Inout("PingData_version", PingData_version);
            } else {
                PingData_version = 0;
            }
            _filler = (List<long>) archive.InoutLongList("filler", _filler);
            time = archive.Inout("time", time);

        }

        public override object Clone()
        {
            PingData cloneResult = new PingData();
            FillClone(cloneResult);
            return cloneResult;
        }

        public override void FillClone(OPSObject cloneO)
        {
            base.FillClone(cloneO);
            PingData cloneResult = (PingData)cloneO;
            cloneResult.PingData_version = this.PingData_version;
            cloneResult.filler = new List<long>(this.filler);
            cloneResult.time = this.time;

        }
    }

    // Publisher

    public class PingDataPublisher : Publisher
    {
        public PingDataPublisher(Topic t) : base(t)
        {
            CheckTypeString(PingData.GetTypeName());
        }

        public void Write(PingData o)
        {
            base.Write(o);
        }
    }

    // Subscriber

    public delegate void PingDataEventHandler(PingDataSubscriber sender, PingData data);

    public class PingDataSubscriber : Subscriber
    {
        public event PingDataEventHandler newData;

        public PingDataSubscriber(Topic t) : base(t)
        {
            CheckTypeString(PingData.GetTypeName());
        }

        protected override void NewDataArrived(OPSObject o)
        {
            if (newData != null) newData(this, (PingData)o);
        }

        public PingData WaitForNextData(long millis)
        {
            return (PingData)WaitForNextOpsObjectData(millis);
        }
    }

}

//Auto generated OPS-code. DO NOT MODIFY!

using Ops;
using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace PingPong
{
    // Message


    [Serializable]
    public class PongData : OPSObject
    {
        private byte _PongData_version = PongData_idlVersion;
        public byte PongData_version
        {
            get { return _PongData_version; }
            set {
                if (value > PongData_idlVersion)
                {
                    throw new IdlVersionException(
                        "PongData: received version '" + value + "' > known version '" + PongData_idlVersion + "'");
                }
                _PongData_version = value;
            }
        }

        public const byte PongData_idlVersion = 0;
        private List<long> _filler = new List<long>();
        public List<long> filler { get { return  _filler; } set { _filler = value; } } 

        public long time { get; set; }



        public static new string GetTypeName() { return "PingPong.PongData"; }

        public PongData() : base()
        {
            AppendType(GetTypeName());
            for (int i = 0; i < 3; i++) _filler.Add(0);

        }

        public override void Serialize(IArchiverInOut archive)
        {
            base.Serialize(archive);
            if (IdlVersionMask != 0) {
                PongData_version = archive.Inout("PongData_version", PongData_version);
            } else {
                PongData_version = 0;
            }
            _filler = (List<long>) archive.InoutLongList("filler", _filler);
            time = archive.Inout("time", time);

        }

        public override object Clone()
        {
            PongData cloneResult = new PongData();
            FillClone(cloneResult);
            return cloneResult;
        }

        public override void FillClone(OPSObject cloneO)
        {
            base.FillClone(cloneO);
            PongData cloneResult = (PongData)cloneO;
            cloneResult.PongData_version = this.PongData_version;
            cloneResult.filler = new List<long>(this.filler);
            cloneResult.time = this.time;

        }
    }

    // Publisher

    public class PongDataPublisher : Publisher
    {
        public PongDataPublisher(Topic t) : base(t)
        {
            CheckTypeString(PongData.GetTypeName());
        }

        public void Write(PongData o)
        {
            base.Write(o);
        }
    }

    // Subscriber

    public delegate void PongDataEventHandler(PongDataSubscriber sender, PongData data);

    public class PongDataSubscriber : Subscriber
    {
        public event PongDataEventHandler newData;

        public PongDataSubscriber(Topic t) : base(t)
        {
            CheckTypeString(PongData.GetTypeName());
        }

        protected override void NewDataArrived(OPSObject o)
        {
            if (newData != null) newData(this, (PongData)o);
        }

        public PongData WaitForNextData(long millis)
        {
            return (PongData)WaitForNextOpsObjectData(millis);
        }
    }

}

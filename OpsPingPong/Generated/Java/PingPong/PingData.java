//Auto generated OPS-code. DO NOT MODIFY!

package PingPong;

import ops.OPSObject;
import configlib.ArchiverInOut;
import configlib.SerializableFactory;
import configlib.Serializable;
import java.io.IOException;

public class PingData extends OPSObject
{
    public byte PingData_version = PingData_idlVersion;

    public static final byte PingData_idlVersion = 0;
    public java.util.Vector<Long> filler = new java.util.Vector<Long>();
    public long time;


    private static SerializableFactory factory = new TypeFactory();

    public static String getTypeName(){return "PingPong.PingData";}

    public static SerializableFactory getTypeFactory()
    {
        return factory;
    }

    public PingData()
    {
        super();
        appendType(getTypeName());
        for (int i = 0; i < 3; i++) filler.add((long)0);

    }
    public void serialize(ArchiverInOut archive) throws IOException
    {
        super.serialize(archive);
        if (idlVersionMask != 0) {
            byte tmp = archive.inout("PingData_version", PingData_version);
            if (tmp > PingData_idlVersion) {
                throw new IOException("PingData: received version '" + tmp + "' > known version '" + PingData_idlVersion + "'");
            }
            PingData_version = tmp;
        } else {
            PingData_version = 0;
        }
        filler = (java.util.Vector<Long>) archive.inoutLongList("filler", filler);
        time = archive.inout("time", time);

    }
    @Override
    public Object clone()
    {
        PingData cloneResult = new PingData();
        fillClone(cloneResult);
        return cloneResult;
    }

    @Override
    public void fillClone(OPSObject cloneO)
    {
        super.fillClone(cloneO);
        PingData cloneResult = (PingData)cloneO;
        cloneResult.PingData_version = this.PingData_version;
        cloneResult.filler = (java.util.Vector)this.filler.clone();
        cloneResult.time = this.time;

    }

    private static class TypeFactory implements SerializableFactory
    {
        public Serializable create(String type)
        {
            if (type.equals(PingData.getTypeName()))
            {
                return new PingData();
            }
            else
            {
                return null;
            }
        }
    }
}

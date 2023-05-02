//Auto generated OPS-code. DO NOT MODIFY!

package PingPong;

import ops.OPSObject;
import configlib.ArchiverInOut;
import configlib.SerializableFactory;
import configlib.Serializable;
import java.io.IOException;

public class PongData extends OPSObject
{
    public byte PongData_version = PongData_idlVersion;

    public static final byte PongData_idlVersion = 0;
    public java.util.Vector<Long> filler = new java.util.Vector<Long>();
    public long time;


    private static SerializableFactory factory = new TypeFactory();

    public static String getTypeName(){return "PingPong.PongData";}

    public static SerializableFactory getTypeFactory()
    {
        return factory;
    }

    public PongData()
    {
        super();
        appendType(getTypeName());
        for (int i = 0; i < 3; i++) filler.add((long)0);

    }
    public void serialize(ArchiverInOut archive) throws IOException
    {
        super.serialize(archive);
        if (idlVersionMask != 0) {
            byte tmp = archive.inout("PongData_version", PongData_version);
            if (tmp > PongData_idlVersion) {
                throw new IOException("PongData: received version '" + tmp + "' > known version '" + PongData_idlVersion + "'");
            }
            PongData_version = tmp;
        } else {
            PongData_version = 0;
        }
        filler = (java.util.Vector<Long>) archive.inoutLongList("filler", filler);
        time = archive.inout("time", time);

    }
    @Override
    public Object clone()
    {
        PongData cloneResult = new PongData();
        fillClone(cloneResult);
        return cloneResult;
    }

    @Override
    public void fillClone(OPSObject cloneO)
    {
        super.fillClone(cloneO);
        PongData cloneResult = (PongData)cloneO;
        cloneResult.PongData_version = this.PongData_version;
        cloneResult.filler = (java.util.Vector)this.filler.clone();
        cloneResult.time = this.time;

    }

    private static class TypeFactory implements SerializableFactory
    {
        public Serializable create(String type)
        {
            if (type.equals(PongData.getTypeName()))
            {
                return new PongData();
            }
            else
            {
                return null;
            }
        }
    }
}

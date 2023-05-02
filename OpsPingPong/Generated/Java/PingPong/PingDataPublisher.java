//Auto generated OPS-code. !DO NOT MODIFY!

package PingPong;

import ops.Publisher;
import ops.OPSObject;
import ops.Topic;

public class PingDataPublisher extends Publisher 
{
    public PingDataPublisher(Topic<PingData> t) throws ops.ConfigurationException
    {
        super(t);
    }
    public void write(PingData o) 
    {
        super.write(o);
    }    
}
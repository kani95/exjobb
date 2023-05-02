//Auto generated OPS-code. !DO NOT MODIFY!

package PingPong;

import ops.Publisher;
import ops.OPSObject;
import ops.Topic;

public class PongDataPublisher extends Publisher 
{
    public PongDataPublisher(Topic<PongData> t) throws ops.ConfigurationException
    {
        super(t);
    }
    public void write(PongData o) 
    {
        super.write(o);
    }    
}
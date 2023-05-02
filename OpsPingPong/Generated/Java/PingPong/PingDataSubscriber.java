//Auto generated OPS-code. !DO NOT MODIFY!

package PingPong;

import java.io.IOException;
import ops.Subscriber;
import ops.OPSObject;
import ops.Topic;
import ops.OPSTopicTypeMissmatchException;

public class PingDataSubscriber extends Subscriber 
{
    public PingDataSubscriber(Topic<PingData> t) throws ops.ConfigurationException
    {
        super(t);

        participant.addTypeSupport(PingData.getTypeFactory());
        
    }

    public PingData getData()
    {
        return (PingData)super.getData();
    }
}
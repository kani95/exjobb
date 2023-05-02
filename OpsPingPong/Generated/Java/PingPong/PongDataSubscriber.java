//Auto generated OPS-code. !DO NOT MODIFY!

package PingPong;

import java.io.IOException;
import ops.Subscriber;
import ops.OPSObject;
import ops.Topic;
import ops.OPSTopicTypeMissmatchException;

public class PongDataSubscriber extends Subscriber 
{
    public PongDataSubscriber(Topic<PongData> t) throws ops.ConfigurationException
    {
        super(t);

        participant.addTypeSupport(PongData.getTypeFactory());
        
    }

    public PongData getData()
    {
        return (PongData)super.getData();
    }
}
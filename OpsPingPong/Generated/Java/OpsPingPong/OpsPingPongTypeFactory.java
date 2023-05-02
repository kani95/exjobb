/**
 *
 * OPS generated code, DO NOT MODIFY!
 */
package OpsPingPong;

import configlib.Serializable;
import configlib.SerializableFactory;


public class OpsPingPongTypeFactory implements SerializableFactory
{
    public Serializable create(String type)
    {
        if(type.equals("PingPong.PingData"))
        {
            return new PingPong.PingData();
        }
        if(type.equals("PingPong.PongData"))
        {
            return new PingPong.PongData();
        }
        return null;

    }
}

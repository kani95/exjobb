/**
 *
 * OPS generated code, DO NOT MODIFY!
 */
package Config;

import configlib.Serializable;
import configlib.SerializableFactory;


public class ConfigTypeFactory implements SerializableFactory
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

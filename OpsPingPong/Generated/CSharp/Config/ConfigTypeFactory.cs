/**
 *
 * OPS generated code, DO NOT MODIFY!
 */

using Ops;

namespace Config
{
    public class ConfigTypeFactory : ISerializableFactory
    {
        public ISerializable Create(string type)
        {
            if (type.Equals("PingPong.PingData"))
            {
                return new PingPong.PingData();
            }
            if (type.Equals("PingPong.PongData"))
            {
                return new PingPong.PongData();
            }
            return null;

        }

        public string Create(object obj)
        {
            if (obj is PingPong.PingData)
            {
                return "PingPong.PingData";
            }
            if (obj is PingPong.PongData)
            {
                return "PingPong.PongData";
            }
            return null;

        }

    }
}

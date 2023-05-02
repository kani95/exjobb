#Auto generated OPS-code. DO NOT MODIFY!
from ops.Factory import BasicFactory
from PingPong import PingData
from PingPong import PongData


class ConfigTypeFactory(BasicFactory):
	def __init__(self):
		super(ConfigTypeFactory,self).__init__()
		self.addType("PingPong.PingData",PingData)
		self.addType("PingPong.PongData",PongData)

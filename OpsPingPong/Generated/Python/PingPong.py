#Auto generated OPS-code. DO NOT MODIFY!
import sys
#Helper for handling Python 2 and 3 differences
if sys.version_info < (3,):
  integer_types = (int, long,)
  numeric_types = (float, int, long,)
else:
  integer_types = (int,)
  numeric_types = (float, int,)

from ops.opsTypes import IdlVersionError
from ops.opsTypes import OPS_Object

class PingData(OPS_Object):
	TypeName = "PingPong.PingData"
	PINGDATA_IDLVERSION = 0

	def __init__(self):
		super(PingData,self).__init__()
		self.appendType("PingPong.PingData")
		self.PingData_version = PingData.PINGDATA_IDLVERSION
		self.filler = [0 for x in range(3)]
		self.time = 0

	def serialize(self,archiver):
		super(PingData,self).serialize(archiver)
		if self.idlVersionMask != 0:
			self.PingData_version = archiver.Int8("PingData_version", self.PingData_version)
			if self.PingData_version > PingData.PINGDATA_IDLVERSION:
				raise IdlVersionError("PingData", self.PingData_version, PingData.PINGDATA_IDLVERSION)
			else:
				pass
		else:
			self.PingData_version = 0
		archiver.Int64Vector("filler", self.filler)
		self.time = archiver.Int64("time", self.time)

	def validate(self):
		super(PingData,self).validate()
		if len(self.filler) != 3:
			raise ValueError()
		for x in self.filler:
			if not isinstance(x,integer_types):
				raise ValueError()
		if not isinstance(self.time,integer_types):
			raise ValueError()

class PongData(OPS_Object):
	TypeName = "PingPong.PongData"
	PONGDATA_IDLVERSION = 0

	def __init__(self):
		super(PongData,self).__init__()
		self.appendType("PingPong.PongData")
		self.PongData_version = PongData.PONGDATA_IDLVERSION
		self.filler = [0 for x in range(3)]
		self.time = 0

	def serialize(self,archiver):
		super(PongData,self).serialize(archiver)
		if self.idlVersionMask != 0:
			self.PongData_version = archiver.Int8("PongData_version", self.PongData_version)
			if self.PongData_version > PongData.PONGDATA_IDLVERSION:
				raise IdlVersionError("PongData", self.PongData_version, PongData.PONGDATA_IDLVERSION)
			else:
				pass
		else:
			self.PongData_version = 0
		archiver.Int64Vector("filler", self.filler)
		self.time = archiver.Int64("time", self.time)

	def validate(self):
		super(PongData,self).validate()
		if len(self.filler) != 3:
			raise ValueError()
		for x in self.filler:
			if not isinstance(x,integer_types):
				raise ValueError()
		if not isinstance(self.time,integer_types):
			raise ValueError()



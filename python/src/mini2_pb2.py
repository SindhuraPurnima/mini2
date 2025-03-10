# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: mini2.proto
# Protobuf Python Version: 5.29.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    29,
    0,
    '',
    'mini2.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0bmini2.proto\x12\x05mini2\"\xe5\x06\n\rCollisionData\x12\x12\n\ncrash_date\x18\x01 \x01(\t\x12\x12\n\ncrash_time\x18\x02 \x01(\t\x12\x0f\n\x07\x62orough\x18\x03 \x01(\t\x12\x10\n\x08zip_code\x18\x04 \x01(\t\x12\x10\n\x08latitude\x18\x05 \x01(\x01\x12\x11\n\tlongitude\x18\x06 \x01(\x01\x12\x10\n\x08location\x18\x07 \x01(\t\x12\x16\n\x0eon_street_name\x18\x08 \x01(\t\x12\x19\n\x11\x63ross_street_name\x18\t \x01(\t\x12\x17\n\x0foff_street_name\x18\n \x01(\t\x12!\n\x19number_of_persons_injured\x18\x0b \x01(\x05\x12 \n\x18number_of_persons_killed\x18\x0c \x01(\x05\x12%\n\x1dnumber_of_pedestrians_injured\x18\r \x01(\x05\x12$\n\x1cnumber_of_pedestrians_killed\x18\x0e \x01(\x05\x12!\n\x19number_of_cyclist_injured\x18\x0f \x01(\x05\x12 \n\x18number_of_cyclist_killed\x18\x10 \x01(\x05\x12\"\n\x1anumber_of_motorist_injured\x18\x11 \x01(\x05\x12!\n\x19number_of_motorist_killed\x18\x12 \x01(\x05\x12%\n\x1d\x63ontributing_factor_vehicle_1\x18\x13 \x01(\t\x12%\n\x1d\x63ontributing_factor_vehicle_2\x18\x14 \x01(\t\x12%\n\x1d\x63ontributing_factor_vehicle_3\x18\x15 \x01(\t\x12%\n\x1d\x63ontributing_factor_vehicle_4\x18\x16 \x01(\t\x12%\n\x1d\x63ontributing_factor_vehicle_5\x18\x17 \x01(\t\x12\x14\n\x0c\x63ollision_id\x18\x18 \x01(\t\x12\x1b\n\x13vehicle_type_code_1\x18\x19 \x01(\t\x12\x1b\n\x13vehicle_type_code_2\x18\x1a \x01(\t\x12\x1b\n\x13vehicle_type_code_3\x18\x1b \x01(\t\x12\x1b\n\x13vehicle_type_code_4\x18\x1c \x01(\t\x12\x1b\n\x13vehicle_type_code_5\x18\x1d \x01(\t\"x\n\x0e\x41nalysisRecord\x12\x12\n\ncrash_date\x18\x01 \x01(\t\x12\x0f\n\x07\x62orough\x18\x02 \x01(\t\x12\x10\n\x08zip_code\x18\x03 \x01(\t\x12\x17\n\x0fpersons_injured\x18\x04 \x01(\x05\x12\x16\n\x0epersons_killed\x18\x05 \x01(\x05\":\n\x0e\x43ollisionBatch\x12(\n\ncollisions\x18\x01 \x03(\x0b\x32\x14.mini2.CollisionData\"e\n\x0eRiskAssessment\x12\x0f\n\x07\x62orough\x18\x01 \x01(\t\x12\x10\n\x08zip_code\x18\x02 \x01(\t\x12\x16\n\x0etotal_injuries\x18\x03 \x01(\x05\x12\x18\n\x10total_fatalities\x18\x04 \x01(\x05\"\x07\n\x05\x45mpty2O\n\x11\x45ntryPointService\x12:\n\x10StreamCollisions\x12\x14.mini2.CollisionData\x1a\x0c.mini2.Empty\"\x00(\x01\x32\x82\x01\n\x12InterServerService\x12\x34\n\x0b\x46orwardData\x12\x15.mini2.CollisionBatch\x1a\x0c.mini2.Empty\"\x00\x12\x36\n\rShareAnalysis\x12\x15.mini2.RiskAssessment\x1a\x0c.mini2.Empty\"\x00\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'mini2_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_COLLISIONDATA']._serialized_start=23
  _globals['_COLLISIONDATA']._serialized_end=892
  _globals['_ANALYSISRECORD']._serialized_start=894
  _globals['_ANALYSISRECORD']._serialized_end=1014
  _globals['_COLLISIONBATCH']._serialized_start=1016
  _globals['_COLLISIONBATCH']._serialized_end=1074
  _globals['_RISKASSESSMENT']._serialized_start=1076
  _globals['_RISKASSESSMENT']._serialized_end=1177
  _globals['_EMPTY']._serialized_start=1179
  _globals['_EMPTY']._serialized_end=1186
  _globals['_ENTRYPOINTSERVICE']._serialized_start=1188
  _globals['_ENTRYPOINTSERVICE']._serialized_end=1267
  _globals['_INTERSERVERSERVICE']._serialized_start=1270
  _globals['_INTERSERVERSERVICE']._serialized_end=1400
# @@protoc_insertion_point(module_scope)

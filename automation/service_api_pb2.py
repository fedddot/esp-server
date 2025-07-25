# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: service_api.proto
# Protobuf Python Version: 6.31.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    6,
    31,
    0,
    '',
    'service_api.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x11service_api.proto\x12\x0bservice_api\"{\n\x14ThermostatApiRequest\x12.\n\x0crequest_type\x18\x01 \x01(\x0e\x32\x18.service_api.RequestType\x12\x17\n\x0fset_temperature\x18\x02 \x01(\x02\x12\x1a\n\x12time_resolution_ms\x18\x03 \x01(\r\"n\n\x15ThermostatApiResponse\x12\'\n\x06status\x18\x01 \x01(\x0e\x32\x17.service_api.StatusCode\x12\x0f\n\x07message\x18\x02 \x01(\t\x12\x1b\n\x13\x63urrent_temperature\x18\x03 \x01(\x02*0\n\x0bRequestType\x12\t\n\x05START\x10\x00\x12\x08\n\x04STOP\x10\x01\x12\x0c\n\x08GET_TEMP\x10\x02*&\n\nStatusCode\x12\x0b\n\x07SUCCESS\x10\x00\x12\x0b\n\x07\x46\x41ILURE\x10\x01\x32o\n\x11ThermostatService\x12Z\n\x11\x43ontrolThermostat\x12!.service_api.ThermostatApiRequest\x1a\".service_api.ThermostatApiResponseb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'service_api_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_REQUESTTYPE']._serialized_start=271
  _globals['_REQUESTTYPE']._serialized_end=319
  _globals['_STATUSCODE']._serialized_start=321
  _globals['_STATUSCODE']._serialized_end=359
  _globals['_THERMOSTATAPIREQUEST']._serialized_start=34
  _globals['_THERMOSTATAPIREQUEST']._serialized_end=157
  _globals['_THERMOSTATAPIRESPONSE']._serialized_start=159
  _globals['_THERMOSTATAPIRESPONSE']._serialized_end=269
  _globals['_THERMOSTATSERVICE']._serialized_start=361
  _globals['_THERMOSTATSERVICE']._serialized_end=472
# @@protoc_insertion_point(module_scope)

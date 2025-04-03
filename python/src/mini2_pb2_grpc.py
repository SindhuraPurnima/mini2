# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc
import warnings

import mini2_pb2 as mini2__pb2

GRPC_GENERATED_VERSION = '1.70.0'
GRPC_VERSION = grpc.__version__
_version_not_supported = False

try:
    from grpc._utilities import first_version_is_lower
    _version_not_supported = first_version_is_lower(GRPC_VERSION, GRPC_GENERATED_VERSION)
except ImportError:
    _version_not_supported = True

if _version_not_supported:
    raise RuntimeError(
        f'The grpc package installed is at version {GRPC_VERSION},'
        + f' but the generated code in mini2_pb2_grpc.py depends on'
        + f' grpcio>={GRPC_GENERATED_VERSION}.'
        + f' Please upgrade your grpc module to grpcio>={GRPC_GENERATED_VERSION}'
        + f' or downgrade your generated code using grpcio-tools<={GRPC_VERSION}.'
    )


class EntryPointServiceStub(object):
    """Service for Python client → Server A communication
    """

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.StreamCollisions = channel.stream_unary(
                '/mini2.EntryPointService/StreamCollisions',
                request_serializer=mini2__pb2.CollisionData.SerializeToString,
                response_deserializer=mini2__pb2.Empty.FromString,
                _registered_method=True)
        self.SetDatasetInfo = channel.unary_unary(
                '/mini2.EntryPointService/SetDatasetInfo',
                request_serializer=mini2__pb2.DatasetInfo.SerializeToString,
                response_deserializer=mini2__pb2.Empty.FromString,
                _registered_method=True)


class EntryPointServiceServicer(object):
    """Service for Python client → Server A communication
    """

    def StreamCollisions(self, request_iterator, context):
        """Defines how clients stream data to Server A
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def SetDatasetInfo(self, request, context):
        """Add new method to set dataset size
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_EntryPointServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'StreamCollisions': grpc.stream_unary_rpc_method_handler(
                    servicer.StreamCollisions,
                    request_deserializer=mini2__pb2.CollisionData.FromString,
                    response_serializer=mini2__pb2.Empty.SerializeToString,
            ),
            'SetDatasetInfo': grpc.unary_unary_rpc_method_handler(
                    servicer.SetDatasetInfo,
                    request_deserializer=mini2__pb2.DatasetInfo.FromString,
                    response_serializer=mini2__pb2.Empty.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'mini2.EntryPointService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))
    server.add_registered_method_handlers('mini2.EntryPointService', rpc_method_handlers)


 # This class is part of an EXPERIMENTAL API.
class EntryPointService(object):
    """Service for Python client → Server A communication
    """

    @staticmethod
    def StreamCollisions(request_iterator,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.stream_unary(
            request_iterator,
            target,
            '/mini2.EntryPointService/StreamCollisions',
            mini2__pb2.CollisionData.SerializeToString,
            mini2__pb2.Empty.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

    @staticmethod
    def SetDatasetInfo(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/mini2.EntryPointService/SetDatasetInfo',
            mini2__pb2.DatasetInfo.SerializeToString,
            mini2__pb2.Empty.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)


class InterServerServiceStub(object):
    """Service for inter-server communication (A,B,C,D,E)
    """

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.ForwardData = channel.unary_unary(
                '/mini2.InterServerService/ForwardData',
                request_serializer=mini2__pb2.CollisionBatch.SerializeToString,
                response_deserializer=mini2__pb2.Empty.FromString,
                _registered_method=True)
        self.ShareAnalysis = channel.unary_unary(
                '/mini2.InterServerService/ShareAnalysis',
                request_serializer=mini2__pb2.RiskAssessment.SerializeToString,
                response_deserializer=mini2__pb2.Empty.FromString,
                _registered_method=True)
        self.SetTotalDatasetSize = channel.unary_unary(
                '/mini2.InterServerService/SetTotalDatasetSize',
                request_serializer=mini2__pb2.DatasetInfo.SerializeToString,
                response_deserializer=mini2__pb2.Empty.FromString,
                _registered_method=True)


class InterServerServiceServicer(object):
    """Service for inter-server communication (A,B,C,D,E)
    """

    def ForwardData(self, request, context):
        """Forward data between servers
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ShareAnalysis(self, request, context):
        """Share analysis results
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def SetTotalDatasetSize(self, request, context):
        """Add new method
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_InterServerServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'ForwardData': grpc.unary_unary_rpc_method_handler(
                    servicer.ForwardData,
                    request_deserializer=mini2__pb2.CollisionBatch.FromString,
                    response_serializer=mini2__pb2.Empty.SerializeToString,
            ),
            'ShareAnalysis': grpc.unary_unary_rpc_method_handler(
                    servicer.ShareAnalysis,
                    request_deserializer=mini2__pb2.RiskAssessment.FromString,
                    response_serializer=mini2__pb2.Empty.SerializeToString,
            ),
            'SetTotalDatasetSize': grpc.unary_unary_rpc_method_handler(
                    servicer.SetTotalDatasetSize,
                    request_deserializer=mini2__pb2.DatasetInfo.FromString,
                    response_serializer=mini2__pb2.Empty.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'mini2.InterServerService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))
    server.add_registered_method_handlers('mini2.InterServerService', rpc_method_handlers)


 # This class is part of an EXPERIMENTAL API.
class InterServerService(object):
    """Service for inter-server communication (A,B,C,D,E)
    """

    @staticmethod
    def ForwardData(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/mini2.InterServerService/ForwardData',
            mini2__pb2.CollisionBatch.SerializeToString,
            mini2__pb2.Empty.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

    @staticmethod
    def ShareAnalysis(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/mini2.InterServerService/ShareAnalysis',
            mini2__pb2.RiskAssessment.SerializeToString,
            mini2__pb2.Empty.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

    @staticmethod
    def SetTotalDatasetSize(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/mini2.InterServerService/SetTotalDatasetSize',
            mini2__pb2.DatasetInfo.SerializeToString,
            mini2__pb2.Empty.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

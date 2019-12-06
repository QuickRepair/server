cmake_minimum_required(VERSION 3.7)
project(QuickRepairRestHandler)

set(_PROTOC_INCLUDE_PATH ${CMAKE_HOME_DIRECTORY}/third_party/grpc/third_party/protobuf/src/)
set(_GRPC_PYTHON_PLUGIN_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/../grpc/grpc_python_plugin)
set(_PROTOC_PATH ${CMAKE_CURRENT_BINARY_DIR}/../grpc/third_party/protobuf/protoc)
set(GRPC_OUT QuickRepair_pb2.py QuickRepair_pb2_grpc.py)
add_custom_target(
        QuickRepairRestHandler ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${_PROTOC_PATH} -I${_PROTOC_INCLUDE_PATH} -I../../protos --python_out=./ ../../protos/QuickRepair.proto
        COMMAND ${_PROTOC_PATH} -I${_PROTOC_INCLUDE_PATH} -I../../protos --grpc_out=./ --plugin=protoc-gen-grpc=${_GRPC_PYTHON_PLUGIN_EXECUTABLE} ../../protos/QuickRepair.proto
        USES_TERMINAL
        COMMENT "generate c++ grpc files"
        DEPENDS ../../protos/QuickRepair.proto
)
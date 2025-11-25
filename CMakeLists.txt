cmake_minimum_required(VERSION 3.16)
project(PSET14)

set(CMAKE_CXX_STANDARD 17)

# === Qt ===
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# === Proto + gRPC ===
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

set(PROTO_FILE proto/ocr.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_FILE})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

# === Server ===
add_executable(server
    server/main.cpp
    server/ocr_server.cpp
    server/ocr_server.h
    server/worker_pool.cpp
    server/worker_pool.h
    ${PROTO_SRCS} ${PROTO_HDRS}
    ${GRPC_SRCS} ${GRPC_HDRS}
)

target_link_libraries(server
    gRPC::grpc++
    ${Protobuf_LIBRARIES}
    Tesseract::libtesseract
)

# === Client ===
add_executable(client
    client/main.cpp
    client/mainwindow.cpp
    client/mainwindow.h
    client/ocr_client.cpp
    client/ocr_client.h
    ${PROTO_SRCS} ${PROTO_HDRS}
    ${GRPC_SRCS} ${GRPC_HDRS}
)

target_link_libraries(client
    Qt6::Widgets
    gRPC::grpc++
    ${Protobuf_LIBRARIES}
)

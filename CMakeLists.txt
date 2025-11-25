cmake_minimum_required(VERSION 3.16)
project(PSET4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt (MSVC)
set(CMAKE_PREFIX_PATH "C:/QtMSVC/6.9.3/msvc2022_64")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# vcpkg libraries
set(VCPKG_ROOT "C:/Users/user/vcpkg/installed/x64-windows")
include_directories(${VCPKG_ROOT}/include)
link_directories(${VCPKG_ROOT}/lib)

# Proto files (hardcode generated files if needed)
set(PROTO_SRCS proto/ocr.pb.cc)
set(PROTO_HDRS proto/ocr.pb.h)
set(GRPC_SRCS proto/ocr.grpc.pb.cc)
set(GRPC_HDRS proto/ocr.grpc.pb.h)

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
    PRIVATE
    gRPC::grpc++
    protobuf::libprotobuf
    tesseract::tesseract
    Leptonica::leptonica
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
    PRIVATE
    Qt6::Widgets
    gRPC::grpc++
    protobuf::libprotobuf
    tesseract::tesseract
    Leptonica::leptonica
)

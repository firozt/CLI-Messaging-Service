cmake_minimum_required(VERSION 3.14)
project(p2p-messenger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files (including main.cpp explicitly)
file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Add main.cpp
set(SOURCES ${SOURCES} ${PROJECT_SOURCE_DIR}/main.cpp
        src/utils/TerminalInput.cpp
        src/utils/TerminalInput.h
        src/net/SignalingServer.cpp
        src/net/SignalingServer.h
        src/net/BaseServerUDP.cpp
        src/net/BaseServerUDP.h)

# Create the executable
add_executable(p2p-messenger ${SOURCES})

# add src to path
target_include_directories(p2p-messenger PRIVATE ${PROJECT_SOURCE_DIR}/src)

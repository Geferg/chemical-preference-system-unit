cmake_minimum_required(VERSION 3.25)
project(TcpDebugClean)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(TcpDebugClean main.cpp
        Formatting/Argument.h
        Formatting/Instruction.cpp
        Formatting/Instruction.h
        Formatting/Enums.h
        #Tcp/TcpConnection.cpp
        #Tcp/TcpConnection.h
        Formatting/JsonFormatter.cpp
        Formatting/JsonFormatter.h
        Formatting/JsonImplementation.cpp
        #Tcp/ThreadSafeQueue.h
        Formatting/Protocol.cpp
        Formatting/Protocol.h
        TcpLinuxFuture/ClientQueue.h
        TcpLinuxFuture/ServerHandler.cpp
        TcpLinuxFuture/ServerHandler.h
        Hardware/HEnums.h
        Hardware/ValveHandler.cpp
        Hardware/ValveHandler.h
        Hardware/MotorHandler.cpp
        Hardware/MotorHandler.h
        Hardware/HardwareControl.cpp
        Hardware/HardwareControl.h
)

target_link_libraries(TcpDebugClean
        #ws2_32
        pigpio
)
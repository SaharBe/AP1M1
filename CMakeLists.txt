cmake_minimum_required(VERSION 3.15)
project(untitled8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(ex3 ex3.cpp ex3.h interpreter.cpp interpreter.h  ex1.cpp ex1.cpp main.cpp OpenServerCommand.cpp ConnectControlCommand.cpp ConnectControlCommand.h OpenServerCommand.h)
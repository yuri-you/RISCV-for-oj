cmake_minimum_required(VERSION 3.5)
project(RISCV)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(code main.cpp register.hpp instruction_fetch.hpp instruction_decode.hpp memory_access.hpp execute.hpp write_back.hpp)
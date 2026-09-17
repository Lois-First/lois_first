cmake_minimum_required(VERSION 3.10)
project(MyStringProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyStringTest
    MyString.cpp
    main.cpp
)

target_compile_options(MyStringTest PRIVATE -Wall -Wextra)
cmake_minimum_required(VERSION 3.30)
project(Cryptography_Task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(proto)
add_subdirectory(src/common)
add_subdirectory(src/auth)
add_subdirectory(src/mta)
add_subdirectory(src/client)
add_subdirectory(src/server)

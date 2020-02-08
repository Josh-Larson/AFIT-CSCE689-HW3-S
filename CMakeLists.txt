cmake_minimum_required(VERSION 3.10.2)
project(AFIT-CSCE689-HW3)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW3 src/main.cpp
               src/PCalc.cpp    include/PCalc.h
               src/PCalc_SP.cpp include/PCalc_SP.h
               src/PCalc_T.cpp  include/PCalc_T.h)

target_include_directories(HW3 PRIVATE src include)

target_link_libraries(HW3 pthread)

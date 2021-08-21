cmake_minimum_required(VERSION 3.5)

project(siegel2021)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)


add_library(siegel2021
    siegel2021.h
    linear_algebra/*.h
    matrix/*.h
    vector/*.h
    /utils/*.h
}
CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

SET (ProJect_Name OpenglWindow)

PROJECT (${ProJect_Name})

IF (SET_CPP11)
	# 定义代码中使用CPP11语法，需要高版本VS支持
	ADD_DEFINITIONS (-DCPP11)
ENDIF (SET_CPP11)

# Common Option
OPTION (SET_USE_OPENMP "Use OpenMP" ON)

# 支持C++17
SET(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL)
# 宏控制
# 是否使用默认版本号
OPTION (USE_DEFAULT_VERSION "DEFAULT_VERSION" OFF)

#项目控制
# Project Option
OPTION (PROJECT_OPENGL "GLADGUI" ON)

# 设置公共路径
#SET (COMMON ../Common/)

message("{CMAKE_BINARY_DIR} is ${CMAKE_BINARY_DIR}")
# 设置输出路径
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../Bin)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ../Bin)
# 设置默认构建类型为Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
# 设置不同构建类型的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)

# 根据构建类型设置不同的变量
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode. Executables will be placed in bin/Debug")
    set(MY_PROJECT_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
else()
    message("Building in Release mode. Executables will be placed in bin/Release")
    set(MY_PROJECT_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
endif()


# Projects
IF (PROJECT_OPENGL)
	ADD_SUBDIRECTORY (OpenglGui)
ENDIF (PROJECT_OPENGL)	
	

















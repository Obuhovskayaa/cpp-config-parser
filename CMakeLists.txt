
cmake_minimum_required(VERSION 3.10) 

project(cpp-config-parser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_library(config_parser INTERFACE)

#target_include_directories(config_parser INTERFACE 
#	${CMAKE_CURRENT_SOURCE_DIR}/include
#) commented

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


set(PROJECT_SOURCES 
	${SOURCE_DIR}/ConfigValue.cpp
	${SOURCE_DIR}/ConfigNode.cpp
	${SOURCE_DIR}/JsonParser.cpp
)

add_library(config_parser STATIC
	${PROJECT_SOURCES}
)

target_include_directories(config_parser PUBLIC ${INCLUDE_DIR})

add_executable(config_main src/main.cpp)

target_link_libraries(config_main PRIVATE config_parser)
cmake_minimum_required(VERSION 3.10)

project(cpp-config-parser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if(MSVC)
#	add_compile_options(/W4 /WX)
#else()
#	add_compile_options(-Wall -Wextra -Werror)
#endif()

add_library(config_value STATIC
	src/ConfigValue.cpp
)

target_include_directories(config_value 
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	include
)

add_executable(config_test
	src/main.cpp
)

target_link_libraries(config_test config_value)

































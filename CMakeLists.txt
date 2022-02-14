
cmake_minimum_required(VERSION 3.0)

PROJECT(modern_cpp)

set(
	CMAKE_CXX_STANDARD
	20
)

file(
	GLOB
	src
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_executable(
	${PROJECT_NAME}
	${src}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_directory_properties(
	PROPERTIES
	VS_STARTUP_PROJECT
	${PROJECT_NAME}
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/workspace/"
	VS_DEBUGGER_ENVIRONMENT
	"PATH=PATH;c:/"
)


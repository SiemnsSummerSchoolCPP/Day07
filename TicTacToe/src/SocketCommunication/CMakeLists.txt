cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(SocketCommunication)

set(CMAKE_CXX_STANDARD 11)

file(GLOB includes "${CMAKE_SOURCE_DIR}/includes/SocketCommunication/*")
file(GLOB srcs "*.cpp" "*.h" "*.hpp")
add_library(${PROJECT_NAME} SHARED ${includes} ${srcs})

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_SOURCE_DIR}/includes/SocketCommunication/")

set_target_properties(${PROJECT_NAME} PROPERTIES
	LINKER_LANGUAGE				CXX
	ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
)

String(CONCAT Flags ${Flags} " " "-Wall")
# String(CONCAT Flags ${Flags} " " "-Wextra")
String(CONCAT Flags ${Flags} " " "-Werror")
String(CONCAT Flags ${Flags} " " "-Waddress")
String(CONCAT Flags ${Flags} " " "-Waggregate-return")
Set(CMAKE_CXX_FLAGS "${Flags}")
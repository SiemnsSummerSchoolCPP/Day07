cmake_minimum_required(VERSION 3.0.0)

set(ProjName "Exe")
project(${ProjName})

set(CMAKE_CXX_STANDARD 11)

add_executable(${ProjName} "main.cpp")

target_link_libraries(${ProjName} "PalindromeLib")
target_include_directories(${ProjName} PRIVATE "${CMAKE_SOURCE_DIR}/includes/")

set_target_properties(${ProjName} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
    EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
)
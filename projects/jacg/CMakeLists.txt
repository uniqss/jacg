cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME uconfig)
PROJECT(${PROJECT_NAME} LANGUAGES CXX)

#set(CMAKE_UNITY_BUILD yes)
#set(CMAKE_UNITY_BUILD_BATCH_SIZE 16)

#=======================================================================================================================
# Set C/C++ compiler version
#=======================================================================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(IGNORE_ME ${CMAKE_C_COMPILER}) # Suppress CMake warning message

if (WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
else()
	#find_package(asio CONFIG REQUIRED)
endif()

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/uniqs_xlsx_reader
	${CMAKE_CURRENT_SOURCE_DIR}/cfg
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../include/OpenXLSX
	)

file(GLOB PROJECT_SRC_FILES
	"src/*.cpp"
	"src/*.h"
	"uniqs_xlsx_reader/*.cpp"
	"uniqs_xlsx_reader/*.h"
	"cfg/*.cpp"
	"cfg/*.h"
	"include/*.h"
	)

source_group(src FILES ${PROJECT_SRC_FILES})
list(APPEND SELF_SRC_FILES ${PROJECT_SRC_FILES})

add_executable(${PROJECT_NAME} ${SELF_SRC_FILES})

if (WIN32)
	#target_link_libraries(${PROJECT_NAME} jemalloc)
else()
	option(OpenXLSX_SHARED "openxlsx is shared not static" OFF)
	if (OpenXLSX_SHARED)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib64)
		set(CMAKE_BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../lib64")
		#add_link_options("-Wl,--rpath=./ -Wl,-rpath-link=../lib64")
		ADD_LIBRARY(OpenXLSX SHARED IMPORTED)
		SET_TARGET_PROPERTIES(OpenXLSX PROPERTIES
			IMPORTED_LOCATION ../../lib64/libOpenXLSX.so)
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} OpenXLSX stdc++fs)
	else ()
		#link_directories(../../lib64)
		#target_link_libraries(${PROJECT_NAME} libOpenXLSX.a)
		#target_link_libraries(${PROJECT_NAME} OpenXLSX)
		ADD_LIBRARY(OpenXLSX STATIC IMPORTED)
		SET_TARGET_PROPERTIES(OpenXLSX PROPERTIES
			IMPORTED_LOCATION ../../lib64/libOpenXLSX.a)
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} OpenXLSX stdc++fs)
	endif()
endif()


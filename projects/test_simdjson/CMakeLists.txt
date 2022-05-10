cmake_minimum_required(VERSION 3.2)

SET(PNAME "test_generated")

PROJECT(${PNAME})

INCLUDE(../cmake/ModuleImport.cmake)
INCLUDE(../cmake/ModuleDepends.cmake)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
INCLUDE(../cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

SET(UNIQS_VERSION_MAJOR "1")
SET(UNIQS_VERSION_MINOR "0")
SET(UNIQS_VERSION_PATCH "1")
SET(UNIQS_VERSION "${UNIQS_VERSION_MAJOR}.${UNIQS_VERSION_MINOR}.${UNIQS_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${UNIQS_VERSION}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include/)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include/openxlsx/)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/uniqs_xlsx_reader/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/gen_cpp/)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../cfg/)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include/sol/)

FILE(GLOB project_src_cpp "src/*.cpp")
FILE(GLOB project_src_h "src/*.h")
FILE(GLOB gen_cpp_cpp "gen_cpp/*.cpp")
FILE(GLOB gen_cpp_h "gen_cpp/*.h")
FILE(GLOB project_include "include/*.h")

IF (WIN32)
    LIST(APPEND project_src_cpp)
ENDIF(WIN32)

ADD_EXECUTABLE(${PNAME} ${project_src_cpp} ${project_src_h} ${gen_cpp_cpp} ${gen_cpp_h} ${project_include})

# ModuleDependLibraryShared(${PNAME} OpenXLSXd)
# ModuleDependLibraryShared(${PNAME} OpenXLSX)

# ModuleDependLibraryShared(${PNAME} jsoncpp)
# TARGET_LINK_LIBRARIES(${PNAME} ../../lib/)

cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(videoUtil)

#Preprocessor define
add_definitions(-DDEBUG_PERCEPTION -DFREDNATOR)

#Opencv dependency
find_package( OpenCV REQUIRED )

#Perception directory
set(P_SRC_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/includes-competition-code/perception/include/")

#Perception Files
set(P_SRC   ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/include/yellowDetector.hpp
            ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/src/yellowDetector.cpp
            )

# Build a shared library named after the project from the files in `src/`
set( SOURCE_FILES "src/cpp/videoUtil.cc")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
add_library(P_SRC_LIB SHARED ${P_SRC})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC} ${OpenCV_INCLUDE_DIRS} PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" ${P_SRC_INCLUDE_DIR})

###################################################################################
# Qibuild

set(NAOQI ${NAOQI} "$ENV{HOME}/nao/devtools/sdk/naoqi-sdk-2.1.4.13-linux64")
set(NAOQI_LIB_PATH ${NAOQI_LIB_PATH} "${NAOQI}/lib")
set(INCLUDE_PATH ${INCLUDE_PATH} "${NAOQI}/include")

find_library(ALCOMMON_LIBRARY NAMES alcommon HINTS ${NAOQI_LIB_PATH})
message(STATUS ${ALCOMMON_LIBRARY})
###################################

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS} P_SRC_LIB)
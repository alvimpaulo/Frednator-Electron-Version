cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(videoUtil)

#Directories to be included for the compiler
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/motion/include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/perception/include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/communication/include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/behavior/include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/unboard
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/thread
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/common
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/module
    ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/motion/runswift
    )


#Opencv dependency
find_package( OpenCV REQUIRED )

# Build a shared library named after the project from the files in `src/`
set( SOURCE_FILES "src/cpp/videoUtil.cc")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC} ${OpenCV_INCLUDE_DIRS} PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api")

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
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS})
cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(Napi_modules)

#Name of the modules
set(MODULE_1_NAME "videoUtil")
set(MODULE_2_NAME "yellowDetectorUtil")


#Preprocessor define
add_definitions(-DDEBUG_PERCEPTION -DFREDNATOR)

#Opencv dependency
find_package( OpenCV REQUIRED)

#Boost depedency
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#Perception directory
set(P_SRC_INCLUDE_DIR 
        ${Boost_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/motion/include
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/motion/runswift
        # ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/perception/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/communication/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/behavior/include
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/unboard
        # ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/thread
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/common
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/module
        # ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/testThread/include
    )
include_directories(${P_SRC_INCLUDE_DIR})

#Perception Files
set(P_SRC   ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/include/yellowDetector.hpp
            ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/src/yellowDetector.cpp
            )

#Miscelaneous Files
set(MISC_SRC  ${CMAKE_SOURCE_DIR}/includes-competition-code/unboard/unBoard.cpp
                ${CMAKE_SOURCE_DIR}/includes-competition-code/unboard/BehaviorRequest.hpp
                ${CMAKE_SOURCE_DIR}/includes-competition-code/thread/threadClass.cpp
                ${CMAKE_SOURCE_DIR}/includes-competition-code/module/basemodule.cpp
                )

# Build a shared library named after each module from the files in `src/`
set( MODULE_1_SOURCE_FILES "src/cpp/videoUtil.cc")
add_library(${MODULE_1_NAME} SHARED ${MODULE_1_SOURCE_FILES})

set( MODULE_2_SOURCE_FILES "src/cpp/yellowDetectorUtil.cc")
add_library(${MODULE_2_NAME} SHARED ${MODULE_2_SOURCE_FILES})

#Custom Libraries
add_library(P_SRC_LIB SHARED ${P_SRC})
add_library(MISC_SRC_LIB SHARED ${MISC_SRC})


# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${MODULE_1_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${MODULE_2_NAME} PROPERTIES PREFIX "" SUFFIX ".node")



###################################################################################
# Qibuild

set(NAOQI ${NAOQI} "$ENV{HOME}/nao/devtools/sdk/naoqi-sdk-2.1.4.13-linux64")
set(NAOQI_LIB_PATH ${NAOQI_LIB_PATH} "${NAOQI}/lib")
set(NAOQI_INCLUDE_PATH ${NAOQI_INCLUDE_PATH} "${NAOQI}/include")

message(STATUS "LOOKING FOR ALCOMMON")
find_library(ALCOMMON_LIBRARY NAMES alcommon  HINTS ${NAOQI_LIB_PATH})
message(STATUS ${ALCOMMON_LIBRARY})

message(STATUS "LOOKING FOR ALPROXIES")
find_library(ALPROXIES_LIBRARY NAMES  alproxies HINTS ${NAOQI_LIB_PATH})
message(STATUS ${ALPROXIES_LIBRARY})

message(STATUS "LOOKING FOR ALVISION")
find_library(ALVISION_LIBRARY NAMES  alvision HINTS ${NAOQI_LIB_PATH})
message(STATUS ${ALVISION_LIBRARY})

message(STATUS "LOOKING FOR ALVALUES")
find_library(ALVALUES_LIBRARY NAMES  alvalue HINTS ${NAOQI_LIB_PATH})
message(STATUS ${ALVALUES_LIBRARY})

message(STATUS "LOOKING FOR QI")
find_library(QI_LIBRARY NAMES  qi HINTS ${NAOQI_LIB_PATH})
message(STATUS ${QI_LIBRARY})

message(STATUS "LOOKING FOR QITYPE")
find_library(QITYPE_LIBRARY NAMES  qitype HINTS ${NAOQI_LIB_PATH})
message(STATUS ${QITYPE_LIBRARY})


set(NAOQI_LIBS 
    ${ALCOMMON_LIBRARY} 
    ${ALPROXIES_LIBRARY} 
    ${ALVISION_LIBRARY} 
    ${ALVALUES_LIBRARY} 
    ${QI_LIBRARY}
    ${QITYPE_LIBRARY}
    )

###################################

#Disable BOOST Useless warning
add_definitions(-DBOOST_SIGNALS_NO_DEPRECATION_WARNING)

#Modules include directories
target_include_directories(${MODULE_1_NAME} 
PRIVATE ${CMAKE_JS_INC} 
${OpenCV_INCLUDE_DIRS} 
PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" 
)

target_include_directories( ${MODULE_2_NAME} 
PRIVATE ${CMAKE_JS_INC} 
${OpenCV_INCLUDE_DIRS} 
PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" 
${NAOQI_INCLUDE_PATH} 
${P_SRC_INCLUDE_DIR})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${MODULE_1_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS})
target_link_libraries(${MODULE_2_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS} ${Boost_LIBRARIES} P_SRC_LIB MISC_SRC_LIB ${NAOQI_LIBS})


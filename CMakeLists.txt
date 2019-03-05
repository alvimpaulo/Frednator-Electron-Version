cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(videoUtil)

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

# Build a shared library named after the project from the files in `src/`
set( SOURCE_FILES "src/cpp/videoUtil.cc")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

#Custom Libraries
add_library(P_SRC_LIB SHARED ${P_SRC})
add_library(MISC_SRC_LIB SHARED ${MISC_SRC})


# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} 
PRIVATE ${CMAKE_JS_INC} 
${OpenCV_INCLUDE_DIRS} 
PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" 
${P_SRC_INCLUDE_DIR}
)

###################################################################################
# Qibuild

set(NAOQI ${NAOQI} "$ENV{HOME}/nao/devtools/sdk/naoqi-sdk-2.1.4.13-linux64")
set(NAOQI_LIB_PATH ${NAOQI_LIB_PATH} "${NAOQI}/lib")
set(INCLUDE_PATH ${INCLUDE_PATH} "${NAOQI}/include")
include_directories(${INCLUDE_PATH})

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

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS} ${Boost_LIBRARIES} P_SRC_LIB MISC_SRC_LIB ${NAOQI_LIBS})
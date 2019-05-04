cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(Napi_modules)

#File with all the include directories
set(INCLUDE_DIR_FILE "include_dir_file.txt")
file(WRITE ${INCLUDE_DIR_FILE} "Start:\n")

#Name of the modules
set(MODULE_1_NAME "videoUtil")

# $start CMakeLists.txt:moduleNameSet$
# $start CMakeLists.txt:moduleNameSet:yellowDetector$
set(MODULE_2_NAME "yellowDetectorUtil")
# $end CMakeLists.txt:moduleNameSet:yellowDetector$

# $start CMakeLists.txt:moduleNameSet:lineDetector$
set(MODULE_3_NAME "lineDetectorUtil")
# $end CMakeLists.txt:moduleNameSet:lineDetector$

# $end CMakeLists.txt:moduleNameSet$

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
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/unboard
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/common
        ${CMAKE_CURRENT_SOURCE_DIR}/includes-competition-code/module
    )
include_directories(${P_SRC_INCLUDE_DIR})

#Perception Files
set(P_SRC 
    #$start CMakeLists.txt:P_SRC$

    #$start CMakeLists.txt:P_SRC:yellowDetector$
    ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/src/yellowDetector.cpp
    ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/include/yellowDetector.hpp
    #$end CMakeLists.txt:P_SRC:yellowDetector$
    
    #$start CMakeLists.txt:P_SRC:lineDetector$
    ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/src/lineDetector.cpp
    ${CMAKE_SOURCE_DIR}/includes-competition-code/perception/include/lineDetector.hpp
#$end CMakeLists.txt:P_SRC:lineDetector$

#$end CMakeLists.txt:P_SRC$
)

#Miscelaneous Files
set(MISC_SRC  
    ${CMAKE_SOURCE_DIR}/includes-competition-code/unboard/unBoard.cpp
    ${CMAKE_SOURCE_DIR}/includes-competition-code/unboard/BehaviorRequest.hpp
    ${CMAKE_SOURCE_DIR}/includes-competition-code/thread/threadClass.cpp
    ${CMAKE_SOURCE_DIR}/includes-competition-code/module/basemodule.cpp
)

# Build a shared library named after each module from the files in `src/`
set( MODULE_1_SOURCE_FILES "src/cpp/videoUtil.cc")
add_library(${MODULE_1_NAME} SHARED ${MODULE_1_SOURCE_FILES})

#$start CMakeLists.txt:libraryBuilds$

#$start CMakeLists.txt:libraryBuilds:yellowDetector$
set( MODULE_2_SOURCE_FILES "src/cpp/yellowDetectorUtil.cc")
add_library(${MODULE_2_NAME} SHARED ${MODULE_2_SOURCE_FILES})
#$end CMakeLists.txt:libraryBuilds:yellowDetector$

#$start CMakeLists.txt:libraryBuilds:lineDetector$
set( MODULE_3_SOURCE_FILES "src/cpp/lineDetectorUtil.cc")
add_library(${MODULE_3_NAME} SHARED ${MODULE_3_SOURCE_FILES})
#$end CMakeLists.txt:libraryBuilds:lineDetector$

#$end CMakeLists.txt:libraryBuilds$

#Custom Libraries
add_library(P_SRC_LIB SHARED ${P_SRC})
add_library(MISC_SRC_LIB SHARED ${MISC_SRC})


# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${MODULE_1_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

#$start CMakeLists.txt:targetProperties$

#$start CMakeLists.txt:targetProperties:yellowDetector$
set_target_properties(${MODULE_2_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
#$end CMakeLists.txt:targetProperties:yellowDetector$

#$start CMakeLists.txt:targetProperties:lineDetector$
set_target_properties(${MODULE_3_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
#$end CMakeLists.txt:targetProperties:lineDetector$

#$end CMakeLists.txt:targetProperties$



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

#$start CMakeLists.txt:includeDirectories$

#$start CMakeLists.txt:includeDirectories:yellowDetector$
target_include_directories( ${MODULE_2_NAME} 
PRIVATE ${CMAKE_JS_INC} 
${OpenCV_INCLUDE_DIRS} 
PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" 
${NAOQI_INCLUDE_PATH} 
${P_SRC_INCLUDE_DIR})
#$end CMakeLists.txt:includeDirectories:yellowDetector$

#$start CMakeLists.txt:includeDirectories:lineDetector$
target_include_directories( ${MODULE_3_NAME} 
PRIVATE ${CMAKE_JS_INC} 
${OpenCV_INCLUDE_DIRS} 
PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" 
${NAOQI_INCLUDE_PATH} 
${P_SRC_INCLUDE_DIR})
#$end CMakeLists.txt:includeDirectories:lineDetector$

#$end CMakeLists.txt:includeDirectories$


file(APPEND ${INCLUDE_DIR_FILE} 
"${CMAKE_JS_INC}" ";"
"${OpenCV_INCLUDE_DIRS}" ";"
"${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" ";"
"${NAOQI_INCLUDE_PATH}" ";"
"${P_SRC_INCLUDE_DIR}")

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${MODULE_1_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS})

#$start CMakeLists.txt:linkLibraries$

#$start CMakeLists.txt:linkLibraries:yellowDetector$
target_link_libraries(${MODULE_2_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS} ${Boost_LIBRARIES} P_SRC_LIB MISC_SRC_LIB ${NAOQI_LIBS})
#$end CMakeLists.txt:linkLibraries:yellowDetector$

#$start CMakeLists.txt:linkLibraries:lineDetector$
target_link_libraries(${MODULE_3_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS} ${Boost_LIBRARIES} P_SRC_LIB MISC_SRC_LIB ${NAOQI_LIBS})
#$end CMakeLists.txt:linkLibraries:lineDetector$

#$end CMakeLists.txt:linkLibraries$


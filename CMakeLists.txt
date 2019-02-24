cmake_minimum_required(VERSION 2.8)

# Name of the project (will be the name of the plugin)
project(videoUtil)

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

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS})
cmake_minimum_required(VERSION 3.7)

project(C3DFXProject CXX)

set (CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES 
  "Sources/*.h"
  "Sources/*.cpp")

set(3DFX_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/3dfx-Glide-API-master/glide3x/cvg/glide3/src")
set(3DFX_FXMISC_DIRS "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/3dfx-Glide-API-master/swlibs/fxmisc")
set(3DFX_CVG_DIRS "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/3dfx-Glide-API-master/glide3x/cvg/incsrc")
set(3DFX_INIT_DIRS "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/3dfx-Glide-API-master/glide3x/cvg/init")
set(TYNY_OBJ_LOADER "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/tinyobjloader-release")

set(glm_include_DIR "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/cglm-master/include")
set(glm_lib_DIR "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/cglm-master/build/Release/cglm.lib") # if necessary

set(3DFX_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/3dfx-Glide-API-master/glide3x/cvg/lib/Debug/Glide3x.lib")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

include_directories(${PROJECT_NAME} ${3DFX_INCLUDE_DIRS} ${3DFX_FXMISC_DIRS} ${3DFX_CVG_DIRS} ${3DFX_INIT_DIRS} ${glm_include_DIR} ${TYNY_OBJ_LOADER} )
target_link_libraries(${PROJECT_NAME} PRIVATE ${3DFX_LIBRARIES} ${glm_lib_DIR})
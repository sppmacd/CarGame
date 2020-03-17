cmake_minimum_required (VERSION 2.7)
project (CarGame)



set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

macro(cg_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# setup options
cg_set_option(CG_COMPILE_LAUNCHER TRUE BOOL "Compile CG Launcher")
cg_set_option(CG_ARCH_PREFIX "invalid" STRING "Folder name to use for executables and in updater file")

# set cg version
cg_set_option(CG_VER_STR "invalid" STRING "Version string (displayed in-game)")
cg_set_option(CG_VER_SIG "invalid" STRING "Version signature (used in file names)")
cg_set_option(SFML_ROOT "invalid" PATH "SFML 2.4.2 root")

# add version to be used in game
#configure_file ("${PROJECT_SOURCE_DIR}/version.cpp.in" "${PROJECT_SOURCE_DIR}/version.cpp")

# include directories
include_directories("${PROJECT_BINARY_DIR}")

set(EXEC "cargame-${CG_VER_SIG}")

# add launcher exec if required
if(CG_COMPILE_LAUNCHER)
	message("CG: compiling launcher")
	message("CG WARNING: Launcher is not compiling yet, skipping")
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cglauncher)
	#file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "cglauncher/*.cpp" "cglauncher/libs/ziputils/*.c")
	#add_executable(cglauncher "${SOURCES}")
	#target_link_libraries(cglauncher "winhttp")
	#target_include_directories(cglauncher PUBLIC "${CMAKE_SOURCE_DIR}/cglauncher/libs/*")
endif()

# add CG binaries to build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC}/${CG_ARCH_PREFIX})

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "cgui_fork/*.cpp")
add_executable(cg "${SOURCES}")
add_executable(cgd "${SOURCES}")

# find and add SFML
cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
set(SFML_STATIC_LIBRARIES TRUE)
message("CG: SFML root is ${SFML_ROOT}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
	message("CG: adding SFML as dependency")
	
	target_include_directories(cg PUBLIC "${SFML_INCLUDE_DIR}")
	target_include_directories(cgd PUBLIC "${SFML_INCLUDE_DIR}")
	
	target_link_libraries(cg ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	target_link_libraries(cgd ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# add lib directory
link_directories(${CMAKE_BINARY_DIR}/lib)

# compile and add hmutil to cg and cgd
message("CG: compiling and adding hmutil")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/hmutil/include/HackerMan/Util/*.cpp")
add_library(hmutil "${SOURCES}")
target_include_directories(hmutil PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")

target_link_libraries(cg hmutil)
target_link_libraries(cgd hmutil)

# add cg and hmutil libraries to compiler cmd
message("CG: adding headers")
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(cgd PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/cgui_fork/include")
target_include_directories(cgd PUBLIC "${CMAKE_SOURCE_DIR}/cgui_fork/include")
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")
target_include_directories(cgd PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")

message("CG: successfully configured CG")

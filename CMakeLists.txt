cmake_minimum_required (VERSION 2.8)
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
cg_set_option(CG_COMPILE_DEBUG TRUE BOOL "Compile debug version of CG")
cg_set_option(CG_ARCH_PREFIX "invalid" STRING "Folder name to use for executables and in updater file")

# set cg version
cg_set_option(CG_VER_STR "invalid" STRING "Version string (displayed in-game)")
cg_set_option(CG_VER_SIG "invalid" STRING "Version signature (used in file names)")
cg_set_option(SFML_ROOT "invalid" PATH "SFML 2.4.2 root")

# add version to be used in game
message("CG: version is '${CG_VER_STR}'")
configure_file ("${PROJECT_SOURCE_DIR}/src/version.cpp.in" "${PROJECT_SOURCE_DIR}/src/version.cpp")

# include directories
include_directories("${PROJECT_BINARY_DIR}")

set(EXEC "cargame-${CG_VER_SIG}")

# add launcher exec if required
if(CG_COMPILE_LAUNCHER)
	message("CG: compiling launcher")
	message("CG: IMPORTANT WARNING: Launcher is not compiling yet, it will result in errors!")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cglauncher)
	file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "cglauncher/*.cpp" "cglauncher/libs/ziputils/*.c" "src/*.rc")
	add_executable(cglauncher "${SOURCES}")
	target_link_libraries(cglauncher "winhttp")
	target_include_directories(cglauncher PUBLIC "${CMAKE_SOURCE_DIR}/cglauncher/libs/*")
endif()

# add CG binaries to build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC}/${CG_ARCH_PREFIX})

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "src/*.rc" "cgui_fork/src/*.cpp" "src/*.hpp" "cgui_fork/include/CG/*.hpp" "src/*.h" "cgui_fork/include/CG/*.h" "cgui_fork/include/CG/*.hpp")
set(CMAKE_BUILD_TYPE "Release")
message("CG: sources are ${SOURCES}")
add_executable(cg "${SOURCES}")

if(CG_COMPILE_DEBUG)
	set(CMAKE_BUILD_TYPE "Debug")
	add_executable(cgd "${SOURCES}")
endif()

# copy resources to binary folder
message("CG: copying resource dir to ${EXEC}")
file(COPY res/ DESTINATION ${EXEC})

# find and add SFML
cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
set(SFML_STATIC_LIBRARIES TRUE)
message("CG: SFML root is ${SFML_ROOT}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
	message("CG: adding SFML as dependency")
	
	target_include_directories(cg PUBLIC "${SFML_INCLUDE_DIR}")
	target_link_libraries(cg ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	
	if(CG_COMPILE_DEBUG)
		target_include_directories(cgd PUBLIC "${SFML_INCLUDE_DIR}")
		target_link_libraries(cgd ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	endif()	
endif()

# add lib directory
link_directories(${CMAKE_BINARY_DIR}/lib)

# compile hmutil
message("CG: compiling and adding hmutil")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/hmutil/include/HackerMan/Util/*.cpp")
add_library(hmutil "${SOURCES}")
target_include_directories(hmutil PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")

# add cg and hmutil libraries to compiler cmd
message("CG: adding headers")
target_link_libraries(cg hmutil)
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/cgui_fork/include")
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")

if(CG_COMPILE_DEBUG)
	target_link_libraries(cgd hmutil)
	target_include_directories(cgd PUBLIC "${CMAKE_SOURCE_DIR}/src")
	target_include_directories(cgd PUBLIC "${CMAKE_SOURCE_DIR}/cgui_fork/include")
	target_include_directories(cgd PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")
endif()

# force add headers to project when configuring for IDE

# set working directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC})
message("CG: successfully configured CG ${CG_VER_STR}")

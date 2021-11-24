##############################
# CARGAME CMake Build System #
#      Ver. 0.3-beta.1       #
##############################

cmake_minimum_required (VERSION 3.13)
project (CarGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

macro(cg_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# On GCC, set C++ stdlib to be linked statically
macro(cg_gcc_static_link target)
	# nothing
endmacro()

###################################
# try to detect compiler and arch #
###################################

message("CG: compiler is ${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	if(WIN32)
		set(CG_COMPILER "mingw")
		set(CG_ARCH_PREFIX_A "win")
	elseif(UNIX)
		set(CG_COMPILER "unix")
		set(CG_ARCH_PREFIX_A "unix")
	endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CG_COMPILER "msvc")
	set(CG_ARCH_PREFIX_A "win")
else()
	set(CG_COMPILER "invalid")
	set(CG_ARCH_PREFIX_A "_")
	set(CG_ARCH_PREFIX_B "_")
endif()

# detect arch
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)")
	set(CG_ARCH_PREFIX_B "x86")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(amd64)|(AMD64)")
	set(CG_ARCH_PREFIX_B "x64")
else()
	set(CG_ARCH_PREFIX_B "_")
endif()

# generate arch prefix
set(CG_ARCH_PREFIX "${CG_ARCH_PREFIX_A}_${CG_ARCH_PREFIX_B}")
if(NOT DEFINED CG_EXTLIB_PATH)
	set(CG_EXTLIB_PATH "extlibs/libs-${CG_COMPILER}/${CG_ARCH_PREFIX_B}")
endif()

# todo: generate CG_VER_SIG from CG_VER_STR
# ...

# setup options
cg_set_option(CG_COMPILE_LAUNCHER TRUE BOOL "Compile CG Launcher")
cg_set_option(CG_COMPILE_DEBUG TRUE BOOL "Compile debug version of CG")
cg_set_option(CG_ARCH_PREFIX "invalid" STRING "Folder name to use for executables and in updater file")

# set cg version
cg_set_option(CG_VER_STR "invalid" STRING "Version string (displayed in-game)")
cg_set_option(CG_VER_SIG "invalid" STRING "Version signature (used in file names)")
#cg_set_option(SFML_ROOT "invalid" PATH "SFML 2.4.2 root")

# add version to be used in game
message("CG: version is '${CG_VER_STR}'")
configure_file ("${PROJECT_SOURCE_DIR}/src/version.cpp.in" "${PROJECT_SOURCE_DIR}/src/version.cpp")

set(VERSION_SOURCES
	"src/version.hpp"
	"src/version.cpp"
)

# include directories
include_directories("${PROJECT_BINARY_DIR}")

set(EXEC "cargame-${CG_VER_SIG}")

# add launcher exec if required
if(CG_COMPILE_LAUNCHER)
	if(CG_ARCH_PREFIX_A STREQUAL "win")
		message("CG: compiling launcher")
		#message("CG: IMPORTANT WARNING: Launcher is not compiling yet, it will result in errors!")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cglauncher)
		file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "cglauncher/*.cpp" "cglauncher/libs/ziputils/*.c" "cglauncher/*.rc")
		add_executable(cglauncher "${SOURCES}")
		target_link_libraries(cglauncher "winhttp")
		target_include_directories(cglauncher PUBLIC "${CMAKE_SOURCE_DIR}/cglauncher/libs/*")
	else()
		message("CG: launcher not supported on non-Windows platforms")
	endif()
endif()

##########################
# add cg.dll/cg.so (API) #
##########################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC}/${CG_ARCH_PREFIX})

file(GLOB_RECURSE API_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} ${VERSION_SOURCES} "src/cargame/*.rc" "src/cargame/*.cpp" "src/cargame/*.cpp" "src/cargame/*.inl" "src/cargame/*.hpp" "cgui_fork/src/*.cpp" "cgui_fork/include/CG/*.hpp" "src/cargame/*.h" "cgui_fork/include/CG/*.h" "cgui_fork/include/CG/*.hpp")
set(CMAKE_BUILD_TYPE "Release")
#message("CG: API sources are ${API_SOURCES}")
add_library(cgapi SHARED "${API_SOURCES}")
set_target_properties(cgapi PROPERTIES LINK_FLAGS "-fPIC")
#set_target_properties(cgapi PROPERTIES COMPILE_FLAGS)

if(CG_ARCH_PREFIX_A STREQUAL "unix")
	target_link_libraries(cgapi PUBLIC dl)
endif()

if(CG_COMPILE_DEBUG)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

# copy resources to binary folder
message("CG: copying resource dir to ${EXEC}")
file(COPY data/ DESTINATION ${EXEC})

#####################
# find and add SFML #
#####################

cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
message("CG: SFML root is '${SFML_ROOT}'")
find_package(SFML 2.5 COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
	message("CG: adding SFML as dependency")

	#target_include_directories(cgapi PUBLIC "${SFML_INCLUDE_DIR}")
	target_link_libraries(cgapi PUBLIC sfml-graphics sfml-audio)
endif()

# add sfml extlibs
message("CG: sfml extlib path is ${SFML_ROOT}/${CG_EXTLIB_PATH}")
#link_directories("${SFML_ROOT}/${CG_EXTLIB_PATH}")
link_directories("${CMAKE_BINARY_DIR}/sfml/${CG_EXTLIB_PATH}")

# add lib directory
link_directories(${CMAKE_BINARY_DIR}/lib)

# compile hmutil
message("CG: compiling and adding hmutil")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(GLOB_RECURSE HMUTIL_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/hmutil/include/HackerMan/Util/*.cpp")
add_library(hmutil "${HMUTIL_SOURCES}")
set_target_properties(hmutil PROPERTIES LINK_FLAGS "-fPIC")
set_target_properties(hmutil PROPERTIES COMPILE_FLAGS "-fPIC")
target_include_directories(hmutil PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")

#####################
# add cg.exe (ROOT) #
#####################

file(GLOB_RECURSE ROOT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} ${VERSION_SOURCES} "src/root/${CG_ARCH_PREFIX_A}/*.rc" "src/root/${CG_ARCH_PREFIX_A}/*.cpp" "src/root/${CG_ARCH_PREFIX_A}/*.hpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC}/${CG_ARCH_PREFIX})
#message("CG: ROOT sources for '${CG_ARCH_PREFIX_A}' are ${ROOT_SOURCES}")

#set(CMAKE_EXE_LINKER_FLAGS " -static")

add_executable(cg "${ROOT_SOURCES}")
set_target_properties(cg PROPERTIES LINK_FLAGS "-fPIC")
set_target_properties(cg PROPERTIES COMPILE_FLAGS "-fPIC")
cg_gcc_static_link(cg)

set(CG_MOD_DIR ${CMAKE_BINARY_DIR}/${EXEC}/${CG_ARCH_PREFIX}/mods)
message("CG: mod directory is ${CG_MOD_DIR}")

# Add mod to build system.
macro(cg_setup_mod name sources)
	message("CG: Adding MODULE: '${name}'")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CG_MOD_DIR}")
	add_library(${name} MODULE "${sources}")
	set_target_properties(${name} PROPERTIES LINK_FLAGS "-fPIC")
	set_target_properties(${name} PROPERTIES COMPILE_FLAGS "-fPIC")
	set_target_properties(${name} PROPERTIES PREFIX "")
	cg_link_common(${name})
	target_link_directories(${name} PUBLIC ${CMAKE_BINARY_DIR})
	target_include_directories(${name} PUBLIC "${CMAKE_SOURCE_DIR}/src/root/${CG_ARCH_PREFIX_A}")
	target_link_libraries(${name} PRIVATE cgapi)
endmacro()

# Add default includes to target.
macro(cg_link_common target)
	target_include_directories(${target} PUBLIC "${CMAKE_SOURCE_DIR}/src/")
	target_include_directories(${target} PUBLIC "${CMAKE_SOURCE_DIR}/cgui_fork/include")
	target_include_directories(${target} PUBLIC "${CMAKE_SOURCE_DIR}/hmutil/include")
	target_include_directories(${target} PUBLIC "${SFML_INCLUDE_DIR}")
endmacro()

# add cg and hmutil libraries to compiler cmd
message("CG: adding headers")
target_link_directories(cg PUBLIC ${CMAKE_BINARY_DIR})
target_include_directories(cg PUBLIC "${CMAKE_SOURCE_DIR}/src/root/${CG_ARCH_PREFIX_A}")
target_link_libraries(cg PUBLIC cgapi)
cg_link_common(cg)

# add libraries to API DLL
target_link_directories(cgapi PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(cgapi PRIVATE hmutil)
cg_link_common(cgapi)

# add default modules
add_subdirectory("src/core")

# set working directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC})
message("CG: successfully configured CG ${CG_VER_STR}")


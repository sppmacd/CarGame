cmake_minimum_required (VERSION 2.6)
project (CarGame)

# set cg version
set CG_VER_STR "invalid"
set CG_VER_SIG "invalid"

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

# add version to be used in game
configure_file (
	"${PROJECT_SOURCE_DIR}/version.cpp.in"
	"${PROJECT_SOURCE_DIR}/version.cpp"
)

# include directories
include_directories("${PROJECT_BINARY_DIR}")

set(EXEC "cargame-"+${CG_VER_SIG})

# add launcher exec if required
if(${CG_COMPILE_LAUNCHER})
    add_executable(cglauncher "cglauncher/*.cpp"
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXEC}/${CG_ARCH_PREFIX})
add_executable(cg "src/*.cpp" "cgui_fork/*.cpp")
add_executable(cgd "src/*.cpp" "cgui_fork/*.cpp")

# find and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_link_libraries(cg* ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

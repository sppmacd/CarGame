cmake_minimum_required (VERSION 2.6)
project (CarGame)

# set cg version
set CG_VER_STR "beta 0.2 v190901 [snapshot]"
set CG_VER_SIG "beta-0.2-v190901-snapshot"

configure_file (
	"${PROJECT_SOURCE_DIR}/version.cpp.in"
	"${PROJECT_SOURCE_DIR}/version.cpp"
)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(cg32 "src/*.cpp")
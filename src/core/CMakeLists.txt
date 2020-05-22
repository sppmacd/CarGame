##############################
# CARGAME CMake Build System #
#      Ver. 0.3-beta.1       #
##############################

# add core
file(GLOB_RECURSE CGCORE_MOD_SOURCES "*.rc" "*.cpp" "*.hpp" "*.h")
cg_setup_mod(cgcore "${CGCORE_MOD_SOURCES}")

# Compiling CG with CMake
 (todo: cmake command line)

- We assume you have a compiler
- Download CMake (minimum version: 2.8)
	- https://cmake.org/download/
- Download SFML (2.0 to 2.4.2, don't use 2.5 and newer!)
	- you can download source and compile it yourself using CMake.
	- if you are compiling SFML from sources, *the binary path must be equal to the source path* (!!!!)
	- if you are downloading SFML binaries, make sure they are matching your compiler version !!
	- https://en.sfml-dev.org/download.php
- Download the CG source code:
	- https://github.com/sppmacd/CarGame/releases
	- git clone https://github.com/sppmacd/CarGame.git (you must have Git installed and configured)
		- it's always newest version, it might be unstable.
	- https://github.com/sppmacd/CarGame/ > Clone or Download
- VERY IMPORTANT: Copy SFML dependencies from <SFML Path>/extlibs/libs-<your compiler>/* to <SFML Path>/lib/ !!!!
- VERY IMPORTANT (Windows only): Copy OpenAL binaries from <SFML Path>/extlibs/bin/<your arch (x86/x64)>/openal32.dll to <CG Source Path>/res/<CG_ARCH_PREFIX> (if it don't exist)
- Start CMake and set:
	- source dir to <CG Source Path> (e.g. C:/CG/sources)
	- binary dir to every path that is not in <CG Source Path> (e.g. C:/CG/build)
- Start configuring - click "Configure" button in CMake GUI
- If the compiler prompt pops up (todo)
- There should be an error. 
	- Set SFML_ROOT to your compiled SFML path
	- Set CG_ARCH_PREFIX to some prefix (e.g. win_x64,win_x86)
	- Set CG_VER_SIG and CG_VER_STR to some CG version:
		- Version format: <alpha/beta/release>-<major>.[minor>.[rev]<-v[snapshot date and number]-snapshot>
						e.g alpha-0.0.0, beta-0.1, beta-0.1.1, beta-0.2-v190803-snapshot
	- If you are using an IDE, set specific paths for IDE also
		- Code::Blocks: set CMAKE_CODEBLOCKS_EXECUTABLE to <C::B path>/codeblocks.exe
	- If you want compile launcher, check CG_COMPILE_LAUNCHER.
	- If you want to compile Debug also, check CG_COMPILE_DEBUG
		- There is a bug - it compiles Release also as debug !!
	- !!! Don't change CMAKE_BUILD_TYPE - it is already overwritten by CG config script.
- Start configuring again.
- After configuring, click Generate.
- Compile the project using IDE or script.
	- Code::Blocks - MinGW/Unix Makefiles - simply open the project and press Ctrl+F9.
	- MinGW Makefiles - open cmd in build dir and type ```mingw32-make all```.
	- Unix Makefiles - open terminal in build dir and type ```make all```.
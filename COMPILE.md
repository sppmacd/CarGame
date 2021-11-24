# Compiling CG with CMake

## Dependencies

* Git
* CMake (2.8+)
* Make
* GCC 7.2.0+

## Building CG on Unix-like systems

Download and install dependencies for your platform:

### Ubuntu

	sudo apt-get install -y libpthread-stubs0-dev libgl1-mesa-dev libx11-dev libxrandr-dev libfreetype6-dev libglew1.5-dev libjpeg8-dev libsndfile1-dev libopenal-dev libudev-dev libxcb-image0-dev libjpeg-dev libflac-dev git cmake ninja gcc

### Arch Linux/Manjaro

	sudo pacman -S git gcc cmake ninja sfml

### Build steps

	git clone https://github.com/sppmacd/CarGame
	cd CarGame
	mkdir build && cd build
	cmake .. -GNinja -DCG_VER_SIG=<version>
	ninja -j$(nproc)

### Run

	cd cargame-<version>
	./cglaunch.sh

(```chmod u+x cglaunch.sh``` may be needed)

## Building CG on Windows

WARNING: These are outdated and probably doesn't work

- We assume you have a compiler
- Download CMake (minimum version: 2.8)
	- https://cmake.org/download/
- Download SFML (2.4.2+)
	- you can download source and compile it yourself using CMake.
	- if you are compiling SFML from sources, *the binary path must be equal to the source path* (!!!!)
	- if you are downloading SFML binaries, make sure they are matching your compiler version !!
	- https://en.sfml-dev.org/download.php
- Download the CG source code:
	- https://github.com/sppmacd/CarGame/releases
	- git clone https://github.com/sppmacd/CarGame.git (you must have Git installed and configured)
		- it's always newest version, it might be unstable.
	- https://github.com/sppmacd/CarGame/ > Clone or Download
- Start CMake and set:
	- source dir to <CG Source Path> (e.g. C:/CG/sources)
	- binary dir to every path that is not in <CG Source Path> (e.g. C:/CG/build)
- Start configuring - click "Configure" button in CMake GUI
- If the compiler prompt pops up (todo)
- There should be an error. 
	- Set SFML_ROOT to your compiled SFML path
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

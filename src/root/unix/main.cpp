////////////////////////////
// Car Game ROOT Launcher //
// (cg.exe)               //
////////////////////////////

#include <UnixGameLoader.hpp>

// todo: document loading progress and mark load state for all virtual functions
int main(int argc, char* argv[])
{
    UnixGameLoader gameLoader;
    return gameLoader.main(argc, argv);
}

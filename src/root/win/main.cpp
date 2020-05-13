////////////////////////////
// Car Game ROOT Launcher //
// (cg.exe)               //
////////////////////////////

#include <WindowsGameLoader.hpp>

// todo: document loading progress and mark load state for all virtual functions
int main(int argc, char* argv[])
{
    WindowsGameLoader gameLoader;
    return gameLoader.main(argc, argv);
}

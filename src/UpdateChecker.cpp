#include <UpdateChecker.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "version.hpp"
#include <sstream>

string UpdateChecker::currentUpdateString = CG_VERSION_SIG;

bool UpdateChecker::checkUpdates()
{
    updateMade = false;
    /*
    sf::Ftp ftp;
    ftp.connect("...");
    ftp.download("cg/UPDATE.DATA", "tmp/UPDATE.DATA", Ftp::Ascii);
    ftp.disconnect();
    */
    Http http("http://raw.githubusercontent.com");
    Http::Request req("/sppmacd/CarGame/master/UPDATE.DATA", Http::Request::Get);
    Http::Response resp = http.sendRequest(req);
    std::string body = resp.getBody();
    if(resp.getStatus() != Http::Response::Ok)
    {
        cout << "UpdateChecker: Couldn't check updates: " << int(resp.getStatus()) << " " << body << endl;
        return false;
    }
    bool err = parseUpdateFile(body);
    if(err)
    {
        cout << "UpdateChecker: Couldn't parse update file" << endl;
        return false;
    }
    return currentUpdateString != newUpdateString;
}

void UpdateChecker::makeUpdate()
{
    cout << "UpdateChecker: Updating..." << endl;
    updateMade = true;
}

bool UpdateChecker::parseUpdateFile(string in)
{
    std::istringstream str(in);
    // commands:
    // ADDFILE <path> <url> - requests to download new file from <url> and place to <path>.
    // ADDDIR <path> - requests to add directory.
    // RMFILE <path> - requests to remove file from <path>.
    // RMDIR <path> - requests to remove directory with all files.
    // VERSION <ver> - sets CG version signature.
    return true;
}

string UpdateChecker::getVersion()
{
    return newUpdateString;
}

#include <SFML/Network.hpp>
#include <iostream>
#include "version.hpp"
#include <sstream>
#include "DebugLogger.hpp"
#include "UpdateChecker.hpp"

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
        DebugLogger::log("Couldn't check updates: " + std::to_string(resp.getStatus()), "UpdateChecker", "WARN");
        return false;
    }
    bool err = parseUpdateFile(body);
    if(err)
    {
        DebugLogger::log("Couldn't parse update file: (not implemented)", "UpdateChecker", "WARN");
        return false;
    }
    return currentUpdateString != newUpdateString;
}

void UpdateChecker::makeUpdate()
{
    DebugLogger::log("makeUpdate() - (not implemented)", "UpdateChecker", "WARN");

    // MOVED TO LAUNCHER

    updateMade = true;
}

bool UpdateChecker::parseUpdateFile(string in)
{
    std::istringstream str(in);

    // MOVED TO LAUNCHER

    return true;
}

string UpdateChecker::getVersion()
{
    return newUpdateString;
}

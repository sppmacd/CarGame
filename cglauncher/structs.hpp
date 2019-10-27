#pragma once

// structures

struct CGVersion
{
    int id;
    string verType;
    string verTag;
    string verSignature;
    vector<string> keptFiles;
    vector<string> keptFilesPatterns;
    string execName32;
    string execName64;
    string execNameDbg;

    int errorCode;
    int errorLine;
};
struct CGUpdateConfig
{
    vector<CGVersion> foundVersions;
    vector<string> keptFiles;
    vector<string> keptFilesPatterns;
    int currentVersionId;
    int launcherVer;

    string selectedVersion;
    string latestVersion;

    string execName32;
    string execName64;
    string execNameDbg;

    int errorCode;
    int errorLine;
};

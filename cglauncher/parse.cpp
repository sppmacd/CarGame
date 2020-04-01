#include "config.hpp"

CGVersion CG_parseUpdate(string in, CGVersion cfg)
{
    cfg.errorCode = 0;
    istringstream iss(in);
    size_t counter = 0;
    //cout << "size=" << in.size() << endl;
    while(!iss.eof())
    {
        //cout << "Line=" << counter+1 << endl;
        counter++;
        string in2;
        getline(iss, in2);
        istringstream issLine(in2);
        string cmd;

        if(!(issLine >> cmd))
        {
            //cout << "Couldn't read command for line " << counter << " and version " << cfg.verSignature << endl;
            //cfg.errorCode = CGERR_STREAM_ERROR;
            //cfg.errorLine = counter;
            break;
        }
        if(cmd.find("//") != string::npos || cmd[0] == '\0')
            continue;
        //cout << "Command = " << cmd << endl;

        if(cmd == "KEEPFILE")
        {
            string str;
            if(!(issLine >> str))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            //cout << "Keeping file " << str << endl;
            cfg.keptFiles.push_back(str);
        }
        else if(cmd == "KEEPFILEX")
        {
            string str;
            if(!(issLine >> str))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            cfg.keptFilesPatterns.push_back(CG_trim(str));
        }
        else if(cmd == "EXEC32")
        {
            if(!(issLine >> cfg.execName32))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
        }
        else if(cmd == "EXEC64")
        {
            if(!(issLine >> cfg.execName64))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
        }
        else if(cmd == "EXECDBG")
        {
            if(!(issLine >> cfg.execNameDbg))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
        }
        else
        {
            //ignore invalid entries
        }
    }
    return cfg;
}

CGUpdateConfig CG_parseUpdateConfig(string in)
{
    CGUpdateConfig cfg;
    cfg.errorCode = 0;
    cfg.currentVersionId = -1;
    istringstream iss(in);
    size_t counter = 0;
    while(!iss.eof())
    {
        counter++;
        string in2;
        if(!getline(iss, in2))
        {
            cfg.errorCode = CGERR_STREAM_ERROR;
            cfg.errorLine = counter;
            return cfg;
        }
        istringstream issLine(in2);
        string cmd;
        if(!(issLine >> cmd))
        {
            cfg.errorCode = CGERR_STREAM_ERROR;
            cfg.errorLine = counter;
            return cfg;
        }
        cmd = CG_trim(cmd);

        if(cmd.find("//") != string::npos || cmd.empty() || cmd[0] == '\0')
            continue;
        else if(cmd == "CURRENTVERSION")
        {
            if(!(issLine >> cfg.currentVersionId))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
        }
        else if(cmd == "AVAILABLEVERSION")
        {
            CGVersion ver;
            if(!(issLine >> ver.id >> ver.verType >> ver.verTag >> ver.verSignature))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }

            // trim all strings from spaces
            ver.verTag = CG_trim(ver.verTag);
            ver.verType = CG_trim(ver.verType);
            ver.verSignature = CG_trim(ver.verSignature);

            //cout << "Reading version " << ver.verSignature << endl;
            string c = CG_readUpdateConfig(ver.verSignature);
            ver = CG_parseUpdate(c, ver);

            //cout << "'" << c << "'" << endl;
            if(ver.errorCode == 0)
            {
                cout << "Read version successfully: " << ver.verSignature << endl;
                cfg.foundVersions.push_back(ver);
            }
            else
            {
                cout << "Version parsing error: " << ver.errorCode << " on line " << ver.errorLine << endl;
            }
        }
        else if(cmd == "KEEPFILE")
        {
            string str;
            if(!(issLine >> str))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            cfg.keptFiles.push_back(str);
            //cout << "Keeping file " << str << endl;
        }
        else if(cmd == "KEEPFILEX")
        {
            string str;
            if(!(issLine >> str))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            cfg.keptFilesPatterns.push_back(CG_trim(str));
            //cout << "Keeping file pattern " << str << endl;
        }
        else if(cmd == "EXEC32")
        {
            if(!(issLine >> cfg.execName32))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            //cout << "Setting x86 executable " << cfg.execName32 << endl;
        }
        else if(cmd == "EXEC64")
        {
            if(!(issLine >> cfg.execName64))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            //cout << "Setting x64 executable " << cfg.execName64 << endl;
        }
        else if(cmd == "EXECDBG")
        {
            if(!(issLine >> cfg.execNameDbg))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
            //cout << "Setting dbg executable " << cfg.execNameDbg << endl;
        }
        else if(cmd == "LAUNCHERVERSION")
        {
            if(!(issLine >> cfg.launcherVer))
            {
                cfg.errorCode = CGERR_SYNTAX_ERROR;
                cfg.errorLine = counter;
                return cfg;
            }
        }
        else
        {
            //ignore invalid entries
        }
    }
    return cfg;
}

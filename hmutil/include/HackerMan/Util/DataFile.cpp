#include "DataFile.hpp"

HMDataFile::HMDataFile(string name)
{
    m_fileName = name;
    open();
}

bool HMDataFile::open(string name)
{
    close();
    if(name == "")
        m_fileName = name;

    m_fileStream.open(name);
    return m_fileStream.good();
}

void HMDataFile::close()
{
    m_fileStream.close();
}

string HMDataFile::read()
{
    string data;

    if(m_fileStream.good())
    {
        while(!m_fileStream.eof())
        {
            data += readLine() + '\n';
        }
    }

    return data;
}

string HMDataFile::readLine()
{
    string data;

    if(m_fileStream.good())
        getline(m_fileStream, data);

    return data;
}

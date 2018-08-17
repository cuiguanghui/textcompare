#include "zini.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <iostream>

bool ZIni::writeString(std::string strSectName,
                       std::string strKeyName,
                       std::string strValue,
                       std::string strFileName)
{
    // open INI file
    std::ifstream fsIni(strFileName.c_str(),
        std::ios_base::in);

    // open INI.bak file
    std::ofstream fsIniBak((strFileName + ".zbk").c_str(),
        std::ios_base::out | std::ios_base::trunc);

    // file opened successfully
    if (fsIni && fsIniBak)
    {
        // search section
        std::string strLine = "";
        std::string strSection = std::string("[") + strSectName + "]";
        bool bWritten = false;
        while (getline(fsIni, strLine))
        {
            // remain the others
            fsIniBak << strLine << std::endl;

            // section found
            if (!bWritten && 0 == strSection.compare(trim(strLine)))
            {
                // search key
                while (getline(fsIni, strLine))
                {
                    std::string strTemp = trim(strLine);

                    // write value to the end of this section
                    if (strTemp.length() > 0 && '[' == strTemp[0])
                    {
                        fsIniBak << strKeyName << " = " << strValue
                            << std::endl;
                        fsIniBak << strLine << std::endl;
                        bWritten = true;
                        break;
                    }
                    // replace the origin value
                    if (strTemp.length() > strKeyName.length() &&
                        0 == strTemp.compare(0,
                        strKeyName.length(), strKeyName) &&
                        '=' == strTemp[strKeyName.length()])
                    {
                        fsIniBak << strKeyName << " = " << strValue
                            << std::endl;
                        bWritten = true;
                        break;
                    }
                    // remain the others
                    fsIniBak << strLine << std::endl;
                }

                // write value
                if (!bWritten)
                {
                    fsIniBak << strKeyName << " = " << strValue << std::endl;
                    bWritten = true;
                }
            }
        }

        // write value
        if (!bWritten)
        {
            fsIniBak << strSection << std::endl;
            fsIniBak << strKeyName << " = " << strValue << std::endl;
        }

        // close file stream
        fsIni.close();
        fsIniBak.close();
    }
    else
    {
        return false;
    }

    // open INI.zbk file
    std::ifstream fsIniBakNew((strFileName + ".zbk").c_str(),
        std::ios_base::in);

    // open INI file
    std::ofstream fsIniNew(strFileName.c_str(),
        std::ios_base::out | std::ios_base::trunc);

    // file opened successfully
    if (fsIniBakNew && fsIniNew)
    {
        std::string strLine = "";
        while (getline(fsIniBakNew, strLine))
        {
            fsIniNew << strLine << std::endl;
        }

        // close file stream
        fsIniBakNew.close();
        fsIniNew.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool ZIni::writeInt(std::string strSectName,
                    std::string strKeyName,
                    int iValue,
                    std::string strFileName)
{
    // convert int to string
    std::stringstream ssValue;
    ssValue << iValue;
    return writeString(strSectName, strKeyName, ssValue.str(), strFileName);
}

bool ZIni::writeDouble(std::string strSectName,
                       std::string strKeyName,
                       double fValue,
                       std::string strFileName)
{
    // convert double to string
    std::stringstream ssValue;
    ssValue << fValue;
    return writeString(strSectName, strKeyName, ssValue.str(), strFileName);
}

std::string ZIni::readString(std::string strSectName,
                             std::string strKeyName,
                             std::string strDefault,
                             std::string strFileName)
{
    // open INI file
    std::ifstream fsIni(strFileName.c_str(), std::ios_base::in);

    // file opened successfully
    if (fsIni)
    {
        // search section
        std::string strLine = "";
        std::string strSection = std::string("[") + strSectName + "]";
        while (getline(fsIni, strLine))
        {
            // section found
            if (0 == strSection.compare(trim(strLine)))
            {
                // search key
                while (getline(fsIni, strLine))
                {
                    std::string strTemp = trim(strLine);
                    // end of this section
                    if (strTemp.length() > 0 && '[' == strTemp[0])
                    {
                        return strDefault;
                    }
                    if (strTemp.length() > strKeyName.length() + 1 &&
                        0 == strTemp.compare(0,
                        strKeyName.length(), strKeyName) &&
                        '=' == strTemp[strKeyName.length()])
                    {
                        return (strTemp.substr(strKeyName.length() + 1));
                    }
                }
            }
        }
    }

    // read failed
    return strDefault;
}

int ZIni::readInt(std::string strSectName,
                  std::string strKeyName,
                  int iDefault,
                  std::string strFileName)
{
    // converting between int and string
    std::stringstream ssDefault;
    ssDefault << iDefault;
    return atoi(readString(strSectName, strKeyName,
        ssDefault.str(), strFileName).c_str());
}

double ZIni::readDouble(std::string strSectName,
                        std::string strKeyName,
                        double fDefault,
                        std::string strFileName)
{
    // converting between double and string
    std::stringstream ssDefault;
    ssDefault << fDefault;
    return atof(readString(strSectName, strKeyName,
        ssDefault.str(), strFileName).c_str());
}

//消除字串两段的空格，字串中间的保留
std::string ZIni::trim(std::string &strInput)
{
    std::string str(strInput);
    if(str.empty())
        return str;
    str = str.substr(str.find_first_not_of(' '),str.find_last_not_of(' ')+1);
    std::string::size_type pos_equations = str.find_first_of('=');
    if(pos_equations != std::string::npos)
    {
        if(pos_equations > 0 && str[pos_equations-1] == ' ')
            str.erase(pos_equations-1,1);

        pos_equations = str.find_first_of('=');
        if(pos_equations < str.size() && str[pos_equations+1] == ' ')
            str.erase(pos_equations+1,1);
    }
    return str;
}
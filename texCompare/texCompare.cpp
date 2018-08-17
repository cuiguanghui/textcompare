// texCompare.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "zini.h"

using namespace std;


void readTxt(string file, map<string,int> &mapData)
{
    ifstream infile;
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

    string s;
    while (getline(infile, s))
    {
        mapData.insert(make_pair(s, 1));
    }
    infile.close();             //关闭文件输入流 
}


int main()
{
    // 读取配置文件信息
    std::string strIniPath("config.ini");

    string strFirstPath = ZIni::readString("SYS", "firstFilePath", "", strIniPath.c_str());
    string strSecondPath = ZIni::readString("SYS", "secondFilePath", "", strIniPath.c_str());
    string strPushPath = ZIni::readString("SYS", "outputFilePath", "", strIniPath.c_str());
    int type = ZIni::readInt("SYS", "type", 0, strIniPath.c_str());



    map<string,int> mapOrigin;
    map<string,int> mapPush;

    readTxt(strFirstPath, mapOrigin);
    readTxt(strSecondPath, mapPush);

    ofstream f1(strPushPath); //打开文件用于写，若不存在则创建它
    if (!f1) {
        cout << "失败，创建文件失败："<<strPushPath.data() << endl;
        return 0;  //打开文件失败则退出运行
    } 

    //1:去除重复   2：比较文件，保留相同部分输出  3：比较文件，保留异同部分输出
    if (type == 1) {
        map<string, int>::iterator itOrigin = mapOrigin.begin();
        while (itOrigin != mapOrigin.end())
        {
            f1 << itOrigin->first << endl;
            itOrigin++;
        }
    }
    else if (2 == type) {
        map<string, int>::iterator itOrigin = mapOrigin.begin();
        while (itOrigin != mapOrigin.end())
        {
            auto itPush = mapPush.find(itOrigin->first);
            if (itPush != mapPush.end()) {
                f1 << itOrigin->first << endl;
            }
            itOrigin++;
        }
    }
    else if (3 == type) {
        map<string, int>::iterator itOrigin = mapOrigin.begin();
        while (itOrigin != mapOrigin.end())
        {
            auto itPush = mapPush.find(itOrigin->first);
            if (itPush == mapPush.end()) {
                f1 << itOrigin->first << endl;
            }
            itOrigin++;
        }

        map<string, int>::iterator itPush = mapPush.begin();
        while (itPush != mapPush.end())
        {
            auto itOrigin = mapOrigin.find(itPush->first);
            if (itOrigin == mapOrigin.end()) {
                f1 << itPush->first << endl;
            }
            itPush++;
        }
    }

    
    f1.close(); //关闭文件
    cout << "成功！！！" << endl;
    system("pause");
    return 0;
}


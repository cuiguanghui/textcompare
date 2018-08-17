// texCompare.cpp : �������̨Ӧ�ó������ڵ㡣
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
    infile.open(file.data());   //���ļ����������ļ��������� 
    assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

    string s;
    while (getline(infile, s))
    {
        mapData.insert(make_pair(s, 1));
    }
    infile.close();             //�ر��ļ������� 
}


int main()
{
    // ��ȡ�����ļ���Ϣ
    std::string strIniPath("config.ini");

    string strFirstPath = ZIni::readString("SYS", "firstFilePath", "", strIniPath.c_str());
    string strSecondPath = ZIni::readString("SYS", "secondFilePath", "", strIniPath.c_str());
    string strPushPath = ZIni::readString("SYS", "outputFilePath", "", strIniPath.c_str());
    int type = ZIni::readInt("SYS", "type", 0, strIniPath.c_str());



    map<string,int> mapOrigin;
    map<string,int> mapPush;

    readTxt(strFirstPath, mapOrigin);
    readTxt(strSecondPath, mapPush);

    ofstream f1(strPushPath); //���ļ�����д�����������򴴽���
    if (!f1) {
        cout << "ʧ�ܣ������ļ�ʧ�ܣ�"<<strPushPath.data() << endl;
        return 0;  //���ļ�ʧ�����˳�����
    } 

    //1:ȥ���ظ�   2���Ƚ��ļ���������ͬ�������  3���Ƚ��ļ���������ͬ�������
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

    
    f1.close(); //�ر��ļ�
    cout << "�ɹ�������" << endl;
    system("pause");
    return 0;
}


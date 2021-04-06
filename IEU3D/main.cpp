#include <iostream>
#include <fstream>
#include <vector>
#include "IEU3D.h"

///���������� ����������� � IDE CodeBlocks 20.03 ����� ����, ��� Visual Studio ������������ ������ ����������� ��������� ������� C++, �������� ��������

using namespace std;

ifstream file;

vector <Block> blocks;

///������ �������� �������� �� n(1-8) �����, ���������� � ����������� ���� (00 11 22 33 -> 0x33221100)
static unsigned long long readFileByLONG(int n)
{
    char *buff = new char[n];
    unsigned long long value = 0;
    file.read(buff,n);
    for(int i = n-1; i >= 0; i--)
    {
        value = value << 8;
        value = value + buff[i];
    }
    //delete buff;
    return value;
}
///������ �������� �������� �� 1 �����, ���������� ���������� ����;
static char readFileByCHAR()
{
    char* value = new char[0];;
    file.read(value,1);
    return value[0];
}

///������� ������, �������� �������� ������ ������ ��� ������� Data
int uncorrectCounter;

///��������� ����� ������ ������, ��������� ���������� Data � MetaData ���������
static int SearchBlock(unsigned int value)
{
    Block block;
    block.BlockType = value;
    value = 0;
    value = readFileByLONG(4);
    block.DataSize = value;
    value = 0;
    value = readFileByLONG(4);
    block.MetaDataSize = value;

    cout <<hex << block.BlockType;
    cout <<hex <<" DATASIZE: " <<  block.DataSize;
    cout <<hex <<" METADATASIZE: " <<  block.MetaDataSize << endl;

    // ��������� ���������� ������ Data � MetaData, ��� ���������� ��� ���������� ��������� ��������� ���� ���������, �� ��������� ������������ ������
    if (block.DataSize > 500 | block.MetaDataSize > 500)
    {
        cout << " it was UNCORRECT BLOCK "<< endl;
        uncorrectCounter++;
        return 123;
    }

    block.Data = new unsigned short[block.DataSize];
    for(int i = 0; i < block.DataSize; i++) // ���������� �������� ����� � ���� Data � ������ �������
    {
        block.Data[i] = readFileByCHAR();
    }

    block.MetaData = new unsigned short[block.MetaDataSize];
    for(int i = 0; i < block.MetaDataSize; i++) // ���������� �������� ����� � ���� MetaData � ������ �������
    {
        block.MetaData[i] = readFileByCHAR();
    }

    if(block.BlockType == 0x00443355) // �������� ������� Data, ���� ��� Header ����
        TryParseDataBlock(block.Data);

    blocks.push_back(block); //��������� ��������� ���� ������ � vector
    return 0;
}

int main()
{
    bool trigger;
    unsigned int value;

    file.open("C:/Users/Toculus/Documents/CodeBlocks Projects/IEU3D/bin/Debug/output.u3d");
    if(file.is_open()) cout << "isOpen"<< endl;
    //for( int i = 0; i < 30; i++)
    while(!file.eof())
    {
        value = 0;
        if(file.tellg()%4 == 0)
            value = readFileByLONG(4);
        else
            readFileByCHAR();

        for (int j = 0; j < sizeof(BlockTypes)/sizeof(int); j++)
        {
            if (value == BlockTypes[j])
            {
                SearchBlock(value);
                trigger = true;
                continue;
            }
        }
        /*if(value > 0x00000100 && value < 0x00FFFFFF)//����� ������ ���� New Object
        {
            SearchBlock(value);
            trigger = true;
        }*/
        if(!trigger)
            cout <<hex << value << endl;
        trigger = false;
    }

    cout << dec << "CorrectBlockCount: " <<blocks.size()<< endl;
    cout << dec << "UncorrectBlockCount: " <<uncorrectCounter;
    return 0;
}

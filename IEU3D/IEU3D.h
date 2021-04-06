/// Пока что это лишь кучка разных структур и функций, поскольку я обычно сначала делаю 100% рабочий вариант, потом оптимизирую и переписываю его части
/// Так же некоторые функции используют в своем теле ссылки на функции, доступные только из main.cpp, которые не способны работать в пространстве этой библиотеки
using namespace std;

///все тэги блоков спецификации, за исключением New Object Block
unsigned int BlockTypes[] =
{
    0x00443355,
    0xFEFEFF12,
    0xFEFEFF14,
    0xFEFEFF15,
    0xFEFEFF16,

    0xFEFEFF21,
    0xFEFEFF22,
    0xFEFEFF23,
    0xFEFEFF24,

    0xFEFEFF31,
    0xFEFEFF3B,
    0xFEFEFF3C,

    0xFEFEFF36,
    0xFEFEFF3E,

    0xFEFEFF37,
    0xFEFEFF3F,

    0xFEFEFF41,
    0xFEFEFF42,
    0xFEFEFF43,
    0xFEFEFF44,
    0xFEFEFF45,
    0xFEFEFF46,

    0xFEFEFF51,
    0xFEFEFF52,
    0xFEFEFF53,
    0xFEFEFF54,
    0xFEFEFF55,
    0xFEFEFF5C,
    0xFEFEFF56
};

///Структура всех блоков данных формата U3D
struct Block
{
    unsigned int BlockType;
    unsigned int DataSize;
    unsigned int MetaDataSize;
    unsigned short* Data;
    unsigned short* MetaData;
};

///Читает байты массива Data начиная с startPos заканчивая endPos, возвращает в развернутом виде (00 11 22 33 -> 0x33221100)
static long long readData(unsigned short* SearchBlock,int startPos,int endPos)
{
    //char *buff = new char[n];
    unsigned long long value = 0;

    for(int i = endPos; i >= startPos; i--)
    {
        value = value << 8;
        value = value + SearchBlock[i];
    }
    return value;
}

/// Парсит Data блок как Header
void TryParseDataBlock(unsigned short* Data)
{
    //HEADER
    unsigned short      MajVer;
    unsigned short      MinVer;
    unsigned int        ProfInd;
    unsigned int        DeclSize;
    unsigned long long  FileSize;
    unsigned int        CharEnc;
    double              UnitScalFactor;

    MajVer = readData(Data,0,1);
    MinVer = readData(Data,2,3);
    ProfInd = readData(Data,4,7);
    DeclSize = readData(Data,8,11);
    FileSize = readData(Data,12,19);
    CharEnc = readData(Data,20,23);
    UnitScalFactor = readData(Data,24,31); // выходит за границы массива Data блока Header, выдает случайное значение каждый запуск

    cout << hex << "Name    "           << "Hex  "          << " = Dec" << endl;
    cout << hex << "MajVer  "           << MajVer           << " = "    << dec << MajVer            << endl;
    cout << hex << "MinVer  "           <<MinVer            << " = "    << dec << MinVer            << endl;
    cout << hex << "ProfInd  "          << ProfInd          << " = "    << dec << ProfInd           << endl;
    cout << hex << "DeclSize  "         << DeclSize         << " = "    << dec << DeclSize          << endl;
    cout << hex << "FileSize  "         << FileSize         << " = "    << dec << FileSize          << endl;
    cout << hex << "CharEnc  "          << CharEnc          << " = "    << dec << CharEnc           << endl;
    cout << hex << "UnitScalFactor  "   << UnitScalFactor   << " = "    << dec << UnitScalFactor    << endl;
}


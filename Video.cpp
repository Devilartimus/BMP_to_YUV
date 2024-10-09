#include "Video.h"

#include <iostream>
#include <fstream>

Video::Video (const string& fileName)
{
    loadYUV(fileName);
}

int Video::getWidth() const
{
    return _WIDTH;
}

int Video::getHeight() const
{
    return _HEIGHT;
}

const vector<unsigned char>& Video::getYUVData() const
{
    return _YUV_DATA;
}

/**
 * @brief Video::loadYUV
 * @param fileName - Path and fileName of input YYUV Video
 * Reads YUV parametrs
 */
void Video::loadYUV(const string& fileName)
{
    ifstream file(fileName);

    if (!file)
    {
        cerr << "Error while opening YUV input file" << endl;
        return;
    }

    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);

    _HEIGHT = 288;
    _WIDTH =  352;

    _YUV_DATA.resize(fileSize);

    file.read(reinterpret_cast<char*>(_YUV_DATA.data()), _YUV_DATA.size());
    file.close();
}

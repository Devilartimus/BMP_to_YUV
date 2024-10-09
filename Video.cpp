#include "Video.h"

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
    ifstream file(fileName, ios::binary);

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

    int frameSize = _WIDTH * _HEIGHT * 3 / 2;

    if (fileSize % frameSize != 0)
    {
        cerr << "Error: File size is not a multiple of the frame size. Check video dimensions." << endl;
        return;
    }

    _YUV_DATA.resize(fileSize);

     file.read(reinterpret_cast<char*>(_YUV_DATA.data()), _YUV_DATA.size());

    if (file.gcount() != _YUV_DATA.size())
    {
        cerr << "Error: Incomplete YUV data read." << endl;
        return;
    }

    file.close();
}

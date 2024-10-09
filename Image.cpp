#include "Image.h"

#include <iostream>
#include <fstream>

using namespace std;

Image::Image(const string& fileName)
{
    loadBMP(fileName);
    RGBtoYUV();
}

/**
 * @brief Image::loadBMP
 * @param fileName - Name and Path to input BMP file
 * Method to load and read BMP data from input file
 */
void Image::loadBMP(const string& fileName)
{
    ifstream file(fileName);

    if (!file)
    {
        cerr << "Error while opening BMPP input file" <<endl;
        return;
    }

    unsigned char header[54];

    file.read(reinterpret_cast<char*>(header), sizeof(header));

    if(file.gcount() != sizeof(header))
    {
        cerr << "Error: Incorrect BMP header read" << endl;
        return;
    }

    _WIDTH = *(int*)&header[18];
    _HEIGHT = *(int*)&header[22];

    if (_WIDTH <= 0 || _HEIGHT <= 0)
    {
        cerr << "Error: Invalid BMP image dimensions." << endl;
        return;
    }

    _RGB_DATA.resize(_WIDTH * _HEIGHT * 3);

    file.read(reinterpret_cast<char*>(_RGB_DATA.data()), _RGB_DATA.size());

    if (file.gcount() != _RGB_DATA.size())
    {
        cerr << "Error: Incomplete RGB data read from BMP." << endl;
        return;
    }

    file.close();
}

int Image::getHeight() const
{
    return _HEIGHT;
}

int Image::getWidth() const
{
    return _WIDTH;
}

const std::vector<unsigned char>& Image::getYUVData() const
{
    return _YUV_DATA;
}

const std::vector<unsigned char>& Image::getRGBData() const
{
    return _RGB_DATA;
}

void Image::RGBtoYUV()
{
    _YUV_DATA.resize(_WIDTH * _HEIGHT * (3 / 2));

    int uvWidth = _WIDTH / 2;
    int uvHeight = _HEIGHT / 2;

    for (int i = 0; i < _RGB_DATA.size() / 3; i++)
    {
        int r = _RGB_DATA[i * 3];
        int g = _RGB_DATA[i * 3 + 1];
        int b = _RGB_DATA[i * 3 + 2];

        int y = 0.299 * r + 0.587 * g + 0.114 * b ;
        y = (y < 0) ? 0 : (y > 255) ? 255 : y;

        _YUV_DATA[i] = y;
    }

    for (int i = 0; i < uvHeight; i++)
    {
        for (int j = 0; j < uvWidth; j++)
        {
            int yIndex = (i * 2 * _WIDTH + j * 2);
            int uIndex = (_WIDTH * _HEIGHT) + (i * uvWidth + j);
            int vIndex = (_WIDTH * _HEIGHT) + (uvWidth * uvHeight) + (i * uvWidth + j);

            int sumR = 0, sumG = 0, sumB = 0;

            for (int y = 0; y < 2; y++)
            {
                for (int x = 0; x < 2; x++)
                {
                    int pixelIndex = (yIndex + y * _WIDTH + x) * 3;
                    int r = _RGB_DATA[pixelIndex];
                    int b = _RGB_DATA[pixelIndex + 2];

                    sumR += r;
                    sumB += b;
                }
            }

            int avgR = sumR / 4;
            int avgB = sumB / 4;

            int avgY = _YUV_DATA[yIndex];

            int u = (0.492 * (avgB - avgY)) + 128;
            u = (u < 0) ? 0 : (u > 255) ? 255 : u;

            int v = (0.887 * (avgR - avgY)) + 128;
            v = (v < 0) ? 0 : (v > 255) ? 255 : v;

            _YUV_DATA[uIndex] = u;
            _YUV_DATA[vIndex] = v;
        }
    }

}

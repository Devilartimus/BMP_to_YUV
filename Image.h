#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Image
{
public:

    Image(const string& fileName);

    int getWidth() const;
    int getHeight() const;
    const vector<unsigned char>& getYUVData() const;
    const vector<unsigned char>& getRGBData() const;

private:

    void loadBMP(const string& fileName);
    void RGBtoYUV();

    vector<unsigned char> _RGB_DATA;
    vector<unsigned char> _YUV_DATA;
    int _WIDTH;
    int _HEIGHT;
};

#endif // IMAGE_H

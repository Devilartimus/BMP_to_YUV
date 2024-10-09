#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

using namespace std;

class Image
{
public:

    Image(const string& fileName);

    const vector<unsigned char>& getYUVData() const;
    const vector<unsigned char>& getRGBData() const;
    int getWidth() const;
    int getHeight() const;

private:

    vector<unsigned char> _RGB_DATA;
    vector<unsigned char> _YUV_DATA;
    int _WIDTH;
    int _HEIGHT;

    void loadBMP(const string& fileName);
    void RGBtoYUV();

};

#endif // IMAGE_H

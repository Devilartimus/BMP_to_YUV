#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

class Image
{
public:

    Image(const std::string& fileName);

    const std::vector<unsigned char>& getRGBData() const;
    int getWidth() const;
    int getHeight() const;

private:

    std::vector<unsigned char> _RGB_DATA;

    int _WIDTH;
    int _HEIGHT;

    void loadBMP(const std::string& fileName);

};

#endif // IMAGE_H

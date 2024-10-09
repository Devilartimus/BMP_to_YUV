#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>

class Video
{
public:

    Video(const std::string& fileName);
    void overlayImage(const std::vector<unsigned char>& imageYUV, int imageWidth, int imageHeight, int x, int y);
    void save(const std::string& outputFileName);

private:

    std::vector<unsigned char> _YUV_DATA;
    int _WIDTH;
    int _HEIGHT;

    void loadYUV(const std::string& fileName);
};

#endif // VIDEO_H

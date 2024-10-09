#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>

using namespace std;

class Video
{
public:

    Video(const string& fileName);

    int getWidth() const;
    int getHeight() const;

    const std::vector<unsigned char>& getYUVData() const;

private:

    vector<unsigned char> _YUV_DATA;
    int _WIDTH;
    int _HEIGHT;

    void loadYUV(const string& fileName);
};

#endif // VIDEO_H

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Video.h"
#include "Image.h"

#include <string>

using namespace std;

class Processor
{
public:

    Processor(const string& VideoPath, const string& ImagePath);

    void overlayImage(const vector<unsigned char>& imageYUV, int imageWidth, int imageHeight, int x, int y);
    void process(const string& outputFileName);
    void save(const string& outputFileName);


private:

    Video video;
    Image image;
};

#endif // PROCESSOR_H

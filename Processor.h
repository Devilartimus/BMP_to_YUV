#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Video.h"
#include "Image.h"

#include <string>

class Processor
{
public:

    Processor(const string& VideoPath, const string& ImagePath);
    void process(const string& outputFileName);
    void overlayImage ();
    void save (const string& outputFileName);

private:
    Video video;
    Image image;
};

#endif // PROCESSOR_H

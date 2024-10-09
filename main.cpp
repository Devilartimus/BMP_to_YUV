#include <iostream>

#include "Processor.h"

using namespace std;

int main()
{
    string videoName = "C:/Users/user/Desktop/Test/BMP_to_YUV/input.yuv";
    string imageName = "C:/Users/user/Desktop/Test/BMP_to_YUV/BMP_352_288.bmp";
    string outputName = "C:/Users/user/Desktop/Test/BMP_to_YUV/output.yuv";

    Processor processor(videoName, imageName);
    processor.process(outputName);

    cout << "Complete!" << endl;

    return 0;
}

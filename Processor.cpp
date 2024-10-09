#include "Processor.h"

Processor::Processor (const string& videoName, const string& imageName) : video(videoName), image(imageName) {}

/**
 * @brief Processor::process
 * @param outputFileName - Path and FileName to comlite video
 * Func to call Overlay and Save methods
 */
void Processor::process(const string& outputFileName)
{

    overlayImage();

    save(outputFileName);
}

void Processor::overlayImage ()
{
    int imageWidth = image.getWidth();
    int videoWidth = video.getWidth();
    int videoHeight = video.getHeight();

    const vector<unsigned char>& imageData = image.getYUVData();
    const vector<unsigned char>& videoData = video.getYUVData();

    vector<unsigned char>& videoChangeableData = const_cast<vector<unsigned char>&>(videoData);

    int frameSize = videoWidth * videoHeight;

    for (size_t frameIndex = 0; frameIndex < videoChangeableData.size() / frameSize; frameIndex++)
    {
        for (int i = 0; i < videoHeight; i++)
        {
            for (int j  = 0; j < videoWidth; j++)
            {
                int videoIndex = frameIndex * frameSize + i * videoWidth + j;
                int imageIndex = i * imageWidth + j;

                if (videoIndex < videoChangeableData.size() && imageIndex < imageData.size())
                {
                    videoChangeableData[videoIndex] = imageData[imageIndex];
                }
            }
        }
    }
}

void Processor::save(const string& outputFileName)
{
    ofstream outputFile(outputFileName, ios::binary);

    if(!outputFile)
    {
        cerr << "Error while opening output file" << endl;
        return;
    }

    outputFile.write(reinterpret_cast<char*>(const_cast<unsigned char*>(video.getYUVData().data())), video.getYUVData().size());
    outputFile.close();
}

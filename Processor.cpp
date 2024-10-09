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

void Processor::overlayImage()
{
    int imageWidth = image.getWidth();
    int imageHeight = image.getHeight();
    int videoWidth = video.getWidth();
    int videoHeight = video.getHeight();

    const vector<unsigned char>& imageData = image.getYUVData();
    vector<unsigned char>& videoChangeableData = const_cast<vector<unsigned char>&>(video.getYUVData());

    int frameSize = videoWidth * videoHeight;
    int chromaSize = frameSize / 4;

    size_t numberOfFrames = videoChangeableData.size() / (frameSize * 3 / 2);

    int startX = (videoWidth - imageWidth) / 2;
    int startY = (videoHeight - imageHeight) / 2;

    for (size_t frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
    {
        int yOffset = frameIndex * frameSize * 3 / 2;
        int uOffset = yOffset + frameSize;
        int vOffset = uOffset + chromaSize;

        for (int i = 0; i < imageHeight; i++)
        {
            int videoYPos = startY + i;
            if (videoYPos < 0 || videoYPos >= videoHeight) continue;

            for (int j = 0; j < imageWidth; j++)
            {
                int videoXPos = startX + j;
                if (videoXPos < 0 || videoXPos >= videoWidth) continue;

                int imageIndex = i * imageWidth + j;
                int videoYIndex = yOffset + (videoYPos * videoWidth + videoXPos);

                int videoChromaXPos = videoXPos / 2;
                int videoChromaYPos = videoYPos / 2;

                int videoUIndex = uOffset + (videoChromaYPos * (videoWidth / 2) + videoChromaXPos);
                int videoVIndex = vOffset + (videoChromaYPos * (videoWidth / 2) + videoChromaXPos);

                if (videoYIndex < videoChangeableData.size() && imageIndex < imageData.size())
                {
                    videoChangeableData[videoYIndex] = imageData[imageIndex];
                }

                if (videoUIndex < videoChangeableData.size() && videoVIndex < videoChangeableData.size())
                {
                    int imageUIndex = frameSize + (i / 2) * (imageWidth / 2) + (j / 2);
                    int imageVIndex = frameSize + chromaSize + (i / 2) * (imageWidth / 2) + (j / 2);

                    videoChangeableData[videoUIndex] = imageData[imageUIndex];
                    videoChangeableData[videoVIndex] = imageData[imageVIndex];
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

    const vector<unsigned char>& videoData = video.getYUVData();
    int videoWidth = video.getWidth();
    int videoHeight = video.getHeight();

    int frameSize = videoWidth * videoHeight;
    int chromaSize = frameSize / 4;

    size_t numberOfFrames = videoData.size() / (frameSize * 3 / 2);
    if (videoData.size() % (frameSize * 3 / 2) != 0)
    {
        cerr << "Error: YUV data size is not a multiple of the frame size." << endl;
        return;
    }

    for (size_t frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
    {
        int yOffset = frameIndex * frameSize * 3 / 2;
        int uOffset = yOffset + frameSize;
        int vOffset = uOffset + chromaSize;

        outputFile.write(reinterpret_cast<const char*>(&videoData[yOffset]), frameSize);
        if (!outputFile)
        {
            cerr << "Error while writing Y channel to output file." << endl;
            return;
        }

        outputFile.write(reinterpret_cast<const char*>(&videoData[uOffset]), chromaSize);
        if (!outputFile)
        {
            cerr << "Error while writing U channel to output file." << endl;
            return;
        }

        outputFile.write(reinterpret_cast<const char*>(&videoData[vOffset]), chromaSize);
        if (!outputFile)
        {
            cerr << "Error while writing V channel to output file." << endl;
            return;
        }
    }

    outputFile.close();
    if (outputFile.fail())
    {
        cerr << "Error while closing output file." << endl;
        return;
    }
}

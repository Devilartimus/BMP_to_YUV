#include "Image.h"

Image::Image(const string& fileName)
{
    loadBMP(fileName);
    RGBtoYUV();
}

/**
 * @brief Image::loadBMP
 * @param fileName - Name and Path to input BMP file
 * Method to load and read BMP data from input file
 */
void Image::loadBMP(const string& fileName)
{
    ifstream file(fileName, ios::binary | ios::in);

    if (!file)
    {
        cerr << "Error while opening BMP input file" << endl;
        return;
    }

    unsigned char header[54];

    file.read(reinterpret_cast<char*>(header), sizeof(header));

    if(file.gcount() != sizeof(header))
    {
        cerr << "Error: Incorrect BMP header read" << endl;
        return;
    }

    memcpy(&_WIDTH, &header[18], sizeof(_WIDTH));
    memcpy(&_HEIGHT, &header[22], sizeof(_HEIGHT));

    if (_WIDTH <= 0 || _HEIGHT <= 0)
    {
        cerr << "Error: Invalid BMP image dimensions." << endl;
        return;
    }

    uint32_t offsetData;
    memcpy(&offsetData, &header[10], sizeof(offsetData));

    file.seekg(offsetData, ios::beg);

    int rowSize = (_WIDTH * 3 + 3) & (~3);
    _RGB_DATA.resize(_WIDTH * _HEIGHT * 3);
    vector<unsigned char> rowBuffer(rowSize);

    for (int i = 0; i < _HEIGHT; ++i)
    {
        file.read(reinterpret_cast<char*>(rowBuffer.data()), rowSize);
        if (file.gcount() != rowSize)
        {
            cerr << "Error: not enough data read for row " << i << endl;
            return;
        }

        for (int j = 0; j < _WIDTH; ++j)
        {
            int sourceIndex = j * 3;
            int targetIndex = ((_HEIGHT - 1 - i) * _WIDTH + j) * 3;
            _RGB_DATA[targetIndex] = rowBuffer[sourceIndex];
            _RGB_DATA[targetIndex + 1] = rowBuffer[sourceIndex + 1];
            _RGB_DATA[targetIndex + 2] = rowBuffer[sourceIndex + 2];
        }
    }

    file.close();
}

int Image::getHeight() const
{
    return _HEIGHT;
}

int Image::getWidth() const
{
    return _WIDTH;
}

const vector<unsigned char>& Image::getYUVData() const
{
    return _YUV_DATA;
}

const vector<unsigned char>& Image::getRGBData() const
{
    return _RGB_DATA;
}

/**
 * @brief Image::processRows - processing rows of image in different threads
 * @param startRow
 * @param endRow
 * now it uses AVX2 optimization
 */
void Image::processRows(int startRow, int endRow)
{
    cout << "Processing rows from " << startRow << " to " << endRow << endl;

    int uvWidth = _WIDTH / 2;
    int uvHeight = _HEIGHT / 2;

    for (int i = startRow; i < endRow; ++i)
    {
        for (int j = 0; j < _WIDTH; ++j)
        {
            int index = (i * _WIDTH + j);
            int r = _RGB_DATA[index * 3 + 2];
            int g = _RGB_DATA[index * 3 + 1];
            int b = _RGB_DATA[index * 3];

            int y = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
            y = (y < 0) ? 0 : (y > 255) ? 255 : y;
            _YUV_DATA[index] = y;

            if (i % 2 == 0 && j % 2 == 0)
            {
                int uIndex = (_WIDTH * _HEIGHT) + (i / 2 * uvWidth + j / 2);
                int vIndex = (_WIDTH * _HEIGHT) + (uvWidth * uvHeight) + (i / 2 * uvWidth + j / 2);

                int sumR = 0, sumG = 0, sumB = 0;

                for (int y = 0; y < 2; ++y)
                {
                    for (int x = 0; x < 2; ++x)
                    {
                        int pixelIndex = ((i + y) * _WIDTH + (j + x)) * 3;

                        if (pixelIndex >= _RGB_DATA.size())
                        {
                            continue;
                        }

                        int b = _RGB_DATA[pixelIndex];
                        int g = _RGB_DATA[pixelIndex + 1];
                        int r = _RGB_DATA[pixelIndex + 2];

                        sumR += r;
                        sumG += g;
                        sumB += b;
                    }
                }

                int avgR = sumR / 4;
                int avgG = sumG / 4;
                int avgB = sumB / 4;

                int u = static_cast<int>(-0.14713 * avgR - 0.28886 * avgG + 0.436 * avgB + 128);
                u = (u < 0) ? 0 : (u > 255) ? 255 : u;

                int v = static_cast<int>(0.615 * avgR - 0.51499 * avgG - 0.10001 * avgB + 128);
                v = (v < 0) ? 0 : (v > 255) ? 255 : v;

                _YUV_DATA[uIndex] = u;
                _YUV_DATA[vIndex] = v;
            }
        }
    }
}

/**
 * @brief Image::RGBtoYUV
 * converting MN RGB to YUV according to YUV documentation
 */
void Image::RGBtoYUV()
{
    _YUV_DATA.resize(_WIDTH * _HEIGHT * 1.5);

    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    cout << "Number of threads: " << numThreads << endl;

    int rowsPerThread = _HEIGHT / numThreads;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i)
    {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? _HEIGHT : startRow + rowsPerThread;

        threads.emplace_back(&Image::processRows, this, startRow, endRow);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
}


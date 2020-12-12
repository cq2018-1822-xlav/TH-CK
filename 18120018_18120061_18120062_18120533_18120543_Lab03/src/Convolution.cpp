﻿#include "Convolution.h"

int Convolution::separateColorImg(const cv::Mat& sourceImage, cv::Mat& destinationImage, int channel)
{
    // Kiểm trả ảnh đầu vào
    if (!sourceImage.data) {
        // Chuyển đổi ảnh thất bại: in ra màn hình console tin nhắn
        std::cout << "[EXCEPTION] Error with input image.\n";
        return 1; // Trả về 1
    }

    // Chiều rộng của ảnh source
    int width = sourceImage.cols;

    // Chiều cao của ảnh source
    int heigth = sourceImage.rows;

    // Số channels của ảnh source
    int sourceChannels = sourceImage.channels();

    // Khởi tạo ảnh destination là ảnh grayscale với thông số height, width
    destinationImage = cv::Mat(heigth, width, CV_8UC1, cv::Scalar(0));

    // Số channels của ảnh destination
    int destinationChannels = destinationImage.channels();

    // Widthstep của ảnh source
    size_t sourceWidthStep = sourceImage.step[0];

    // Widthstep của ảnh destination
    size_t destinationWidthStep = destinationImage.step[0];

    // Con trỏ quản lý vùng nhớ data ảnh source
    uchar* ptrSourceData = sourceImage.data;

    // Con trỏ quản lý vùng nhớ data ảnh destination
    uchar* ptrDestinationData = destinationImage.data;

    for (int y = 0; y < heigth; y++, ptrSourceData += sourceWidthStep, ptrDestinationData += destinationWidthStep) {
        uchar* ptrSourceRow = ptrSourceData;
        uchar* ptrDestinationRow = ptrDestinationData;

        for (int x = 0; x < width; x++, ptrSourceRow += sourceChannels, ptrDestinationRow += destinationChannels) {


            switch (channel) {
            case 0: {
                // Lấy giá trị kênh màu Blue của ảnh source
                uchar blue = ptrSourceRow[0];
                // Gán giá trị độ xám vừa tính được vào kênh vào của ảnh destination
                ptrDestinationRow[0] = blue;

                // std::cout << (int)blue << "\n";

                break;
            }

            case 1:
            {
                // Lấy giá trị kênh màu Blue của ảnh source
                uchar green = ptrSourceRow[1];
                // Gán giá trị độ xám vừa tính được vào kênh vào của ảnh destination
                ptrDestinationRow[0] = green;

                // std::cout << (int)green << "\n";

                break;
            }

            case 2:
            {
                // Lấy giá trị kênh màu Red của ảnh source
                uchar red = ptrSourceRow[2];
                ptrDestinationRow[0] = red;

                // std::cout << (int)red << "\n";

                break;
            }

            default:
                return 1;
            }
        }
    }
    return 0;
}

int Convolution::mergeColorImg(cv::Mat& sourceImage, cv::Mat& blueLayer, cv::Mat& greenLayer, cv::Mat& redLayer)
{
    // Kiểm trả ảnh đầu vào
    if (!blueLayer.data || !greenLayer.data || !redLayer.data) {
        // Chuyển đổi ảnh thất bại: in ra màn hình console tin nhắn
        std::cout << "[EXCEPTION] Error with input image.\n";
        return 1; // Trả về 1
    }

    if ((blueLayer.rows != greenLayer.rows) || (blueLayer.rows != redLayer.rows)) {
        std::cout << "[EXCEPTION] Error with input image.\n";
    }

    if ((blueLayer.cols != greenLayer.cols) || (blueLayer.cols  != redLayer.cols)) {
        std::cout << "[EXCEPTION] Error with input image.\n";
    }

    sourceImage = cv::Mat(blueLayer.rows, blueLayer.cols, CV_8UC3, cv::Scalar(0));

    // Chiều rộng của ảnh source
    int width = sourceImage.cols;

    // Chiều cao của ảnh source
    int heigth = sourceImage.rows;

    // Số channels của ảnh source
    int sourceChannels = sourceImage.channels();

    // Widthstep của ảnh source
    size_t sourceWidthStep = sourceImage.step[0];

    // Con trỏ quản lý vùng nhớ data ảnh source
    uchar* ptrSourceData = sourceImage.data;

    // Số channels của ảnh blue
    int blueLayerChannels = blueLayer.channels();

    // Widthstep của ảnh blue
    size_t blueLayerWidthStep = blueLayer.step[0];

    // Con trỏ quản lý vùng nhớ data ảnh blue
    uchar* ptrBlueLayerData = blueLayer.data;

    // Số channels của ảnh green
    int greenLayerChannels = greenLayer.channels();

    // Widthstep của ảnh green
    size_t greenLayerWidthStep = greenLayer.step[0];

    // Con trỏ quản lý vùng nhớ data ảnh green
    uchar* ptrGreenLayerData = greenLayer.data;

    // Số channels của ảnh green
    int redLayerChannels = redLayer.channels();

    // Widthstep của ảnh green
    size_t redLayerWidthStep = redLayer.step[0];

    // Con trỏ quản lý vùng nhớ data ảnh green
    uchar* ptrRedLayerData = redLayer.data;


    for (int y = 0; y < heigth; y++, ptrSourceData += sourceWidthStep, ptrBlueLayerData += blueLayerWidthStep,
            ptrGreenLayerData += greenLayerWidthStep,
                     ptrRedLayerData += redLayerWidthStep) {
        uchar* ptrSourceRow = ptrSourceData;
        uchar* ptrBlueLayerRow = ptrBlueLayerData;
        uchar* ptrGreenLayerRow = ptrGreenLayerData;
        uchar* ptrRedLayerRow = ptrRedLayerData;

        for (int x = 0; x < width; x++, ptrSourceRow += sourceChannels, ptrBlueLayerRow += blueLayerChannels
            , ptrGreenLayerRow += greenLayerChannels, ptrRedLayerRow += redLayerChannels) {
            uchar blue = ptrBlueLayerRow[0];
            uchar green = ptrGreenLayerRow[0];
            uchar red = ptrRedLayerRow[0];

            ptrSourceRow[0] = blue;
            ptrSourceRow[1] = green;
            ptrSourceRow[2] = red;
        }
    }

    return 0;
}

std::vector<float> Convolution::GetKernel()
{
    return this->_kernel;
}

void Convolution::SetKernel(std::vector<float> kernel, int kWidth, int kHeight)
{
    this->_kernel = kernel;
    this->_kernelWidth = kWidth;
    this->_kernelHeight = kHeight;
}

/*
Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
sourceImage: ảnh input
destinationImage: ảnh output
Hàm trả về
0: nếu tính thành công
1: nếu tính thất bại (không đọc được ảnh input,...)
*/
int Convolution::DoConvolution(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
    // Con trỏ quản lý vùng data Source Image
    uchar* ptrSourceData = sourceImage.data;
    // Con trỏ quản lý vùng data Destination Image
    uchar* ptrDestinationData = destinationImage.data;

    int dataSizeX = sourceImage.cols;
    int dataSizeY = sourceImage.rows;

    int i, j, m, n, x, y, t;
    uchar** inPtr, * outPtr, * ptr;

    int kernelSizeX = this->_kernelWidth;
    int kernelSizeY = this->_kernelHeight;

    int kCenterX, kCenterY;
    int rowEnd, colEnd;                             // ending indice for section divider
    float sum;                                      // temp accumulation buffer
    int k, kSize;
    

    // check validity of params
    if (!ptrSourceData || !ptrDestinationData || this->_kernel.empty())
    {
        if (!ptrSourceData) {
            std::cout << "[EXCEPTION] Error occurs with source data.\n";
        }

        if (!ptrDestinationData) {
            std::cout << "[EXCEPTION] Error occurs with destination data.\n";
        }

        if (this->_kernel.empty()) {
            std::cout << "[EXCEPTION] Error occurs with kernel data.\n";
        }

        return 1;
    }
    if (dataSizeX <= 0 || kernelSizeX <= 0) return 1;

    // find center position of kernel (half of kernel size)
    kCenterX = kernelSizeX >> 1;
    kCenterY = kernelSizeY >> 1;
    kSize = kernelSizeX * kernelSizeY;              // total kernel size

    // allocate memeory for multi-cursor
    inPtr = new unsigned char* [kSize];
    if (!inPtr) return false;                        // allocation error

    // set initial position of multi-cursor, NOTE: it is swapped instead of kernel
    ptr = ptrSourceData + ((long long)dataSizeX * kCenterY + kCenterX); // the first cursor is shifted (kCenterX, kCenterY)
    for (m = 0, t = 0; m < kernelSizeY; m = -~m)
    {
        for (n = 0; n < kernelSizeX; n = -~n, t = -~t)
        {
            inPtr[t] = ptr - n;
        }
        ptr -= dataSizeX;
    }

    // init working  pointers
    outPtr = ptrDestinationData;

    rowEnd = dataSizeY - kCenterY;                  // bottom row partition divider
    colEnd = dataSizeX - kCenterX;                  // right column partition divider

    // convolve rows from index=0 to index=kCenterY-1
    y = kCenterY;
    for (i = 0; i < kCenterY; i = -~i)
    {
        // partition #1 ***********************************
        x = kCenterX;
        for (j = 0; j < kCenterX; j = -~j)                // column from index=0 to index=kCenterX-1
        {
            sum = 0;
            t = 0;
            for (m = 0; m <= y; m = -~m)
            {
                for (n = 0; n <= x; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += (kernelSizeX - x - 1);         // jump to next row
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];    // move all cursors to next
        }

        // partition #2 ***********************************
        for (j = kCenterX; j < colEnd; j=-~j)            // column from index=kCenterX to index=(dataSizeX-kCenterX-1)
        {
            sum = 0;
            t = 0;
            for (m = 0; m <= y; m = -~m)
            {
                for (n = 0; n < kernelSizeX; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];    // move all cursors to next
        }

        // partition #3 ***********************************
        x = 1;
        for (j = colEnd; j < dataSizeX; j = -~j)           // column from index=(dataSizeX-kCenter) to index=(dataSizeX-1)
        {
            sum = 0;
            t = x;
            for (m = 0; m <= y; m = -~m)
            {
                for (n = x; n < kernelSizeX; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += x;                             // jump to next row
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];    // move all cursors to next
        }

        ++y;                                        // add one more row to convolve for next run
    }

    // convolve rows from index=kCenterY to index=(dataSizeY-kCenterY-1)
    for (i = kCenterY; i < rowEnd; i = -~i)               // number of rows
    {
        // partition #4 ***********************************
        x = kCenterX;
        for (j = 0; j < kCenterX; j = -~j)                // column from index=0 to index=kCenterX-1
        {
            sum = 0;
            t = 0;
            for (m = 0; m < kernelSizeY; m = -~m)
            {
                for (n = 0; n <= x; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += (kernelSizeX - x - 1);
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];    // move all cursors to next
        }

        // partition #5 ***********************************
        for (j = kCenterX; j < colEnd; j = -~j)          // column from index=kCenterX to index=(dataSizeX-kCenterX-1)
        {
            sum = 0;
            t = 0;
            for (m = 0; m < kernelSizeY; m = -~m)
            {
                for (n = 0; n < kernelSizeX; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++inPtr[t]; // in this partition, all cursors are used to convolve. moving cursors to next is safe here
                    ++t;
                }
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
        }

        // partition #6 ***********************************
        x = 1;
        for (j = colEnd; j < dataSizeX; j = -~j)           // column from index=(dataSizeX-kCenter) to index=(dataSizeX-1)
        {
            sum = 0;
            t = x;
            for (m = 0; m < kernelSizeY; m = -~m)
            {
                for (n = x; n < kernelSizeX; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += x;
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];    // move all cursors to next
        }
    }

    // convolve rows from index=(dataSizeY-kCenterY) to index=(dataSizeY-1)
    y = 1;
    for (i = rowEnd; i < dataSizeY; i = -~i)              // number of rows
    {
        // partition #7 ***********************************
        x = kCenterX;
        for (j = 0; j < kCenterX; j = -~j)                 // column from index=0 to index=kCenterX-1
        {
            sum = 0;
            t = kernelSizeX * y;

            for (m = y; m < kernelSizeY; m = -~m)
            {
                for (n = 0; n <= x; ++n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += (kernelSizeX - x - 1);
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];    // move all cursors to next
        }

        // partition #8 ***********************************
        for (j = kCenterX; j < colEnd; j = -~j)            // column from index=kCenterX to index=(dataSizeX-kCenterX-1)
        {
            sum = 0;
            t = kernelSizeX * y;
            for (m = y; m < kernelSizeY; m = -~m)
            {
                for (n = 0; n < kernelSizeX; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) ++inPtr[k];
        }

        // partition #9 ***********************************
        x = 1;
        for (j = colEnd; j < dataSizeX; j = -~j)           // column from index=(dataSizeX-kCenter) to index=(dataSizeX-1)
        {
            sum = 0;
            t = kernelSizeX * y + x;
            for (m = y; m < kernelSizeY; m = -~m)
            {
                for (n = x; n < kernelSizeX; n = -~n)
                {
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += x;
            }

            // store output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kSize; k = -~k) {
                ++(inPtr[k]); 
            }    // move all cursors to next
        }

        ++y;                                        // the starting row index is increased
    }

    return 0;
}


int Convolution::DoConvolutionColor(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
    destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3, cv::Scalar(0));
    cv::Mat blueLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, cv::Scalar(0));
    cv::Mat greenLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, cv::Scalar(0));
    cv::Mat redLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, cv::Scalar(0));

    this->separateColorImg(sourceImage, blueLayer, 0);
    this->separateColorImg(sourceImage, greenLayer, 1);
    this->separateColorImg(sourceImage, redLayer, 2);

    cv::Mat blueLayerConvole = blueLayer.clone();
    cv::Mat greenLayerConvole = greenLayer.clone();
    cv::Mat redLayerConvole = redLayer.clone();

    int res = this->DoConvolution(blueLayer, blueLayerConvole);
    //this->DoConvolution(greenLayer, greenLayerConvole);
    //this->DoConvolution(redLayer, redLayerConvole);

    std::cout << res << "\n";

    // this->mergeColorImg(destinationImage, blueLayerConvole, greenLayerConvole, redLayerConvole);
    return 0;
}

Convolution::Convolution()
{
    this->_kernel = std::vector<float>(0, 0);
    this->_kernelWidth = 0;
    this->_kernelHeight = 0;
}

Convolution::~Convolution()
{
    if (!this->_kernel.empty()) {
        this->_kernel.clear();
    }
}
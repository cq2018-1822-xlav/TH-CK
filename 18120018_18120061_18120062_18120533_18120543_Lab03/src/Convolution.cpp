#include "Convolution.h"

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
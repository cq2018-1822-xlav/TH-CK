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
    int i, j, m, n, x, y, t;
    uchar** inPtr, * outPtr, * ptr;
    int dataSizeX = sourceImage.cols;
    int dataSizeY = sourceImage.rows;
    destinationImage = sourceImage.clone();
    uchar* ptrSourceData = sourceImage.data;
    uchar* ptrDestinationData = destinationImage.data;
    int kernelCenterX, kernelCenterY;
    int rowEnd, colEnd;
    float sum;
    int k, kernelSize;

    if (!sourceImage.data || !destinationImage.data || this->_kernel.empty()) {
        return 1;
    }

    kernelCenterX = this->_kernelWidth >> 1;
    kernelCenterY = this->_kernelHeight >> 1;
    kernelSize = (this->_kernelWidth) * (this->_kernelHeight);
    inPtr = new uchar * [kernelSize];
    if (!inPtr) {
        return 1;
    }

    ptr = (ptrSourceData + dataSizeX * kernelCenterY + kernelCenterX);
    for (m = 0, t = 0; m < (this->_kernelHeight); ++m)
    {
        for (n = 0; n < (this->_kernelWidth); ++n, ++t)
        {
            inPtr[t] = ptr - n;
        }
        ptr -= dataSizeX;
    }

    outPtr = ptrDestinationData;
    rowEnd = dataSizeY - kernelCenterY;  // bottom row partition divider
    colEnd = dataSizeX - kernelCenterX;  // right column partition divider

     // convolve rows from index=0 to index=kCenterY-1
    y = kernelCenterY;
    for (i = 0; i < kernelCenterY; ++i)
    {
        // partition #1 ***********************************
        x = kernelCenterX;
        for (j = 0; j < kernelCenterX; ++j)  // column from index=0 to index=kCenterX-1
        {
            sum = 0;
            t = 0;
            for (m = 0; m <= y; ++m)
            {
                for (n = 0; n <= x; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
                t += (this->_kernelWidth - x - 1);  // jump to next row
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }

        // partition #2 ***********************************
        for (j = kernelCenterX; j < colEnd; ++j)  // column from index=kCenterX to index=(dataSizeX-kCenterX-1)
        {
            sum = 0;
            t = 0;
            for (m = 0; m <= y; ++m)
            {
                for (n = 0; n < this->_kernelWidth; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }

        // partition #3 ***********************************
        x = 1;
        for (j = colEnd; j < dataSizeX; ++j)  // column from index=(dataSizeX-kCenter) to index=(dataSizeX-1)
        {
            sum = 0;
            t = x;
            for (m = 0; m <= y; ++m)
            {
                for (n = x; n < this->_kernelWidth; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
                t += x;  // jump to next row
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }

        ++y;  // add one more row to convolve for next run
    }

    // convolve rows from index=kCenterY to index=(dataSizeY-kCenterY-1)
    for (i = kernelCenterY; i < rowEnd; ++i)  // number of rows
    {
        // partition #4 ***********************************
        x = kernelCenterX;
        for (j = 0; j < kernelCenterX; ++j)  // column from index=0 to index=kCenterX-1
        {
            sum = 0;
            t = 0;
            for (m = 0; m < this->_kernelHeight; ++m)
            {
                for (n = 0; n <= x; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
                t += (this->_kernelWidth - x - 1);
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }

        // partition #5 ***********************************
        for (j = kernelCenterX; j < colEnd; ++j)  // column from index=kCenterX to index=(dataSizeX-kCenterX-1)
        {
            sum = 0;
            t = 0;
            for (m = 0; m < this->_kernelHeight; ++m)
            {
                for (n = 0; n < this->_kernelWidth; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++inPtr[t];  // in this partition, all cursors are used to convolve. moving cursors to next is safe
                                 // here
                    ++t;
                }
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
        }

        // partition #6 ***********************************
        x = 1;
        for (j = colEnd; j < dataSizeX; ++j)  // column from index=(dataSizeX-kCenter) to index=(dataSizeX-1)
        {
            sum = 0;
            t = x;
            for (m = 0; m < this->_kernelHeight; ++m)
            {
                for (n = x; n < this->_kernelWidth; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
                t += x;
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }
    }

    // convolve rows from index=(dataSizeY-kCenterY) to index=(dataSizeY-1)
    y = 1;
    for (i = rowEnd; i < dataSizeY; ++i)  // number of rows
    {
        // partition #7 ***********************************
        x = kernelCenterX;
        for (j = 0; j < kernelCenterX; ++j)  // column from index=0 to index=kCenterX-1
        {
            sum = 0;
            t = this->_kernelWidth * y;

            for (m = y; m < this->_kernelHeight; ++m)
            {
                for (n = 0; n <= x; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
                t += (this->_kernelWidth - x - 1);
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }

        // partition #8 ***********************************
        for (j = kernelCenterX; j < colEnd; ++j)  // column from index=kCenterX to index=(dataSizeX-kCenterX-1)
        {
            sum = 0;
            t = this->_kernelWidth * y;
            for (m = y; m < this->_kernelHeight; ++m)
            {
                for (n = 0; n < this->_kernelWidth; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];
        }

        // partition #9 ***********************************
        x = 1;
        for (j = colEnd; j < dataSizeX; ++j)  // column from index=(dataSizeX-kCenter) to index=(dataSizeX-1)
        {
            sum = 0;
            t = this->_kernelWidth * y + x;
            for (m = y; m < this->_kernelHeight; ++m)
            {
                for (n = x; n < this->_kernelWidth; ++n)
                {
                    sum += *inPtr[t] * this->_kernel[t];
                    ++t;
                }
                t += x;
            }

            // store output
            *outPtr = (unsigned char)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; ++k)
                ++inPtr[k];  // move all cursors to next
        }
        ++y;  // the starting row index is increased
    }

    for (int i = 0; i < destinationImage.rows; i++) {
        float* dataRow = destinationImage.ptr<float>(i);
        for (int j = 0; j < destinationImage.cols; j++) {
            dataRow[j] /= 255.0;
        }
    }
    return 0;
}

int Convolution::DoConvolution_1(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
    uchar* in = sourceImage.data;
    uchar* out = destinationImage.data;
    int dataSizeX = sourceImage.cols;
    int dataSizeY = sourceImage.rows;
    int kSizeX = this->_kernelWidth;
    int kSizeY = this->_kernelHeight;
    std::vector <int> kernelX;
    std::vector <int> kernelY;
    for (int i = 0; i < this->_kernelHeight; i++)
        for (int j = 0; j < this->_kernelWidth; j++) {
            kernelX.push_back(i - (this->_kernelHeight / 2));
            kernelY.push_back(j - (this->_kernelWidth / 2));
        }

    // check validity of params
    if (!in || !out || kernelX.empty() || kernelY.empty())
        return false;
    if (dataSizeX <= 0 || kSizeX <= 0)
        return false;

    // Save temporary horizontal convolution for the entire image
    int N = dataSizeX * dataSizeY;
    float* tmpx = new float[N];
    if (!tmpx)
        return false;  // memory allocation error
    for (int i = 0; i < N; ++i)
        tmpx[i] = 0;

    // Save temporary vertical convolution for one row
    float* tmpsum = new float[dataSizeX];
    if (!tmpsum)
        return false;  // memory allocation error
    for (int i = 0; i < dataSizeX; ++i)
        tmpsum[i] = 0;

    // find center position of kernel (half of kernel size)
    int kCenter = kSizeX >> 1;           // center index of kernel array
    int endIndex = dataSizeX - kCenter;  // index for full kernel convolution
    int right_half = kSizeX - kCenter - 1; // size of right half right to index 'kCenter' in the kernel

    /* Convolution in horizontal direction.
        We split [0, dataSizeX-1] into three parts: [0, kCenter - 1], [kCenter, endIndex - 1] and
        [endIndex, dataSizeX - 1]. The middle part is general case, while the first and last part
        are image border cases.
    */
    // [0, kCenter - 1]
    for (int j = 0; j < dataSizeY; ++j)
    {
        int offset = 0;
        for (int i = 0; i < kCenter; ++i)
        {
            int idx = j * dataSizeX + i;
            for (int k = kCenter + offset, m = 0; k >= 0; k--, m++)
                tmpx[idx] += in[idx - offset + m] * kernelX[k];
            offset++;
        }
    }
    // [kCenter, endIndex - 1]
    for (int j = 0; j < dataSizeY; ++j)
    {
        for (int i = kCenter; i < endIndex; ++i)
        {
            int idx = j * dataSizeX + i;
            for (int k = kSizeX - 1, m = 0; k >= 0; k--, m++)
                tmpx[idx] += in[idx - right_half + m] * kernelX[k];
        }
    }
    // [endIndex, dataSizeX - 1]
    for (int j = 0; j < dataSizeY; ++j)
    {
        int offset = 1;
        for (int i = endIndex; i < dataSizeX; ++i)
        {
            int idx = j * dataSizeX + i;
            for (int k = kSizeX - 1, m = 0; k >= offset; k--, m++)
                tmpx[idx] += in[idx - right_half + m] * kernelX[k];
            offset++;
        }
    }

    /* Convolution in vertical direction. Similiar to horizontal ones. */

    kCenter = kSizeY >> 1;
    endIndex = dataSizeY - kCenter;
    right_half = kSizeY - kCenter - 1;

    // [0, kCenter - 1]
    int offset = 0;
    for (int j = 0; j < kCenter; ++j)
    {
        for (int k = kCenter + offset, row = 0; k >= 0; k--, row++)
        {
            for (int i = 0; i < dataSizeX; ++i)
            {
                int idx = row * dataSizeX + i;
                tmpsum[i] += tmpx[idx] * kernelY[k];  // tmpsum is 1D row vector
            }
        }
        offset++;

        // Copy tmpSum result to final output image. One 1D row vector 'tmpsum'
        // is enough and this can save storage.
        for (int i = 0; i < dataSizeX; ++i)
        {
            int idx = j * dataSizeX + i;
            out[idx] = (unsigned char)(float(tmpsum[i]) + 0.5f);
            tmpsum[i] = 0;
        }
    }
    // [kCenter, endIndex - 1]
    for (int j = kCenter; j < endIndex; ++j)
    {
        for (int k = kSizeY - 1, m = 0; k >= 0; k--, m++)
        {
            int row = j - right_half + m;
            for (int i = 0; i < dataSizeX; ++i)
            {
                int idx = row * dataSizeX + i;
                tmpsum[i] += tmpx[idx] * kernelY[k];
            }
        }
        for (int i = 0; i < dataSizeX; ++i)
        {
            int idx = j * dataSizeX + i;
            out[idx] = (unsigned char)(float(tmpsum[i]) + 0.5f);
            tmpsum[i] = 0;
        }
    }

    // [endIndex, dataSizeY - 1]
    offset = 1;
    for (int j = endIndex; j < dataSizeY; ++j)
    {
        for (int k = kSizeY - 1, m = 0; k >= offset; k--, m++)
        {
            int row = j - right_half + m;
            for (int i = 0; i < dataSizeX; ++i)
            {
                int idx = row * dataSizeX + i;
                tmpsum[i] += tmpx[idx] * kernelY[k];  // tmpsum is 1D row vector
            }
        }
        offset++;
        for (int i = 0; i < dataSizeX; ++i)
        {
            int idx = j * dataSizeX + i;
            out[idx] = (unsigned char)(float(tmpsum[i]) + 0.5f);
            tmpsum[i] = 0;
        }
    }

    delete[] tmpx;
    delete[] tmpsum;
    return 0;
}

int Convolution::DoConvolution_2(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
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

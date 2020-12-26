#include "Convolution.h"

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
        return 1;
    }

    if ((blueLayer.cols != greenLayer.cols) || (blueLayer.cols  != redLayer.cols)) {
        std::cout << "[EXCEPTION] Error with input image.\n";
        return 1;
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
    // Tham khảo: http://www.songho.ca/dsp/convolution/convolution.html

    // Con trỏ quản lý vùng data Source Image
    uchar* ptrSourceData = sourceImage.data;
    // Con trỏ quản lý vùng data Destination Image
    uchar* ptrDestinationData = destinationImage.data;

    // Chiều dài của ảnh source
    int sourceWidth = sourceImage.cols;

    // Chiều cao của ảnh source
    int sourceHeight = sourceImage.rows;

    int i, j, m, n, x, y, t;
    uchar** inPtr, * outPtr, * ptr;

    // Chiều dài của kernel
    int kernelWidth = this->_kernelWidth;

    // Chiều cao của kernel
    int kernelHeight = this->_kernelHeight;

    // Hai điểm tọa độ tâm của kernel
    int kCenterX, kCenterY;
    int rowEnd, colEnd;                             // ending indice for section divider
    float sum;                                      // temp accumulation buffer
    int k, kernelSize;
    

    // Kiểm tra tính hợp lệ của thông số
    if (!ptrSourceData || !ptrDestinationData || this->_kernel.empty())
    {
        // Có tồn tại source data hay không?
        if (!ptrSourceData) {
            std::cout << "[EXCEPTION] Error occurs with source data.\n";
        }

        // Có tồn tại destination data hay không?
        if (!ptrDestinationData) {
            std::cout << "[EXCEPTION] Error occurs with destination data.\n";
        }

        // Kernel có rỗng không?
        if (this->_kernel.empty()) {
            std::cout << "[EXCEPTION] Error occurs with kernel data.\n";
        }

        return 1;
    }

    
    if (sourceWidth <= 0 || kernelWidth <= 0) {
        return 1;
    }

    // Tìm vị trí trung tâm của kernel: Bằng shift right 1 kernelWidth và kernelHeight
    kCenterX = kernelWidth >> 1;
    kCenterY = kernelHeight >> 1;

    // Diện tích kernel = kernelWidth * kernelHeight
    kernelSize = kernelWidth * kernelHeight;             

    // Khởi tạo vùng nhớ cho multi-cursor
    inPtr = new unsigned char* [kernelSize];
    // Nếu khởi tạo vùng nhớ thất bại
    if (!inPtr) {
        return 1;                        
    }

    // set initial position of multi-cursor, NOTE: it is swapped instead of kernel
    // the first cursor is shifted (kCenterX, kCenterY)
    ptr = ptrSourceData + ((long long)sourceWidth * kCenterY + kCenterX); 
    for (m = 0, t = 0; m < kernelHeight; m = -~m)
    {
        for (n = 0; n < kernelWidth; n = -~n, t = -~t)
        {
            inPtr[t] = ptr - n;
        }
        ptr -= sourceWidth;
    }

    // init working  pointers
    outPtr = ptrDestinationData;

    rowEnd = sourceHeight - kCenterY; // bottom row partition divider                  
    colEnd = sourceWidth - kCenterX; // right column partition divider                 

    // convolve rows từ index = 0 đến index = kCenterY - 1
    y = kCenterY;
    for (i = 0; i < kCenterY; i = -~i)
    {
        // partition #1 - Phân vùng 01
        x = kCenterX;
        // column từ index = 0 đến index = kCenterX - 1    
        for (j = 0; j < kCenterX; j = -~j)             
        {
            sum = 0;
            t = 0;
            for (m = 0; m <= y; m = -~m)
            {
                for (n = 0; n <= x; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += (kernelWidth - x - 1); // jump to next row         
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k];// move all cursors to next
            }
        }

        // partition #2 - Phân vùng 02
        // column từ index = kCenterX đến index = (sourceWidth - kCenterX - 1)
        for (j = kCenterX; j < colEnd; j=-~j)            
        {
            sum = 0;
            t = 0;
            for (m = 0; m <= y; m = -~m)
            {
                for (n = 0; n < kernelWidth; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k]; // move all cursors to next
            }
        }

        // partition #3 - Phân vùng 03
        // column từ index = (sourceWidth - kCenter) đến index = (sourceWidth - 1)
        x = 1;
        for (j = colEnd; j < sourceWidth; j = -~j)           
        {
            sum = 0;
            t = x;
            for (m = 0; m <= y; m = -~m)
            {
                for (n = x; n < kernelWidth; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += x; // jump to next row
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k];    // move all cursors to next
            }
        }

        ++y; // add one more row to convolve for next run
    }

    // convolve rows từ index = sourceHeight đến index = (sourceHeight - kCenterY - 1)
    for (i = kCenterY; i < rowEnd; i = -~i) // number of rows
    {
        // partition #4 - Phân vùng 04
        x = kCenterX;
        for (j = 0; j < kCenterX; j = -~j)  // column từ index = 0 đến index = kCenterX - 1
        {
            sum = 0;
            t = 0;
            for (m = 0; m < kernelHeight; m = -~m)
            {
                for (n = 0; n <= x; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += (kernelWidth - x - 1);
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k];    // move all cursors to next
            }
        }

        // partition #5 - Phân vùng 05
        for (j = kCenterX; j < colEnd; j = -~j) // column từ index = kCenterX to index = (sourceWidth - kCenterX -1)
        {
            sum = 0;
            t = 0;
            for (m = 0; m < kernelHeight; m = -~m)
            {
                for (n = 0; n < kernelWidth; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++inPtr[t]; // in this partition, all cursors are used to convolve. moving cursors to next is safe here
                    ++t;
                }
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
        }

        // partition #6 - Phân vùng 06
        x = 1;
        for (j = colEnd; j < sourceWidth; j = -~j)  // column from index = (sourceWidth - kCenter) to index = (sourceWidth - 1)
        {
            sum = 0;
            t = x;
            for (m = 0; m < kernelHeight; m = -~m)
            {
                for (n = x; n < kernelWidth; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += x;
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k];    // move all cursors to next
            }
        }
    }

    // convolve rows từ index = (sourceHeight - kCenterY) đến index = (sourceHeight - 1)
    y = 1;
    for (i = rowEnd; i < sourceHeight; i = -~i) // number of rows
    {
        // partition #7 - Phân vùng 07
        x = kCenterX;
        for (j = 0; j < kCenterX; j = -~j) // column từ index = 0 đến index = kCenterX - 1
        {
            sum = 0;
            t = kernelWidth * y;

            for (m = y; m < kernelHeight; m = -~m)
            {
                for (n = 0; n <= x; ++n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += (kernelWidth - x - 1);
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k]; // move all cursors to next
            }
        }

        // partition #8 - Phân vùng 08
        for (j = kCenterX; j < colEnd; j = -~j)  // column từ index = kCenterX to index = (sourceWidth - kCenterX - 1)
        {
            sum = 0;
            t = kernelWidth * y;
            for (m = y; m < kernelHeight; m = -~m)
            {
                for (n = 0; n < kernelWidth; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++inPtr[k];
            }
        }

        // partition #9 - Phân vùng 09
        x = 1;
        for (j = colEnd; j < sourceWidth; j = -~j)  // column từ index = (sourceWidth - kCenter) to index = (sourceWidth - 1)
        {
            sum = 0;
            t = kernelWidth * y + x;
            for (m = y; m < kernelHeight; m = -~m)
            {
                for (n = x; n < kernelWidth; n = -~n)
                {
                    // Tích chập
                    sum += *inPtr[t] * (this->_kernel[t]);
                    ++t;
                }
                t += x;
            }

            // Đưa ra output
            *outPtr = (uchar)((float)fabs(sum) + 0.5f);
            ++outPtr;
            ++x;
            for (k = 0; k < kernelSize; k = -~k) {
                ++(inPtr[k]); 
            }    // move all cursors to next
        }

        ++y; // the starting row index is increased
    }

    return 0;
}


int Convolution::DoConvolutionColor(const cv::Mat& sourceImage, cv::Mat& destinationImage)
{
    // Khởi tạo ảnh nguồn là ảnh 3 kênh màu với chiều cao (Height) = chiều cao ảnh nguồn  (sourceImage.rows), chiều rộng (width) = chiều rộng ảnh nguồn (sourceImage.cols)
    destinationImage = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3, cv::Scalar(0));

    // Khởi tạo Blue Layer là ma trận chứa giá trị kênh màu blue
    // Chiều cao (Height) = chiều cao ảnh nguồn  (sourceImage.rows), chiều rộng (width) = chiều rộng ảnh nguồn (sourceImage.cols)
    cv::Mat blueLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, cv::Scalar(0));

    // Khởi tạo Green Layer là ma trận chứa giá trị kênh màu blue
    // Chiều cao (Height) = chiều cao ảnh nguồn  (sourceImage.rows), chiều rộng (width) = chiều rộng ảnh nguồn (sourceImage.cols)
    cv::Mat greenLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, cv::Scalar(0));

    // Khởi tạo Red Layer là ma trận chứa giá trị kênh màu blue
   // Chiều cao (Height) = chiều cao ảnh nguồn  (sourceImage.rows), chiều rộng (width) = chiều rộng ảnh nguồn (sourceImage.cols)
    cv::Mat redLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, cv::Scalar(0));

    // Thao tác tách ảnh nguồn thành ba layer Blue, Green, Red
    // Thao tác tách ảnh nguồn ra Blue Layer bằng cách gọi phương thức separateColorImg bới param type = 0
    if (this->separateColorImg(sourceImage, blueLayer, 0) == 1) {
        std::cout << "[EXCEPTION] Error when separate color image into Blue Layer.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success separate color image into Blue Layer.\n";
    }

    // Thao tác tách ảnh nguồn ra Green Layer bằng cách gọi phương thức separateColorImg bới param type = 1
    if (this->separateColorImg(sourceImage, greenLayer, 1) == 1) {
        std::cout << "[EXCEPTION] Error when separate color image into Green Layer.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success separate color image into Green Layer.\n";
    }

    // Thao tác tách ảnh nguồn ra Red Layer bằng cách gọi phương thức separateColorImg bới param type = 2
    if (this->separateColorImg(sourceImage, redLayer, 2) == 1) {
        std::cout << "[EXCEPTION] Error when separate color image into Red Layer.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success separate color image into Red Layer.\n";
    }

    // Khởi tạo các ma trận Destination convolution cho các Layer: Blue, Green, Red
    cv::Mat blueLayerConvole = blueLayer.clone();
    cv::Mat greenLayerConvole = greenLayer.clone();
    cv::Mat redLayerConvole = redLayer.clone();

    // Thực thi Convolution 2D với từng layer bằng cách gọi phương thức DoConvolution với các param đúng theo pattern
    if (this->DoConvolution(blueLayer, blueLayerConvole) == 1) {
        std::cout << "[EXCEPTION] Error do convolution for Blue Layer.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success do convolution for Blue Layer.\n";
    }

    if (this->DoConvolution(greenLayer, greenLayerConvole) == 1) {
        std::cout << "[EXCEPTION] Error do convolution for Green Layer.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success do convolution for Green Layer.\n";
    }

    if (this->DoConvolution(redLayer, redLayerConvole) == 1) {
        std::cout << "[EXCEPTION] Error do convolution for Red Layer.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success do convolution for Red Layer.\n";
    }

    // Thực thi thao tác gộp các layer Blue, Green, Red sau khi Convole để kết xuất ảnh kết quả bằng cách gọi phương thức mergeColorImg  với các param đúng theo pattern
    if (this->mergeColorImg(destinationImage, blueLayerConvole, greenLayerConvole, redLayerConvole) == 1) {
        std::cout << "[EXCEPTION] Error when merge Blue, Green, Red layers.\n";
        return 1;
    }
    else {
        std::cout << "[LOG] Success merge Blue, Green, Red layers.\n";
    }

    // Trả về 0: Thành công
    return 0;
}

// Constructor cho lớp Convolution
Convolution::Convolution()
{
    this->_kernel = std::vector<float>(0, 0);
    this->_kernelWidth = 0;
    this->_kernelHeight = 0;
}

// Destructor cho lớp Convolution
Convolution::~Convolution()
{
    if (!this->_kernel.empty()) {
        this->_kernel.clear();
    }
}


/*
N - number of images
K - kernel size
W - output width
H - input width
C - number of input channels
D - number of output channels
Naive convolution

for (int n = 0; n < N; ++n)
    for (int d = 0; d < D; ++d)
        for (int w = 0; w < W; ++w)
            for (int h = 0; h < H; ++h)
                float value = 0;
                for (c = 0; c < C; ++c)
                    for (kx = 0; kx < K; ++kx)
                        for (ky = 0; ky < K; ++ky)
                            value += kernel(kx, ky, c, d) * input(w + kx, h + ky, c, n)
                output(wOut, hOut, cOut, n) = value
  
 */
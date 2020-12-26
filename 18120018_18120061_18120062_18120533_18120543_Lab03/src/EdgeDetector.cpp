#include "EdgeDetector.h"
#include "Kernel.h"
#include "Converter.h"

/*
Hàm phát hiện biên cạnh của ảnh xám
sourceImage: ảnh input
destinationImage: ảnh output
method: phương pháp detect
kWidth, kHeight: kích thước kernel
	1: Sobel
	2: Prewitt
	3: Laplace
Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
*/
int EdgeDetector::DetectEdge(const cv::Mat& sourceImage, cv::Mat& destinationImage, int kWidth, int kHeight, int method)
{
	// Kiểm trả ảnh đầu vào
	if (!sourceImage.data) {
		std::cout << "[EXCEPTION] Error with input image.\n";
		return 1;	    // Trả về 1
	}

	if (kWidth < 0) {
		std::cout << "[EXCEPTION] kWidth negative.\n";
		return 1;	 // Trả về 1
	}

	if (kHeight < 0) {
		std::cout << "[EXCEPTION] kHeigh negative.\n";
		return 1;		   // Trả về 1
	}

	cv::Mat sourceImg;

	if (sourceImage.channels() != 1) {
		std::cout << "[LOG] Enable mode for color image - RGB.\n";
		Converter converter;
		cv::Mat sourceClone = sourceImage.clone();
		converter.Convert(sourceClone, sourceImg, 0);
		converter.~Converter();
		// cv::cvtColor(sourceImage, sourceImg, cv::COLOR_RGB2GRAY);
		cv::namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Gray image", sourceImg);
	}
	else {
		std::cout << "[LOG] Enable mode for grayscale image - RGB.\n";
		sourceImg = sourceImage.clone();
		cv::namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Gray image", sourceImg);
	}

	// Chiều rộng của ảnh source
	int widthSourceImage = sourceImg.cols;

	// Chiều cao của ảnh source
	int heigthSourceImage = sourceImg.rows;

	// Số channels của ảnh source
	int sourceImageChannels = sourceImg.channels();

	// Widthstep của ảnh source
	size_t sourceWidthStep = sourceImg.step[0];

	// Khởi tạo ảnh kết quả
	destinationImage = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

	// Chiều rộng của ảnh source
	int widthDestinationImage = destinationImage.cols;

	// Chiều cao của ảnh source
	int heigthDestinationImage = destinationImage.rows;

	// Machine epsilon
	float eps = 1e-6;

	switch (method) {
	case 1:	// Sobel
	{
		std::cout << "[LOG] Starting Edge Detection with Sobel ...\n";
		Convolution convolutionSobelX;
		Convolution convolutionSobelY;
		std::cout << "[LOG] Calculating Sobel kernel ...\n";
		convolutionSobelX.SetKernel(getSobelKernelX(), 3, 3);
		convolutionSobelY.SetKernel(getSobelKernelY(), 3, 3);

		// Ma trận ảnh dùng đến chứa kết quả tích chập ảnh nguồn với sobel X
		cv::Mat gSobelX = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

		// Ma trận ảnh dùng đến chứa kết quả tích chập ảnh nguồn với sobel Y
		cv::Mat gSobelY = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

		std::cout << "[LOG] Starting convolution with Sobel kernel X ...\n";
		convolutionSobelX.DoConvolution(sourceImg, gSobelX);

		cv::namedWindow("Sobel X", cv::WINDOW_AUTOSIZE);
		cv::imshow("Sobel X", gSobelX);

		convolutionSobelY.DoConvolution(sourceImg, gSobelY);

		std::cout << "[LOG] Starting convolution with Sobel kernel Y ...\n";
		cv::namedWindow("Sobel Y", cv::WINDOW_AUTOSIZE);
		cv::imshow("Sobel Y", gSobelY);


		std::cout << "[LOG] Ending Edge Detection with Sobel ...\n";
		for (int y = 0; y < heigthDestinationImage; y = -~y)
		{
			uchar* dataRow = destinationImage.ptr<uchar>(y);

			for (int x = 0; x < widthDestinationImage; x = -~x) {

				float fx = gSobelX.ptr<uchar>(y)[x];
				float fy = gSobelY.ptr<uchar>(y)[x];
				float value = sqrt(fx * fx + fy * fy);

				// std::cout << e << "\n";

				if ((value - 25.0) > eps) {
					dataRow[x] = cv::saturate_cast<uchar>(255);
				}
			}
		}

		gSobelX.release();
		gSobelY.release();
		convolutionSobelX.~Convolution();
		convolutionSobelY.~Convolution();
		break;
	}
	case 2:	 // Prewitt
	{
		std::cout << "[LOG] Starting Edge Detection with Prewitt ...\n";
		Convolution convolutionPrewittX;
		Convolution convolutionPrewittY;
		std::cout << "[LOG] Calculating Prewitt kernel ...\n";

		convolutionPrewittX.SetKernel(getPrewittKernelX(), 3, 3); // Nạp Prewitt kernel X
		convolutionPrewittY.SetKernel(getPrewittKernelY(), 3, 3);  // Nạp Prewitt kernel Y

		// Khởi tạo hai matrix kết quả trung gian convolution
		cv::Mat gPrewittX = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));
		cv::Mat gPrewittY = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

		std::cout << "[LOG] Starting convolution with Prewitt kernel X ...\n";
		convolutionPrewittX.DoConvolution(sourceImg, gPrewittX);  // Gọi convolution ảnh nguồn với Prewitt kernel X

		cv::namedWindow("Prewitt X", cv::WINDOW_AUTOSIZE);
		cv::imshow(" Prewitt X", gPrewittX);

		convolutionPrewittY.DoConvolution(sourceImg, gPrewittY); // Gọi convolution ảnh nguồn với Prewitt kernel Y

		std::cout << "[LOG] Starting convolution with Prewitt kernel Y ...\n";
		cv::namedWindow("Prewitt Y", cv::WINDOW_AUTOSIZE);
		cv::imshow("Prewitt Y", gPrewittY);


		std::cout << "[LOG] Ending Edge Detection with Prewitt ...\n";

		// Phân ngưỡng
		for (int y = 0; y < heigthDestinationImage; y = -~y)
		{
			uchar* dataRow = destinationImage.ptr<uchar>(y);

			for (int x = 0; x < widthDestinationImage; x = -~x) {
				float fx = gPrewittX.ptr<uchar>(y)[x];
				float fy = gPrewittY.ptr<uchar>(y)[x];
				float value = sqrt(fx * fx + fy * fy);
				// std::cout << e << "\n";
				if ((value - 25.0) > eps) {
					dataRow[x] = cv::saturate_cast<uchar>(255);
				}
			}
		}
		gPrewittX.release();
		gPrewittY.release();
		convolutionPrewittX.~Convolution();
		convolutionPrewittY.~Convolution();
		break;
	}
	case 3:	 // Laplace
	{
		std::cout << "[LOG] Starting Edge Detection with Laplace ...\n";
		Convolution convolution;
		std::cout << "[LOG] Calculating Laplace kernel ...\n";
		convolution.SetKernel(getLaplaceKernel(), 3, 3); // Nạp Laplace kernel

		std::cout << "[LOG] Starting convolution with Laplace kernel ...\n";
		// Gọi convolution
		convolution.DoConvolution(sourceImg, destinationImage);

		float threshold = -1;

		// Số channels của ảnh destination
		int destinationChannels = destinationImage.channels();

		// Widthstep của ảnh destination
		int destinationWidthStep = destinationImage.step[0];

		// Con trỏ quản lý vùng nhớ data ảnh destination
		uchar* ptrDestinationData = destinationImage.data;

		// Tìm giá trị màu lớn nhất trong ảnh destination
		for (int y = 0; y < heigthSourceImage; y++, ptrDestinationData += destinationWidthStep) {
			uchar* ptrDestinationRow = ptrDestinationData;

			for (int x = 0; x < widthSourceImage; x++, ptrDestinationRow += destinationChannels) {

				uchar value = ptrDestinationRow[0];

				threshold = value > threshold ? value : threshold;
			}
		}

		// threshold = maxValueColor / 4 
		threshold /= 4;

		int offsets[9] = { 
			-destinationWidthStep - 1, -destinationWidthStep, -destinationWidthStep + 1,
			-1, 0, 1, 
			destinationWidthStep - 1, destinationWidthStep, destinationWidthStep + 1 };

		for (int y = 1; y < heigthSourceImage; y = -~y) {

			for (int x = 1; x < widthSourceImage; x = -~x) {

				int count = 0;
				// Đường chéo 1
				if (abs(ptrDestinationData[offsets[0]] - ptrDestinationData[offsets[8]]) - eps > threshold) {
					count = count + 1;
				}

				// Đường chéo 2
				if (abs(ptrDestinationData[offsets[2]] - ptrDestinationData[offsets[6]]) - eps > threshold) {
					count = count + 1;
				}

				// Trên dưới
				if (abs(ptrDestinationData[offsets[1]] - ptrDestinationData[offsets[7]]) - eps > threshold) {
					count = count + 1;
				}

				// Trái phải
				if (abs(ptrDestinationData[offsets[3]] - ptrDestinationData[offsets[5]]) - eps > threshold) {
					count = count + 1;
				}

				if (count >= 2) {
					ptrDestinationData[offsets[4]] = 255;
				}
			
			}
		}

		std::cout << "[LOG] Ending Edge Detection with Laplace ...\n";

		break;
	}
	}
    return 0;	 // Trả về 0
}

EdgeDetector::EdgeDetector() = default;

EdgeDetector::~EdgeDetector() = default;

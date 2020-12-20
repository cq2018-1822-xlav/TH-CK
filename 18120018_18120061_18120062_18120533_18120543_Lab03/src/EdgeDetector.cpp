﻿#include "EdgeDetector.h"
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
		return 1;
	}

	if (kWidth < 0) {
		std::cout << "[EXCEPTION] kWidth negative.\n";
		return 1;
	}

	if (kHeight < 0) {
		std::cout << "[EXCEPTION] kHeigh negative.\n";
		return 1;
	}

	cv::Mat sourceImg;

	if (sourceImage.channels() != 1) {
		std::cout << "[LOG] Source image channels NOT equals to 1.\n";
		Converter converter;
		cv::Mat sourceClone = sourceImage.clone();
		converter.Convert(sourceClone, sourceImg, 0);
		converter.~Converter();
		// cv::cvtColor(sourceImage, sourceImg, cv::COLOR_RGB2GRAY);
		cv::namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Gray image", sourceImg);
	}
	else {
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

	destinationImage = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));
	float eps = 1e-6;
	switch (method) {
	case 1:	// Sobel
	{
		std::cout << "Starting Edge Detection with Sobel ...\n";
		Convolution convolutionSobelX;
		Convolution convolutionSobelY;
		std::cout << "Calculating Sobel kernel ...\n";
		convolutionSobelX.SetKernel(getSobelKernelX(), 3, 3);
		convolutionSobelY.SetKernel(getSobelKernelY(), 3, 3);
		cv::Mat gSobelX = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));
		cv::Mat gSobelY = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

		std::cout << "Starting convolution with Sobel kernel X ...\n";
		convolutionSobelX.DoConvolution(sourceImg, gSobelX);

		cv::namedWindow("Sobel X", cv::WINDOW_AUTOSIZE);
		cv::imshow("Sobel X", gSobelX);

		convolutionSobelY.DoConvolution(sourceImg, gSobelY);

		std::cout << "Starting convolution with Sobel kernel Y ...\n";
		cv::namedWindow("Sobel Y", cv::WINDOW_AUTOSIZE);
		cv::imshow("Sobel Y", gSobelY);


		std::cout << "Ending Edge Detection with Sobel ...\n";
		for (int i = 0; i < destinationImage.rows; i++)
		{
			uchar* dataRow = destinationImage.ptr<uchar>(i);
			for (int j = 0; j < destinationImage.cols; j++) {
				float fx = gSobelX.ptr<uchar>(i)[j];
				float fy = gSobelY.ptr<uchar>(i)[j];
				float e = sqrt(fx * fx + fy * fy);
				// std::cout << e << "\n";
				if ((e - 25.0) > 1e-6) {
					dataRow[j] = cv::saturate_cast<uchar>(255);
				}
			}
		}
		return 0;
		break;
	}
	case 2:	 // Prewitt
	{
		std::cout << "Starting Edge Detection with Prewitt ...\n";
		Convolution convolutionPrewittX;
		Convolution convolutionPrewittY;
		std::cout << "Calculating Prewitt kernel ...\n";
		convolutionPrewittX.SetKernel(getPrewittKernelX(), 3, 3);
		convolutionPrewittY.SetKernel(getPrewittKernelY(), 3, 3);
		cv::Mat gPrewittX = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));
		cv::Mat gPrewittY = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

		std::cout << "Starting convolution with Prewitt kernel X ...\n";
		convolutionPrewittX.DoConvolution(sourceImg, gPrewittX);

		cv::namedWindow("Prewitt X", cv::WINDOW_AUTOSIZE);
		cv::imshow(" Prewitt X", gPrewittX);

		convolutionPrewittY.DoConvolution(sourceImg, gPrewittY);

		std::cout << "Starting convolution with Prewitt kernel Y ...\n";
		cv::namedWindow("Prewitt Y", cv::WINDOW_AUTOSIZE);
		cv::imshow("Prewitt Y", gPrewittY);


		std::cout << "Ending Edge Detection with Prewitt ...\n";
		for (int i = 0; i < destinationImage.rows; i++)
		{
			uchar* dataRow = destinationImage.ptr<uchar>(i);
			for (int j = 0; j < destinationImage.cols; j++) {
				float fx = gPrewittX.ptr<uchar>(i)[j];
				float fy = gPrewittY.ptr<uchar>(i)[j];
				float e = sqrt(fx * fx + fy * fy);
				// std::cout << e << "\n";
				if ((e - 30.0) > 1e-6) {
					dataRow[j] = cv::saturate_cast<uchar>(255);
				}
			}
		}
		return 0;
		break;
	}
	case 3:	 // Laplace
	{
		std::cout << "Starting Edge Detection with Laplace ...\n";
		Convolution convolution;
		std::cout << "Calculating Laplace kernel ...\n";
		convolution.SetKernel(getLaplaceKernel(), 3, 3);

		std::cout << "Starting convolution with Laplace kernel ...\n";
		convolution.DoConvolution(sourceImg, destinationImage);

		float threshold = -1;

		// Số channels của ảnh destination
		int destinationChannels = destinationImage.channels();

		// Widthstep của ảnh destination
		int destinationWidthStep = destinationImage.step[0];

		// Con trỏ quản lý vùng nhớ data ảnh destination
		uchar* ptrDestinationData = destinationImage.data;

		for (int y = 0; y < heigthSourceImage; y++, ptrDestinationData += destinationWidthStep) {
			uchar* ptrDestinationRow = ptrDestinationData;

			for (int x = 0; x < widthSourceImage; x++, ptrDestinationRow += destinationChannels) {

				uchar value = ptrDestinationRow[0];

				threshold = value > threshold ? value : threshold;
			}
		}

		threshold /= 4;

		int offsets[9] = { -destinationWidthStep - 1, -destinationWidthStep, -destinationWidthStep + 1, -1, 0, 1, destinationWidthStep - 1, destinationWidthStep, destinationWidthStep + 1 };

		for (int y = 1; y < heigthSourceImage; y++) {

			for (int x = 1; x < widthSourceImage; x++) {

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
				// Trên dưới
				if (abs(ptrDestinationData[offsets[3]] - ptrDestinationData[offsets[5]]) - eps > threshold) {
					count = count + 1;
				}

				if (count >= 2) {
					ptrDestinationData[offsets[4]] = 255;
				}
			
			}
		}

		break;
	}
	}
    return 0;
}

EdgeDetector::EdgeDetector() = default;

EdgeDetector::~EdgeDetector() = default;

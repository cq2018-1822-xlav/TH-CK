#include <iostream>
#include <functional>
#include "EdgeDetector.h"
#include "Blur.h"
#include "Converter.h"
#include "Kernel.h"

static int __str_cmp__(const char* string_1, const char* string_2)
{
	while (*string_1)
	{
		// if characters differ or end of second string is reached
		if (*string_1 != *string_2)
			break;

		// move to next pair of characters
		string_1++;
		string_2++;
	}
	// return the ASCII difference after converting char* to unsigned char*
	return *(const unsigned char*)string_1 - *(const unsigned char*)string_2 == 0 ? 1 : 0;
}

// <Program.exe> <Command> <InputPath> <CmdArguments>
int main(int argc, char* argv[]) {
	EdgeDetector edgeDetector;
	Blur blur;

	// Convolution convolution;
	// Kernel kernel;
	// convolution.SetKernel(getGaussianKernel(17, 17, 1, 2), 17, 17);
	// kernel.displayKernel(convolution.GetKernel());
	// std::cout << convolution.GetKernel().size();
	// cv::Mat inputImage = cv::imread("E:/lena.jpg", cv::IMREAD_ANYCOLOR);
	//cv::Mat inputImage2gray;
	//cv::cvtColor(inputImage, inputImage2gray, cv::COLOR_BGR2GRAY);
	// cv::Mat  outputImage;
	// convolution.DoConvolutionColor(inputImage, outputImage);
	// blur.BlurImage(inputImage, outputImage, 17, 17, 0);
	// convolution.DoConvolutionColor(inputImage, outputImage);
	// blur.BlurImage(inputImage, outputImage, 17, 17, 2);
	// edgeDetector.DetectEdge(inputImage, outputImage, 17, 17, 3);
	// cv::namedWindow("Origin image", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Origin image", inputImage);
	//cv::namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Gray image", inputImage2gray);
	// cv::namedWindow("Output image", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Output image", outputImage);
	// cv::waitKey(0);

	// cv::destroyAllWindows();


	// Code for CLI
	if (__str_cmp__(argv[1], "--mean")) {
		if (argc < 5) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}

		int kWidth = atoi(argv[3]);
		int kHeight = atoi(argv[4]);

		if (kWidth < 0) {
			std::cout << "[Exception] kWidth must be a positive integer.\n";
			return 1;
		}

		if (kHeight < 0) {
			std::cout << "[Exception] kHeight must be a positive integer.\n";
			return 1;
		}

		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);

		if (!inputImage.data) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		int res = blur.BlurImage(inputImage, outputImage, kWidth, kHeight, 0);
		if (res == 0) {
			std::cout << "[SUCCESS] Success filter image with mean filter.\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyAllWindows();

			inputImage.release();
			outputImage.release();
			return 0;
		}
		else {
			std::cout << "[FAILURE] Failed filter image with mean filter.\n";
			inputImage.release();
			outputImage.release();
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--median")) {
		if (argc < 5) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}

		int kWidth = atoi(argv[3]);
		int kHeight = atoi(argv[4]);

		if (kWidth < 0) {
			std::cout << "[Exception] kWidth must be a positive integer.\n";
			return 1;
		}

		if (kHeight < 0) {
			std::cout << "[Exception] kHeight must be a positive integer.\n";
			return 1;
		}


		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);

		if (!inputImage.data) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (blur.BlurImage(inputImage, outputImage, kWidth, kHeight, 1) == 0) {
			std::cout << "[SUCCESS] Success filter image with median filter.\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyAllWindows();
			inputImage.release();
			outputImage.release();
			return 0;
		}
		else {
			std::cout << "[FAILURE] Failed filter image with median filter.\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--gauss")) {
		if (argc < 5) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}

		int kWidth = atoi(argv[3]);
		int kHeight = atoi(argv[4]);

		if (kWidth < 0) {
			std::cout << "[Exception]\n";
			return 1;
		}

		if (kHeight < 0) {
			std::cout << "[Exception]\n";
			return 1;
		}

		// Đọc ảnh đầu vào
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);

		if (!inputImage.data) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (blur.BlurImage(inputImage, outputImage, kWidth, kHeight, 2) == 0) {
			std::cout << "[SUCCESS] Success filter image with Gaussian filter.\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);
			cv::waitKey(0);
			cv::destroyAllWindows();
			inputImage.release();
			outputImage.release();
			return 0;
		}
		else {
			std::cout << "[FAILURE] Failed filter image with Gaussian filter.\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--sobel")) {
		if (argc > 3) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}


		// Đọc ảnh đầu vào
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);

		if (!inputImage.data) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		int kWidth = 3;
		int kHeight = 3;
		if (edgeDetector.DetectEdge(inputImage, outputImage, kWidth, kHeight, 1) == 0) {
			std::cout << "[SUCCESS] Success edge detection with Sobel.\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyAllWindows();
			inputImage.release();
			outputImage.release();
			return 0;
		}
		else {
			std::cout << "[FAILURE] Failed edge detection with Sobel.\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--prewitt")) {
		if (argc > 3) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}

		// Đọc ảnh đầu vào
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);

		if (!inputImage.data) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		int kWidth = 3;
		int kHeight = 3;
		if (edgeDetector.DetectEdge(inputImage, outputImage, kWidth, kHeight, 2) == 0) {
			std::cout << "[SUCCESS] Success edge detection with Prewitt.\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyAllWindows();
			inputImage.release();
			outputImage.release();
			return 0;
		}
		else {
			std::cout << "[FAILURE] Failed edge detection with Prewitt.\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--laplace")) {
		if (argc > 3) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}

		// Đọc ảnh đầu vào
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);

		if (!inputImage.data) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		int kWidth = 3;
		int kHeight = 3;
		if (edgeDetector.DetectEdge(inputImage, outputImage, kWidth, kHeight, 3) == 0) {
			std::cout << "[SUCCESS] Success edge detection with Laplace.\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyAllWindows();
			inputImage.release();
			outputImage.release();
			return 0;
		}
		else {
			std::cout << "[FAILURE] Failed edge detection with Laplace.\n";
			return 0;
		}
	}
	else if ((__str_cmp__(argv[1], "--help")) || (__str_cmp__(argv[1], "--h"))) {
		if (argc > 3) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}
		std::cout << "Usage:  18120018_18120061_18120062_18120533_18120543_Lab03.exe [command] [input path] [command arguments]\n\n"
			<< "Options command & command arguments:\n"
			"--mean kWidth kHeight\t: \n"
			"--median kWidth kHeight\t: \n"
			"--gauss kWidth kHeight\t: \n"
			"--sobel\t\n"
			"--prewitt\t\n"
			"--laplace\t\n"
			"\n\nDepartment of Computer Science, University of Science\nAuthor: Hoang-Duc Nguyen, Nhut-Nam Le, Hoang-Nam Nguyen, Bao-Son Duong Doan, Dai-Tai Tran"
			;
	}
	else {
		std::cout << "[Exception] Error arguments.\n";
	}
	// End Code for CLI
	return 0;
}
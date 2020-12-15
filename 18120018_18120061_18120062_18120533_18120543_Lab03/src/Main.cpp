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
	// [out = std::ref(std::cout << "Fuck this")] (){out.get() << " world.!"; }();
	EdgeDetector edgeDetector;
	Blur blur;

	// Put your test code here
	// define 5x5 Gaussian kernel
	std::vector<float> kernel = { 1 / 256.0f,  4 / 256.0f,  6 / 256.0f,  4 / 256.0f, 1 / 256.0f,
						 4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f,
						 6 / 256.0f, 24 / 256.0f, 36 / 256.0f, 24 / 256.0f, 6 / 256.0f,
						 4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f,
						 1 / 256.0f,  4 / 256.0f,  6 / 256.0f,  4 / 256.0f, 1 / 256.0f };
	// radius-1 Gaussian kernel, size 9
	float gaussian_1[9] = {
		0.00022923296f,0.0059770769f,0.060597949f,
		0.24173197f,0.38292751f, 0.24173197f,
		0.060597949f,0.0059770769f,0.00022923296f };
	// radius-2 Gaussian kernel, size 15
	std::vector<float> gaussian_2 = {
		0.00048869418f,0.0024031631f,0.0092463447f,
		0.027839607f,0.065602221f,0.12099898f,0.17469721f,
		0.19744757f,
		0.17469721f,0.12099898f,0.065602221f,0.027839607f,
		0.0092463447f,0.0024031631f,0.00048869418f
	};
	//radius-3 Gaussian kernel, size 23
	std::vector<float> gaussian_3 = {
		0.00016944126f,0.00053842377f,0.0015324751f,0.0039068931f,
		0.0089216027f,0.018248675f,0.033434924f,0.054872241f,
		0.080666073f,0.10622258f,0.12529446f,
		0.13238440f,
		0.12529446f,0.10622258f,0.080666073f,
		0.054872241f,0.033434924f,0.018248675f,0.0089216027f,
		0.0039068931f,0.0015324751f,0.00053842377f,0.00016944126f
	};
	//radius-4 Gaussian kernel, size 29
	std::vector<float> gaussian_4 = {
		0.00022466264f,0.00052009715f,0.0011314391f,0.0023129794f,
		0.0044433107f,0.0080211498f,0.013606987f,0.021691186f,
		0.032493830f,0.045742013f,0.060509924f,0.075220309f,
		0.087870099f,0.096459411f,0.099505201f,0.096459411f,0.087870099f,
		0.075220309f,0.060509924f,0.045742013f,0.032493830f,
		0.021691186f,0.013606987f,0.0080211498f,0.0044433107f,
		0.0023129794f,0.0011314391f,0.00052009715f,0.00022466264f,
	};
	Convolution convolution;
	convolution.SetKernel(getGaussianKernel(5,5,1,2), 5,5);
	cv::Mat inputImage = cv::imread("E:/lena.jpg", cv::IMREAD_ANYCOLOR);
	cv::Mat inputImage2gray;
	cv::cvtColor(inputImage, inputImage2gray, cv::COLOR_BGR2GRAY);
	cv::Mat  outputImage = cv::Mat(inputImage2gray.rows, inputImage2gray.cols, CV_8UC3, cv::Scalar(0));
	convolution.DoConvolutionColor(inputImage, outputImage);

	cv::namedWindow("Origin image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Origin image", inputImage);
	//cv::namedWindow("Gray image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Gray image", inputImage2gray);
	cv::namedWindow("Output image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Output image", outputImage);
	cv::waitKey(0);

	cv::destroyAllWindows();

	// Code for CLI
	if (__str_cmp__(argv[1], "--mean")) {
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


		// Convert to grayscale
		Converter converter;
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);
		if (!inputImage.data || (inputImage.channels() != 3 && inputImage.channels() != 1)) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		cv::Mat inputImageGrayscale;
		converter.Convert(inputImage, inputImageGrayscale, 1);
		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (blur.BlurImage(inputImage, outputImage, kWidth, kHeight, 0) == 0) {
			std::cout << "[SUCCESS].\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyWindow("Input image");
			cv::destroyWindow("Show result image");
			return 0;
		}
		else {
			std::cout << "[FAILURE].\n";
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
			std::cout << "[Exception]\n";
			return 1;
		}

		if (kHeight < 0) {
			std::cout << "[Exception]\n";
			return 1;
		}


		// Convert to grayscale
		Converter converter;
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);
		if (!inputImage.data || (inputImage.channels() != 3 && inputImage.channels() != 1)) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		cv::Mat inputImageGrayscale;
		converter.Convert(inputImage, inputImageGrayscale, 1);
		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (blur.BlurImage(inputImage, outputImage, kWidth, kHeight, 1) == 0) {
			std::cout << "[SUCCESS].\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyWindow("Input image");
			cv::destroyWindow("Show result image");
			return 0;
		}
		else {
			std::cout << "[FAILURE].\n";
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


		// Convert to grayscale
		Converter converter;
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);
		if (!inputImage.data || (inputImage.channels() != 3 && inputImage.channels() != 1)) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		cv::Mat inputImageGrayscale;
		converter.Convert(inputImage, inputImageGrayscale, 1);
		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (blur.BlurImage(inputImage, outputImage, kWidth, kHeight, 2) == 0) {
			std::cout << "[SUCCESS].\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyWindow("Input image");
			cv::destroyWindow("Show result image");
			return 0;
		}
		else {
			std::cout << "[FAILURE].\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--sobel")) {
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


		// Convert to grayscale
		Converter converter;
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);
		if (!inputImage.data || (inputImage.channels() != 3 && inputImage.channels() != 1)) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		cv::Mat inputImageGrayscale;
		converter.Convert(inputImage, inputImageGrayscale, 1);
		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (edgeDetector.DetectEdge(inputImage, outputImage, kWidth, kHeight, 1) == 0) {
			std::cout << "[SUCCESS].\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyWindow("Input image");
			cv::destroyWindow("Show result image");
			return 0;
		}
		else {
			std::cout << "[FAILURE].\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--prewitt")) {
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


		// Convert to grayscale
		Converter converter;
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);
		if (!inputImage.data || (inputImage.channels() != 3 && inputImage.channels() != 1)) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		cv::Mat inputImageGrayscale;
		converter.Convert(inputImage, inputImageGrayscale, 1);
		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (edgeDetector.DetectEdge(inputImage, outputImage, kWidth, kHeight, 2) == 0) {
			std::cout << "[SUCCESS].\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyWindow("Input image");
			cv::destroyWindow("Show result image");
			return 0;
		}
		else {
			std::cout << "[FAILURE].\n";
			return 0;
		}
	}
	else if (__str_cmp__(argv[1], "--laplace")) {
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


		// Convert to grayscale
		Converter converter;
		cv::Mat inputImage = cv::imread(argv[2], cv::IMREAD_ANYCOLOR);
		if (!inputImage.data || (inputImage.channels() != 3 && inputImage.channels() != 1)) {
			std::cout << "[EXCEPTION] Error occurs.\n";
			return 1;
		}

		cv::Mat inputImageGrayscale;
		converter.Convert(inputImage, inputImageGrayscale, 1);
		// Khởi tạo ảnh đầu ra
		cv::Mat outputImage;
		if (edgeDetector.DetectEdge(inputImage, outputImage, kWidth, kHeight, 3) == 0) {
			std::cout << "[SUCCESS].\n";
			// Dispay ảnh ra màn hình
			cv::namedWindow("Input image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Input image", inputImage);
			cv::namedWindow("Show result image", cv::WINDOW_AUTOSIZE);
			cv::imshow("Show result image", outputImage);

			cv::waitKey(0);
			cv::destroyWindow("Input image");
			cv::destroyWindow("Show result image");
			return 0;
		}
		else {
			std::cout << "[FAILURE].\n";
			return 0;
		}
	}
	else if ((__str_cmp__(argv[1], "--help")) || (__str_cmp__(argv[1], "--h"))) {
		if (argc > 3) {
			std::cout << "[Exception] Error arguments.\n";
			return 1;
		}
		std::cout << "Usage:  ColorTranform.exe [command] [input path] [command arguments]\n\n"
			<< "Options command & command arguments:\n"
			"--mean\t\n"
			"--median\t\n"
			"--gauss\t\n"
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
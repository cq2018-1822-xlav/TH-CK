#include <iostream>
#include <functional>
#include "EdgeDetector.h"

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
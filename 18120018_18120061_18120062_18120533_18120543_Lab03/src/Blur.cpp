#include "Blur.h"
#include "Kernel.h"

/*
Hàm làm trơn ảnh xám
sourceImage: ảnh input
destinationImage: ảnh output
method: phương pháp detect
kWidth, kHeight: kích thước kernel
0: Trung bình
1: Trung vị
2: Gaussian
Hàm trả về
0: nếu detect thành công
1: nếu detect thất bại (không đọc được ảnh input,...)
*/
int Blur::BlurImage(const cv::Mat& sourceImage, cv::Mat& destinationImage, int kWidth, int kHeight, int method)
{
	// Kiểm trả ảnh đầu vào
	if (!sourceImage.data) {
		std::cout << "[EXCEPTION] Error with input image.\n";
		return 1;
	}

	if (kWidth < 0) {
		std::cout << "[EXCEPTION] fuck\n";
		return 1;
	}

	if (kHeight < 0) {
		std::cout << "[EXCEPTION] you\n";
		return 1;
	}

	// Chiều rộng của ảnh source
	int widthSourceImage = sourceImage.cols;

	// Chiều cao của ảnh source
	int heigthSourceImage = sourceImage.rows;

	// Số channels của ảnh source
	int sourceImageChannels = sourceImage.channels();

	if (sourceImageChannels != 1) {
		std::cout << "[EXCEPTION] dumaaaa may\n";
		return 1;
	}

	// Switch method
	switch (method)
	{
		std::cout << "[LOG] Switching methods\n";
	case 0: // Lọc Trung Bình
	{
		std::cout << "Blur method starting: Means\n";
		Convolution convolution;
		std::vector<float> mean_kernel;

		std::cout << "Calculating kernel...\n";

		int sizeKernel = kWidth * kHeight;
		for (int i = 0; i < sizeKernel; i++) {
			mean_kernel.push_back(1.0 / sizeKernel);
		}

		//destinationImage = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));

		std::cout << "Set kernel...\n";
		convolution.SetKernel(mean_kernel, kWidth, kHeight);
		std::cout << "Starting convolution...\n";
		convolution.DoConvolution(sourceImage, destinationImage);
		std::cout << "End convolution...\n";
		
		break;
	}
	case 1: // Lọc Trung Vị
	{
		std::cout << "Blur method starting: Median\n";
		Convolution convolution;

		break;
	}
	case 2: // Lọc Gauss
	{
		std::cout << "Blur method starting: Gaussian\n";
		Convolution convolution;
		std::vector<float> gaussian_kernel;

		std::cout << "Calculating kernel...\n";
		gaussian_kernel = getGaussianKernel(kHeight, kWidth, DEFAULT_SIGMA_X, DEFAULT_SIGMA_Y);
		std::cout << "Set kernel...\n";
		convolution.SetKernel(gaussian_kernel, kWidth, kHeight);
		std::cout << "Starting convolution...\n";
		convolution.DoConvolution(sourceImage, destinationImage);
		std::cout << "End convolution...\n";
		break;
	}
	default:
		std::cout << "[EXCEPTION] Unsupprted this method.\n";
		return 1;
	}
	return 0;
}

Blur::Blur() {

}

Blur::~Blur() {

}

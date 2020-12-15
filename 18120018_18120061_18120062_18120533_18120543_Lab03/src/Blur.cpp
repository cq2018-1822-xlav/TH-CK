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
		std::cout << "[EXCEPTION] kWidth negative.\n";
		return 1;
	}

	if (kHeight < 0) {
		std::cout << "[EXCEPTION] kHeigh negative.\n";
		return 1;
	}

	// Chiều rộng của ảnh source
	int widthSourceImage = sourceImage.cols;

	// Chiều cao của ảnh source
	int heigthSourceImage = sourceImage.rows;

	// Số channels của ảnh source
	int sourceImageChannels = sourceImage.channels();

	// Widthstep của ảnh source
	size_t sourceWidthStep = sourceImage.step[0];

	int mode = 0;


	// Ảnh 1 kênh màu
	if (sourceImageChannels == 1) {
		// std::cout << "[EXCEPTION] Error occurs with source image channels.\n"
		destinationImage = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC1, cv::Scalar(0));
		std::cout << "Gray\n";
		mode = 1;
	}

	// Ảnh 3 kênh màu
	else if (sourceImageChannels == 3) {
		destinationImage = cv::Mat(heigthSourceImage, widthSourceImage, CV_8UC3, cv::Scalar(0));
		std::cout << "Color\n";
		mode = 3;
	}

	else {
		std::cout << "[EXCEPTION] Error occurs with source image channels.\n";
		return 1;
	}

	

	// Switch method
	switch (method) {
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

		std::cout << "Set kernel...\n";
		convolution.SetKernel(mean_kernel, kWidth, kHeight);
		std::cout << "Starting convolution...\n";
		if (mode == 1) {
			convolution.DoConvolution(sourceImage, destinationImage);
			std::cout << "End convolution...\n";
		}
		
		if (mode == 3) {
			convolution.DoConvolutionColor(sourceImage, destinationImage);
			std::cout << "End convolution...\n";
		}
		break;
	}
	case 1: // Lọc Trung Vị
	{
		std::cout << "Blur method starting: Median\n";

		
		int kernelCenterWidth = kWidth >> 1;
		int kernelCenterHeight = kHeight >> 1;

		

		uchar* p = (uchar*)sourceImage.data;

		if (mode == 1) {

			std::vector<int> offsets;
			std::cout << "Calculating kernel center...\n";
			for (int y = -kernelCenterHeight; y <= kernelCenterHeight; y++)
			{
				for (int x = -kernelCenterWidth; x <= kernelCenterWidth; x++)
				{
					offsets.push_back(y* sourceWidthStep + x);
				}
			}
			std::cout << "Bluring...\n";
			for (int i = 0; i < destinationImage.rows; i++) {
				uchar* dataRow = destinationImage.ptr<uchar>(i);
				for (int j = 0; j < destinationImage.cols; j++) {
					int i_source = i + (kWidth / 2), j_source = j + (kHeight / 2);
					uchar* pSource = p + ((long long)i_source * sourceWidthStep + (long long)j_source * sourceImageChannels);

					std::vector<uchar> value;
					for (int k = 0; k < offsets.size(); k++) {
						value.push_back(pSource[offsets[k]]);
					}

					sort(value.begin(), value.begin() + value.size());
					dataRow[j] = cv::saturate_cast<uchar>(value[value.size() / 2 + 1]);
				}
			}

			std::cout << "Finished Bluring...\n";
		}


		if (mode == 3) {
			std::cout << "Enable mode for color image...\n";
			std::vector<std::vector<int>> offsets(sourceImageChannels);
			std::cout << "Calculating kernel center...\n";
			for (int y = -kernelCenterHeight; y <= kernelCenterHeight; y++)
			{
				for (int x = -kernelCenterWidth; x <= kernelCenterWidth; x++)
				{

					for (int channel = 0; channel < sourceImageChannels; channel++)
					{
						offsets[channel].push_back(y * sourceWidthStep + x * sourceImageChannels * channel);
					}
				}
			}

			
			std::cout << "Bluring...\n";
			uchar* pSrcData = sourceImage.data;
			uchar* pDstData = destinationImage.data;

			for (int y = 0; y < heigthSourceImage; y++, pDstData += destinationImage.step[0]) {
				uchar* pDstRow = pDstData;

				for (int x = 0; x < widthSourceImage; x++, pDstRow += destinationImage.channels()) {
					int i_source = y + (kWidth / 2), j_source = x + (kHeight / 2);
					uchar* pSource = pSrcData + ((long long)i_source * sourceWidthStep + (long long)j_source * sourceImageChannels);

					std::vector<uchar> blue;
					std::vector<uchar> green;
					std::vector<uchar> red;


					for (int k = 0; k < offsets.size(); k++) {
						blue.push_back((pSource + offsets[0][k])[0]);
						green.push_back((pSource + offsets[1][k])[1]);
						red.push_back((pSource + offsets[2][k])[2]);
					}

					sort(blue.begin(), blue.begin() + blue.size());
					sort(green.begin(), green.begin() + green.size());
					sort(red.begin(), red.begin() + red.size());
					pDstRow[0] = cv::saturate_cast<uchar>(blue[blue.size() / 2 + 1]);
					pDstRow[1] = cv::saturate_cast<uchar>(green[green.size() / 2 + 1]);
					pDstRow[2] = cv::saturate_cast<uchar>(red[red.size() / 2 + 1]);
				}
			}
			std::cout << "Finished Bluring...\n";
		}
		
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
		if (mode == 1) {
			convolution.DoConvolution(sourceImage, destinationImage);
			std::cout << "End convolution...\n";
		}

		if (mode == 3) {
			convolution.DoConvolutionColor(sourceImage, destinationImage);
			std::cout << "End convolution...\n";
		}
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

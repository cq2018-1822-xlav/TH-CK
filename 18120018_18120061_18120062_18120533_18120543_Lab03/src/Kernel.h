#pragma once
#ifndef KERNEL_H
#define KERNEL_H

#include "CV_Include.h"
const int DEFAULT_SIGMA_X = 1;
const int DEFAULT_SIGMA_Y = 2;

void displayKernel(const std::vector<float>& v);
std::vector<float> getGaussianKernel(int rows, int cols, double sigmax, double sigmay);

#endif // KERNEL_H
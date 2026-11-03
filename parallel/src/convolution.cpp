#include "../include/convolution.hpp"

Convolution::Convolution(int kernelSize) : kernelSize_(kernelSize)
{
  kernel_.resize(kernelSize_, std::vector<float>(kernelSize_, 0.0f));
}

void Convolution::createGaussianKernel(const int size, const float sigma)
{
  if (size % 2 == 0)
  {
    throw std::invalid_argument("Kernel size must be an odd number");
  }

  int radius = size / 2;
  double sum = 0.0;
  double s = 2.0 * sigma * sigma;

  for (int x = -radius; x <= radius; ++x)
  {
    for (int y = -radius; y <= radius; ++y)
    {
      double r = x * x + y * y;
      kernel_[x + radius][y + radius] = std::exp(-r / s) / (M_PI * s);
      sum += kernel_[x + radius][y + radius];
    }
  }

  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      kernel_[i][j] /= sum;
    }
  }
}

void Convolution::createVerticalEdgeKernel(const int size)
{
  if (size % 2 == 0)
  {
    throw std::invalid_argument("Kernel size must be an odd number");
  }

  int radius = size / 2;

  for (int x = -radius; x <= radius; ++x)
  {
    for (int y = -radius; y <= radius; ++y)
    {
      kernel_[x + radius][y + radius] = (y == 0) ? 1.0f : 0.0f;
    }
  }
}

void Convolution::applyConvolution(const std::vector<std::vector<unsigned char>>& input, std::vector<std::vector<unsigned char>>& output)
{
  for (int i = 0; i < input.size(); ++i)
  {
    for (int j = 0; j < input[0].size(); ++j)
    {
      float sum = 0.0f;

      for (int k = 0; k < kernelSize_; ++k)
      {
        for (int l = 0; l < kernelSize_; ++l)
        {
          int x = i + k - kernelSize_ / 2;
          int y = j + l - kernelSize_ / 2;

          if (x >= 0 && x < input.size() && y >= 0 && y < input[0].size())
          {
            sum += input[x][y] * kernel_[k][l];
          }
        }
      }

      output[i][j] = static_cast<unsigned char>(std::round(sum));
    }
  }
}

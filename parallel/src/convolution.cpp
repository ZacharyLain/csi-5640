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
  int width = input[0].size();
  int height = input.size();

  // Loop through each pixel in the input image starting from the center of the kernel
  // Padding is not applied, so the first pixel/last pixels used will be (kernelSize_ / 2) away from the edge
  #pragma acc seq
  for (int pix_x = 0; pix_x < width - kernelSize_ + 1; ++pix_x)
  {
    for (int pix_y = 0; pix_y < height - kernelSize_ + 1; ++pix_y)
    {
      float sum = 0.0f;

      // For each pixel in the image, apply the convolution kernel to compute the output value
      for (int kernel_x = 0; kernel_x < kernelSize_; ++kernel_x)
      {
        for (int kernel_y = 0; kernel_y < kernelSize_; ++kernel_y)
        {
          int row = pix_y + kernel_x;
          int col = pix_x + kernel_y;

          // Check if the coordinates are within the image boundaries
          // If coordinates are OOB they are skipped
          if (col >= 0 && col < width && row >= 0 && row < height)
          {
            sum += input[row][col] * kernel_[kernel_x][kernel_y];
          }
        }
      }

      // Place the computed value in the output image
      try
      {
        output[pix_y][pix_x] = static_cast<unsigned char>(std::round(sum));
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error assigning value to output at (" << pix_x << ", " << pix_y << "): " << e.what() << std::endl;
      }
    }
  }
}

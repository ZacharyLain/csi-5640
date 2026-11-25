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

void Convolution::applyConvolution(const unsigned char* input_image, unsigned char* output, const int width, const int height)
{
  const int output_width = width - kernelSize_ + 1;
  const int output_height = height - kernelSize_ + 1;
  const int kernel_size = kernelSize_;

  float* flat_kernel = new float[kernel_size * kernel_size];

  // Flatten kernel for memory safety
  for (int i = 0; i < kernel_size; i++)
  {
    for (int j = 0; j < kernel_size; j++)
    {
      flat_kernel[i * kernel_size + j] = kernel_[i][j];
    }
  }

  // Loop through each pixel in the input image starting from the center of the kernel
  // Padding is not applied, so the first pixel/last pixels used will be (kernelSize_ / 2) away from the edge
  for (int pix_row = 0; pix_row < output_height; ++pix_row)
  {
    for (int pix_col = 0; pix_col < output_width; ++pix_col)
    {
      float sum = 0.0f;

      // For each pixel in the image, apply the convolution kernel to compute the output value
      for (int kernel_row = 0; kernel_row < kernel_size; ++kernel_row)
      {
        for (int kernel_col = 0; kernel_col < kernel_size; ++kernel_col)
        {
          int row = pix_row + kernel_row;
          int col = pix_col + kernel_col;

          // Check if the coordinates are within the image boundaries
          // If coordinates are OOB they are skipped
          if (col >= 0 && col < width && row >= 0 && row < height)
          {
            unsigned char input_pix = input_image[(width * row) + col];
            sum += input_pix * flat_kernel[(kernel_row * kernel_size) + kernel_col];
          }
        }
      }

      // Place the computed value in the output image
      output[(pix_row * output_width) + pix_col] = static_cast<unsigned char>(static_cast<int>(sum + 0.5f));
    }
  }

  delete[] flat_kernel;
  flat_kernel = nullptr;
}

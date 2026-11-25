#include "../include/convolution.hpp"
#include "../include/utilities.hpp"

#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

#include <string>
#include <chrono>

// Got images here: https://fastly.picsum.photos/id/237/128/128.jpg?grayscale&hmac=8Ye1hFnCaU6mahalA5Vz9WKPNGPvYk59irm670tGrzk

void printHelp(const char* programName)
{
  std::cout << "Usage: " << programName << " [OPTIONS]\n\n"
            << "Options:\n"
            << "  -h, --help                   Show this help message and exit\n"
            << "  -k, --kernel_size <int>      Set the kernel size (Must be odd!) (default: 5)\n"
            << "  -s, --sigma <float>          Change the sigma for the Gaussian function (default: 1.0)\n"
            << "  -i, --image_name <string>    Adjust the image name to run the convolution on a non-default image (default: grayscale_dog_128x128)"
            << std::endl;
}

int main(int argc, char* argv[]) {
  int kernelSize = 5;
  float sigma = 1.0f;
  std::string imageName = "grayscale_dog_128x128";

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help")
    {
      printHelp(argv[0]);
      return 0;
    }
    else if (arg == "-k" || arg == "--kernel_size")
    {
      if (i + 1 < argc) {
        kernelSize = std::stoi(argv[++i]); // Advance i and convert string to int
      } else {
          std::cerr << "Error: " << arg << " requires a kernel size.\n";
          return 1;
      }
    }
    else if (arg == "-s" || arg == "--sigma")
    {
      if (i + 1 < argc) {
        sigma = std::stof(argv[++i]); // Advance i and convert string to float
      } else {
          std::cerr << "Error: " << arg << " requires a sigma.\n";
          return 1;
      }
    }
    else if (arg == "-i" || arg == "--image_name")
    {
      if (i + 1 < argc) {
        imageName = argv[++i]; // Advance i
      } else {
          std::cerr << "Error: " << arg << " requires an image name.\n";
          return 1;
      }
    }
  }

  const std::string& baseFilePath = "./resources/" + imageName + ".jpg";

  std::cout << "Using kernel with size: " << kernelSize << " and sigma: " << sigma << std::endl;

  unsigned char* stbi_image = nullptr;
  unsigned char* output_image = nullptr;

  utilities::loadImage(baseFilePath, stbi_image, true);

  int width, height, channels;
  stbi_info(baseFilePath.c_str(), &width, &height, &channels);

  channels = 1;

  Convolution convolution = Convolution(kernelSize);
  convolution.createGaussianKernel(kernelSize, sigma);

  int outputWidth = width - kernelSize + 1;
  int outputHeight = height - kernelSize + 1;

  // Allocate memory for the output image
  output_image = new unsigned char[outputWidth * outputHeight * channels];

  // Start timing the convolution operation
  auto start = std::chrono::high_resolution_clock::now();
  convolution.applyConvolution(stbi_image, output_image, width, height);
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "Convolution operation completed." << std::endl;

  utilities::saveImage("./parallel/outputs/" + imageName + "_blurred.jpg", output_image, outputWidth, outputHeight, 1);

  // Free memory
  stbi_image_free(stbi_image);
  delete[] output_image;
  output_image = nullptr;

  // Calculate the duration of the convolution operation
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Parallel convolution operation took " << duration.count() << " milliseconds." << std::endl;

  return 0;
}
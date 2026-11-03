#include "convolution.hpp"
#include "utilities.cpp"

#include <string>
#include <chrono>

// Got images here: https://fastly.picsum.photos/id/237/128/128.jpg?grayscale&hmac=8Ye1hFnCaU6mahalA5Vz9WKPNGPvYk59irm670tGrzk

int main(int argc, char* argv[]) {
  int kernelSize = argc > 1 ? std::stoi(argv[1]) : 5;
  int sigma = argc > 2 ? std::stof(argv[2]) : 1.0f;
  const std::string& imageName = argc > 3 ? argv[3] : "grayscale_dog_128x128";
  const std::string& baseFilePath = argc > 3 ? "./resources/" + imageName + ".jpg" : "./resources/grayscale_dog_128x128.jpg";

  std::cout << "Using kernel with size: " << kernelSize << " and sigma: " << sigma << std::endl;

  unsigned char* stbi_image = nullptr;
  unsigned char* blurred_image = nullptr;

  utilities::loadImage(baseFilePath, stbi_image, false);

  int width, height, channels;
  stbi_info(baseFilePath.c_str(), &width, &height, &channels);

  std::cout << "Loaded image with a width of " <<  width << "px, a height of " << height << "px, and " << channels << " channels." << std::endl;

  auto reshaped_image = utilities::reshapeArrayTwoDim(stbi_image, width, height, channels);

  Convolution convolution = Convolution(kernelSize);
  convolution.createVerticalEdgeKernel();
  std::vector<std::vector<unsigned char>> output(width, std::vector<unsigned char>(height));

  // Start timing the convolution operation
  auto start = std::chrono::high_resolution_clock::now();
  convolution.applyConvolution(reshaped_image, output);
  auto end = std::chrono::high_resolution_clock::now();

  utilities::flattenArray(output, blurred_image, width, height, channels);

  utilities::saveImage("./resources/" + imageName + "_blurred.jpg", blurred_image, width, height, channels);

  // Calculate the duration of the convolution operation
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Convolution operation took " << duration.count() << " milliseconds." << std::endl;

  return 0;
}
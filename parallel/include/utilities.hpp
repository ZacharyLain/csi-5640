#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>

namespace utilities {
  void loadImage(const std::string& filename, unsigned char*& image, const bool grayscale = true);

  std::vector<std::vector<unsigned char>> reshapeArrayTwoDim(unsigned char* image, const int width, const int height, const int channels);

  void flattenArray(std::vector<std::vector<unsigned char>>& twoDimImage, unsigned char*& image, const int width, const int height, const int channels);

  void saveImage(const std::string& filename, const unsigned char* image, const int width, const int height, const int channels);
}

#endif // UTILITIES_HPP_
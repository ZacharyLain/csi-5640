#include "../include/utilities.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

namespace utilities {
  void loadImage(const std::string& filename, unsigned char*& image, const bool grayscale) {
    std::cout << "Loading image from " << filename << std::endl;

    int width, height, channels;
    image = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (image == NULL)
    {
      std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
      throw std::runtime_error("Failed to load image");
    }


    // Convert to grayscale
    try
    {
      if (grayscale)
      {
        int gray_channels = channels == 4 ? 2 : 1;
        size_t gray_image_size = width * height * gray_channels;

        unsigned char* gray_image = new unsigned char[gray_image_size];

        if (gray_image == NULL)
        {
          stbi_image_free(image);
          throw std::runtime_error("Failed to allocate memory for grayscale image");
        }

        for(unsigned char *pixel = image, *pixel_gray = gray_image; pixel != image + width * height * channels; pixel += channels, pixel_gray += gray_channels) {
          *pixel_gray = (uint8_t)((*pixel + *(pixel + 1) + *(pixel + 2))/3.0);
          
          if(channels == 4) {
            *(pixel_gray + 1) = *(pixel + 3);
          }
        }

        stbi_image_free(image);
        image = gray_image;
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error converting image to grayscale: " << e.what() << std::endl;
      throw std::runtime_error("Failed to convert image to grayscale");
    }
  }

  std::vector<std::vector<unsigned char>> reshapeArrayTwoDim(unsigned char* image, const int width, const int height, const int channels)
  {
    std::vector<std::vector<unsigned char>> reshaped(height, std::vector<unsigned char>(width));

    for(unsigned char *p = image; p != image + width * height * channels; p += channels) {
      reshaped[((p - image) / channels) / width][((p - image) / channels) % width] = *p;
    }

    return reshaped;
  }

  void flattenArray(std::vector<std::vector<unsigned char>>& twoDimImage, unsigned char*& image, const int width, const int height, const int channels)
  {
    try
    {
      image = new unsigned char[width * height * channels];
    }
    catch (const std::bad_alloc& e)
    {
      std::cerr << "Error allocating memory for flattened image: " << e.what() << std::endl;
      throw std::runtime_error("Failed to allocate memory for flattened image");
    }

    try
    {
      for (int i = 0; i < height; ++i)
      {
        for (int j = 0; j < width; ++j)
        {
          image[i * width + j] = twoDimImage[i][j];
        }
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error flattening array: " << e.what() << std::endl;
      throw std::runtime_error("Failed to flatten array");
    }
  }

  void saveImage(const std::string& filename, const unsigned char* image, const int width, const int height, const int channels)
  {
    try
    {
      int success = stbi_write_jpg(filename.c_str(), width, height, channels, image, 100);
      
      if (!success)
      {
        std::cerr << "Error saving image to " << filename << std::endl;
        throw std::runtime_error("Failed to save image");
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error saving image to " << filename << ": " << e.what() << std::endl;
      throw std::runtime_error("Failed to save image");
    }
  }
}
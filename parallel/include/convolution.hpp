#ifndef CONVOLUTION_HPP_
#define CONVOLUTION_HPP_

#include <iostream>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Convolution {
  public:
    Convolution(int kernelSize = 5);

    void createGaussianKernel(const int size = 5, const float sigma = 1.0f);
    void createVerticalEdgeKernel(const int size = 3);

    void applyConvolution(
      const std::vector<std::vector<unsigned char>>& input,
      std::vector<std::vector<unsigned char>>& output
    );

  private:
    std::vector<std::vector<float>> kernel_;
    int kernelSize_;

};

#endif  // CONVOLUTION_HPP_
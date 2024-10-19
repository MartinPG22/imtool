//
// Created by mapor on 19/10/2024.
//

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <cstdint>

struct Pixel {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

struct ImageAOS {
  std::vector<Pixel> pixels;

  ImageAOS(size_t width, size_t height) {
    pixels.resize(width * height);
  }
};
#endif //IMAGE_HPP

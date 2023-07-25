//
// Created by Bear on 2023/7/17.
//
#include "film.h"

#include <stb_image_write.h>
#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_IMPLEMENTATION
#define TINYEXR_USE_THREAD 1
#include <tinyexr.h>

namespace beart {
template<>
bool Film<RGBSpectrum>::Save(const std::string &filename) const {
  // get file extension
  auto extension = filename.find_last_of(".") != std::string::npos
                   ? filename.substr(filename.find_last_of(".") + 1)
                   : "";
  if (extension.empty()) {
    return false;
  }
  auto N = spectrum_t{}.size();
  if (extension == "hdr") {
    // convert floating-point image to 32-bit per channel
    std::vector<float> data(width_ * height_
                                * N, 0);
    for (auto y = 0u; y < height_; ++y) {
      for (auto x = 0u; x < width_; ++x) {
        unsigned int pixel_offset = N * (y * width_ + x);
        auto cc = buffer_[y * width_ + x];

        data[pixel_offset + 0] = cc.data()[0];
        data[pixel_offset + 1] = cc.data()[1];
        data[pixel_offset + 2] = cc.data()[2];
      }
    }
    return stbi_write_hdr(filename.c_str(), width_, height_, N,
                          &data[0]) != 0;
  }
  else if (extension == "exr") {
    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = N;

    // init channels
    std::vector<float> images[N];
    for (auto i = 0u; i < N; ++i) {
      images[i].resize(width_ * height_);
      for (auto j = 0u; j < height_; ++j) {
        for (auto k = 0u; k < width_; ++k) {
          images[i][j * width_ + k] = buffer_[j * width_ + k].data()[i];
        }
      }
    }

    float *image_ptr[N];
    // first 3 channels are in BGR order, then A
    for (auto i = 0u; i < 3; ++i) {
      image_ptr[i] = &(images[2 - i].at(0));
    }
    if (N == 4) {
      image_ptr[3] = &(images[3].at(0)); // A
    }

    image.images = (uint8_t **) image_ptr;
    image.width = width_;
    image.height = height_;

    header.num_channels = N;
    header.channels = (EXRChannelInfo *) malloc(sizeof(EXRChannelInfo) * header.num_channels);
    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
    const char *chan_names[4] = {"B", "G", "R", "A"};
    for (auto i = 0u; i < N; ++i) {
      strncpy(header.channels[i].name, chan_names[i], 255);
    }
    header.compression_type = TINYEXR_COMPRESSIONTYPE_PIZ;
    header.pixel_types = (int *) malloc(sizeof(int) * header.num_channels);
    header.requested_pixel_types = (int *) malloc(sizeof(int) * header.num_channels);
    for (int i = 0; i < header.num_channels; i++) {
      header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
      header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
    }

    // add custom comment attribute
    header.num_custom_attributes = 1;
    header.custom_attributes = static_cast<EXRAttribute *>(malloc(sizeof(EXRAttribute)));

    strncpy(header.custom_attributes[0].name, "comments", 255);
    strncpy(header.custom_attributes[0].type, "string", 255);
    char comment[] = "Generated with darts";
    header.custom_attributes[0].value = reinterpret_cast<uint8_t *>(&comment);
    header.custom_attributes[0].size = strlen(comment);

    const char *err;
    int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
      std::cerr << "Error saving EXR image: {}";
      return false;
    }
    free(header.channels);
    free(header.pixel_types);
    free(header.requested_pixel_types);
    free(header.custom_attributes);
    return true;
  }
  // convert floating-point image to 8-bit per channel
  std::vector<uint8_t> data(width_ * height_
                                * N, 0);
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      unsigned int pixel_offset = N * (y * width_ + x);
      auto cc = Clamp(LinearRGBTosRGB(buffer_[y * width_ + x]), 0.f, 1.f) * 255.f;

      data[pixel_offset + 0] = static_cast<uint8_t>(cc.data()[0]);
      data[pixel_offset + 1] = static_cast<uint8_t>(cc.data()[1]);
      data[pixel_offset + 2] = static_cast<uint8_t>(cc.data()[2]);
    }
  }
  if (extension == "png") {
    return stbi_write_png(filename.c_str(), width_, height_, N, &data[0],
                          sizeof(uint8_t) * width_ * N) != 0;
  } else if (extension == "jpg" || extension == "jpeg") {
    return stbi_write_jpg(filename.c_str(), width_, height_, N, &data[0], 100) != 0;
  } else if (extension == "bmp") {
    return stbi_write_bmp(filename.c_str(), width_, height_, N, &data[0]) != 0;
  } else if (extension == "tga") {
    return stbi_write_tga(filename.c_str(), width_, height_, N, &data[0]) != 0;
  }
}
}
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <iostream>
#include <cstdint>
#include <vector>

static uint16_t rgba_to_argb1555(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return
    ((a > 127 ? 1 : 0) << 15) |
    ((r & 0xF8) << 7) |
    ((g & 0xF8) << 2) |
    ((b & 0xF8) >> 3);
}

void compile_to_binary(const std::string &file, const std::string &format) {
  int32_t w {};
  int32_t h {};
  int32_t c {};

  int32_t channel {4};

  std::string input_path {"./imgs/" + file + format};

  unsigned char *p_raw {
    stbi_load(
      input_path.c_str(),
      &w,
      &h,
      &c,
      channel
    )
  };

  if (p_raw == nullptr) {
    std::cout << "-+-+ error = could not open IMAGE!\n " << stbi_failure_reason() << std::endl;
    return;
  }

  std::string output {
    "./bin/" + file + ".bin"
  };

  FILE *pf {fopen(output.c_str(), "wb")};
  if (pf == nullptr) {
    std::cout << "-+-+ error = could not open output file to write!" << std::endl;
    stbi_image_free(p_raw);
    return;
  }

  std::vector<uint16_t> out{};
  out.resize(static_cast<size_t>(w) * h);

  for (int32_t i = 0; i < w * h; i++) {
    uint8_t r = p_raw[i * 4 + 0];
    uint8_t g = p_raw[i * 4 + 1];
    uint8_t b = p_raw[i * 4 + 2];
    uint8_t a = p_raw[i * 4 + 3];

    out[i] = rgba_to_argb1555(r, g, b, a);
  }

  uint16_t w16 = static_cast<uint16_t>(w);
  uint16_t h16 = static_cast<uint16_t>(h);
  uint8_t c8 = 2;

  fwrite(&w16, sizeof(w16), 1, pf);
  fwrite(&h16, sizeof(h16), 1, pf);
  fwrite(&c8, sizeof(c8), 1, pf);

  size_t size_bytes = static_cast<size_t>(w) * h * 2;
  size_t written = fwrite(out.data(), 1, size_bytes, pf);

  if (written != size_bytes) {
    std::cout << "-+-+ error = write size mismatch!" << std::endl;
  }

  fclose(pf);
  stbi_image_free(p_raw);
}

int32_t main(int32_t, char**) {
  std::cout << "-- Compiling imgs to binary flash..." << std::endl;

  compile_to_binary("atlas", ".png");

  return 0;
}

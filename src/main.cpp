#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <iostream>
#include <cstdint>

void compile_to_binary(const std::string &file, const std::string &format) {
  int32_t w {};
  int32_t h {};
  int32_t c {};

  int32_t channel {1};

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

  size_t size {static_cast<size_t>(w * h * channel)};

  uint16_t w16 {static_cast<uint16_t>(w)};
  uint16_t h16 {static_cast<uint16_t>(h)};
  uint8_t c8 {static_cast<uint8_t>(channel)};

  fwrite(&w16, sizeof(w16), 1, pf);
  fwrite(&h16, sizeof(h16), 1, pf);
  fwrite(&c8, sizeof(c8), 1, pf);

  size_t written {fwrite(p_raw, 1, size, pf)};

  if (written != size) {
    std::cout << "-+-+ error = write size and size does not match!" << std::endl;
  }

  fclose(pf);
  stbi_image_free(p_raw);
}

int32_t main(int32_t, char**) {
  std::cout << "-- Compiling imgs to binary flash..." << std::endl;

  compile_to_binary("atlas", ".png");

  return 0;
}

#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>

std::string ReadEnhancementAlgorithm() {
  std::string algorithm{};
  std::getline(std::cin, algorithm);
  return algorithm;
}

std::vector<std::vector<bool>> ReadInputImage() {
  std::vector<std::vector<bool>> image{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    std::vector<bool> row{};
    row.reserve(line.size());
    for (const char c : line) {
      row.push_back(c == '#');
    }
    image.push_back(std::move(row));
  }
  return image;
}

std::pair<std::size_t, std::size_t> OutputSize(std::size_t input_height, std::size_t input_width,
                                               std::size_t kernel_size, std::size_t padding) {
  const std::size_t output_height = (input_height + 2 * padding - kernel_size) / 1 + 1;
  const std::size_t output_width = (input_width + 2 * padding - kernel_size) / 1 + 1;
  return {output_height, output_width};
}

bool OutputPixel(const std::bitset<9>& binary, std::string_view enhancement_algorithm) {
  const auto index = binary.to_ulong();
  return enhancement_algorithm[index] == '#';
}

std::vector<std::vector<bool>> Convolution(const std::vector<std::vector<bool>>& image,
                                           std::string_view enhancement_algorithm, bool infinity) {
  constexpr std::size_t kernel_size = 3;
  constexpr std::size_t padding = 2;
  const std::size_t input_height = image.size();
  const std::size_t input_width = image.front().size();

  const auto [output_height, output_width] = OutputSize(input_height, input_width, kernel_size, padding);
  std::vector<std::vector<bool>> output(output_height, std::vector<bool>(output_width, infinity));

  auto LookupInputImage = [&](std::int32_t r, std::int32_t c) -> bool {
    if (r < 0 || c < 0 || input_height <= r || input_width <= c)
      return infinity;
    return image[r][c];
  };

  for (std::int32_t kernel_center_r = -1; kernel_center_r <= static_cast<std::int32_t>(input_height);
       ++kernel_center_r) {
    for (std::int32_t kernel_center_c = -1; kernel_center_c <= static_cast<std::int32_t>(input_width);
         ++kernel_center_c) {
      std::bitset<9> binary{};
      binary[8] = LookupInputImage(kernel_center_r - 1, kernel_center_c - 1);
      binary[7] = LookupInputImage(kernel_center_r - 1, kernel_center_c);
      binary[6] = LookupInputImage(kernel_center_r - 1, kernel_center_c + 1);
      binary[5] = LookupInputImage(kernel_center_r, kernel_center_c - 1);
      binary[4] = LookupInputImage(kernel_center_r, kernel_center_c);
      binary[3] = LookupInputImage(kernel_center_r, kernel_center_c + 1);
      binary[2] = LookupInputImage(kernel_center_r + 1, kernel_center_c - 1);
      binary[1] = LookupInputImage(kernel_center_r + 1, kernel_center_c);
      binary[0] = LookupInputImage(kernel_center_r + 1, kernel_center_c + 1);
      output[kernel_center_r + 1][kernel_center_c + 1] = OutputPixel(binary, enhancement_algorithm);
    }
  }
  return output;
}

std::vector<std::vector<bool>> ConvolutionTwice(const std::vector<std::vector<bool>>& image,
                                                std::string_view enhancement_algorithm) {
  const bool infinity = enhancement_algorithm[0] == '#';
  const auto intermediate = Convolution(image, enhancement_algorithm, !infinity);
  return Convolution(intermediate, enhancement_algorithm, infinity);
}

std::int32_t CountLightPixels(const std::vector<std::vector<bool>>& image) {
  std::int32_t count{0};
  for (const auto& row : image) {
    for (const auto& pixel : row) {
      if (pixel) {
        ++count;
      }
    }
  }
  return count;
}

int main() {
  const auto enhancement_algorithm = ReadEnhancementAlgorithm();
  std::cin.ignore();
  auto image = ReadInputImage();
  for (std::size_t i = 1; i <= 25; ++i) {
    image = ConvolutionTwice(image, enhancement_algorithm);
  }
  std::cout << CountLightPixels(image) << '\n';
}

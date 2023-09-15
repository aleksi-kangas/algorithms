#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

// On some platforms, need to redirect input from a file:
// ./<executable_name> < <input_file_name>, e.g. ./06_1_tuning_trouble < in.txt

int main() {
  constexpr std::size_t kMarkerSize{4};
  std::string signal{};
  std::cin >> signal;
  std::unordered_map<char, std::size_t> window{};
  std::size_t l{0};
  for (std::size_t r = 0; r < signal.size(); ++r) {
    ++window[signal[r]];
    if (window.size() == kMarkerSize) {
      std::cout << r + 1 << std::endl;
      return 0;
    }
    while (r - l + 1 >= kMarkerSize) {
      window[signal[l]]--;
      if (window[signal[l]] == 0) {
        window.erase(signal[l]);
      }
      ++l;
    }
  }
}

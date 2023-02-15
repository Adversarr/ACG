#include "autils/raw_fileio.hpp"
#include <fstream>

#include "autils/log.hpp"
namespace acg::utils::io {
std::vector<char> read_binary_file(std::string path) {
  std::ifstream input_file{path, std::ios::ate | std::ios::binary};
  ACG_CHECK(input_file.is_open(), "Failed to open file: {}", path);
  size_t buffer_size = input_file.tellg();

  input_file.seekg(0);
  std::vector<char> buffer(buffer_size);
  input_file.read(buffer.data(), buffer_size);
  ACG_CHECK(input_file, "Failed to read from file: {}", path);

  input_file.close();  // (optional) Explicitly close input file
  return buffer;
}
}

#include "file_loader.hh"

#include <fmt/core.h>

#include <cerrno>
#include <cstdio>
#include <stdexcept>

namespace Cougar::Utils {

static constexpr std::size_t CHUNK_SIZE = 1000 * 1024;

void FileLoader::load(const std::string &path) {
  clear();

  FILE *f = std::fopen(path.c_str(), "r");
  if (!f) {
    throw std::runtime_error(fmt::format("Error opening file '{}' : {}", path,
                                         std::strerror(errno)));
  }

  while (true) {

    chunk next = std::make_unique<std::byte[]>(CHUNK_SIZE);

    std::size_t r = std::fread(next.get(), 1, CHUNK_SIZE, f);
    if (r == 0)
      break;

    mSize += r;
    mChunks.push_back(std::move(next));
  }

  std::fclose(f);
}

void FileLoader::clear() {
  mChunks.clear();
  mSize = 0;
}

void FileLoader::copyTo(std::byte *out) const {
  std::size_t size = mSize;

  auto it = mChunks.begin();

  while (size > 0) {
    auto chunkSize = std::min(size, CHUNK_SIZE);
    std::memcpy(out, it->get(), chunkSize);
    out += chunkSize;
    size -= chunkSize;
    ++it;
  }
}

} // namespace Cougar::Utils
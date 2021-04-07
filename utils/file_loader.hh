#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Cougar::Utils {

// Loads and keeps file in memory.
// File is storewe in chunks, use copyData to extract contigous data.
class FileLoader {
public:
  // loads file from disk into buffer
  void load(const std::string &path);

  // clears content and releases all memory
  void clear();

  // get number of bytes loaded
  std::size_t getSize() const { return mSize; }

  // copies all bytes into contigous block.
  // The block must have sufficient capacity, at least the number returned by
  // getSize()
  void copyTo(std::byte *out) const;

private:
  using chunk = std::unique_ptr<std::byte[]>;

  std::size_t mSize = 0;
  std::vector<chunk> mChunks;
};

} // namespace Cougar::Utils
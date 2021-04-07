#pragma once

#include <cstddef>
#include <forward_list>
#include <memory>
#include <type_traits>

namespace Cougar::Utils {

// A simple memory zone allocator.
// Provides stretchable memory block in which strings and simple objects can be
// stored Allows for clearing all memory at once, and provides a convenient
// point to inject any memory monitoring/limiting.
//
// During compilation, mane small objects will be created, and needed troughout
// the entire process.
//
// As the memory is freed all at once, w/o calling destructors, only trivially
// constructuble objects can be allocated.
class ZoneAllocator {
public:
  // Allocaste space and create object in allocated space
  template <typename T, typename... Args> T *allocate(Args &&...args) {
    static_assert(std::is_trivially_destructible_v<T>,
                  "Zone allocator can only hold trivially destructible types");
    void *addr = doAllocate(sizeof(T), alignof(T));
    return new (addr) T(std::forward<Args>(args)...);
  }

  // Allocate uninitialized block
  std::byte *allocateBlock(std::size_t size, std::size_t alignment = 1) {
    return (std::byte *)doAllocate(size, alignment);
  }

  // usage info

  // total of all requested allocations
  std::size_t getAllocatedBytes() const { return mAllocated; }

  // total of all blocks actually allocated
  std::size_t getUsedBytes() const { return mUsed; }

private:
  struct Block {
    void *mData = nullptr;
    void *mCurrent = nullptr;
    std::size_t mSpace = 0;

    ~Block();
  };

  void *doAllocate(std::size_t size, std::size_t alignment);
  void *allocateInNewBlock(std::size_t size, std::size_t alignment);

  std::forward_list<Block> mBlocks;
  std::size_t mUsed = 0;
  std::size_t mAllocated = 0;
};

} // namespace Cougar::Utils
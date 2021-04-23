#pragma once

#include "zone_allocator.hh"

namespace Cougar::Utils {

template <typename KeyT, typename ValueT> class Map {

  struct TreeNode {

    template <typename... Args>
    TreeNode(const KeyT &key, Args &&...valueConstructorArgs)
        : mKey(key), mValue(std::forward<Args>(valueConstructorArgs)...) {}

    KeyT mKey;
    ValueT mValue;
    TreeNode *mLeft = nullptr;
    TreeNode *mRight = nullptr;
  };

public:
  // Returns pointer to created element,
  // Throws if element byu that key already exists
  template <typename KeyCompatT, typename... Args>
  ValueT *emplace(const KeyCompatT &key, Args &&...valueConstructorArgs) {
    return doEmplace(mRoot, key, std::forward<Args>(valueConstructorArgs)...);
  }

private:
  template <typename KeyCompatT, typename... Args>
  ValueT *doEmplace(TreeNode *&parent, const KeyCompatT &key,
                    Args &&...valueConstructorArgs) {
    if (parent == nullptr) {
      // here we go
      parent = Zone::make<TreeNode>(
          key, std::forward<Args>(valueConstructorArgs)...);
      return &parent->mValue;
    }

    if (key < parent->mKey)
      return doEmplace(parent->mLeft, key,
                       std::forward<Args>(valueConstructorArgs)...);

    if (parent->mKey < key) {
      return doEmplace(parent->mRight, key,
                       std::forward<Args>(valueConstructorArgs)...);
    }

    throw std::runtime_error("Map: key already exists");
  }

  TreeNode *mRoot = nullptr;
};

} // namespace Cougar::Utils

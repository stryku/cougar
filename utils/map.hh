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
  // TODO: maybe return this instead of throwing?
  // struct EmplaceResult {
  //   ValueT *item;
  //   bool success;
  // };

  // Returns pointer to created element.
  // Throws if element byu that key already exists
  template <typename KeyCompatT, typename... Args>
  ValueT *emplace(const KeyCompatT &key, Args &&...valueConstructorArgs) {
    return doEmplace(mRoot, key, std::forward<Args>(valueConstructorArgs)...);
  }

  // Returns pointer to found value, or null if not found
  template <typename KeyCompatT> ValueT *find(const KeyCompatT &key) {
    return doFind(mRoot, key);
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

  template <typename KeyCompatT>
  ValueT *doFind(TreeNode *node, const KeyCompatT &key) {
    if (!node)
      return nullptr;

    if (key < node->mKey)
      return doFind(node->mLeft, key);

    if (node->mKey < key)
      return doFind(node->mRight, key);

    return &node->mValue;
  }

  TreeNode *mRoot = nullptr;
};

} // namespace Cougar::Utils

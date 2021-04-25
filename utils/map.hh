#pragma once

#include "zone_allocator.hh"

namespace Cougar::Utils {

template <typename KeyT, typename BaseValueT> class Map {

  template <typename ValueT> struct TreeNode {

    template <typename... Args>
    TreeNode(const KeyT &key, Args &&...valueConstructorArgs)
        : mKey(key), mValue(std::forward<Args>(valueConstructorArgs)...) {}

    KeyT mKey;
    TreeNode *mLeft = nullptr;
    TreeNode *mRight = nullptr;
    ValueT mValue;
  };

  using BaseTreeNode = TreeNode<BaseValueT>;

public:
  // TODO: maybe return this instead of throwing?
  // struct EmplaceResult {
  //   ValueT *item;
  //   bool success;
  // };

  // Returns pointer to created element.
  // Throws if element byu that key already exists
  template <typename KeyCompatT, typename... Args>
  BaseValueT *emplace(const KeyCompatT &key, Args &&...valueConstructorArgs) {
    return doEmplace<BaseValueT>(mRoot, key,
                                 std::forward<Args>(valueConstructorArgs)...);
  }

  template <typename ExtendedValueT, typename KeyCompatT, typename... Args>
  ExtendedValueT *emplace(const KeyCompatT &key,
                          Args &&...valueConstructorArgs) {
    static_assert(std::is_base_of_v<BaseValueT, ExtendedValueT>,
                  "emplaced type must be derived from base value type");

    return doEmplace<ExtendedValueT>(
        mRoot, key, std::forward<Args>(valueConstructorArgs)...);
  }

  // Returns pointer to found value, or null if not found
  template <typename KeyCompatT> BaseValueT *find(const KeyCompatT &key) {
    return doFind(mRoot, key);
  }

private:
  template <typename ValueT, typename KeyCompatT, typename... Args>
  ValueT *doEmplace(BaseTreeNode *&parent, const KeyCompatT &key,
                    Args &&...valueConstructorArgs) {
    if (parent == nullptr) {
      // here we go
      TreeNode<ValueT> *node = Zone::make<TreeNode<ValueT>>(
          key, std::forward<Args>(valueConstructorArgs)...);
      parent = reinterpret_cast<BaseTreeNode *>(node);
      return &node->mValue;
    }

    if (key < parent->mKey)
      return doEmplace<ValueT>(parent->mLeft, key,
                               std::forward<Args>(valueConstructorArgs)...);

    if (parent->mKey < key) {
      return doEmplace<ValueT>(parent->mRight, key,
                               std::forward<Args>(valueConstructorArgs)...);
    }

    throw std::runtime_error("Map: key already exists");
  }

  template <typename KeyCompatT>
  BaseValueT *doFind(BaseTreeNode *node, const KeyCompatT &key) {
    if (!node)
      return nullptr;

    if (key < node->mKey)
      return doFind(node->mLeft, key);

    if (node->mKey < key)
      return doFind(node->mRight, key);

    return &node->mValue;
  }

  BaseTreeNode *mRoot = nullptr;
};

} // namespace Cougar::Utils

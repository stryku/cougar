#pragma once

#include "zone_allocator.hh"

#include <cassert>

namespace Cougar::Utils {

template <typename NodeT> class ListIterator {
public:
  explicit ListIterator(NodeT *n) : mNode(n) {}

  auto &operator*() {
    assert(mNode);
    return mNode->mElement;
  };

  auto *operator->() {
    assert(mNode);
    return &mNode->mElement;
  }

  ListIterator &operator++() {
    assert(mNode);
    mNode = mNode->mNext;
    return *this;
  }

  bool operator!=(const ListIterator &o) const { return mNode != o.mNode; }
  bool operator==(const ListIterator &o) const { return mNode == o.mNode; }

private:
  NodeT *mNode = nullptr;
};

/// A linked list linving a zoned allocator
template <typename T> class List {
  struct ListNode;

public:
  using iterator = ListIterator<ListNode>;
  using const_iterator = ListIterator<const ListNode>;

  template <typename... Args> void emplace_back(Args &&...args) {
    ListNode *newNode = Zone::make<ListNode>(std::forward<Args>(args)...);
    if (!mFirst) {
      mFirst = newNode;
    } else {
      mLast->mNext = newNode;
    }
    mLast = newNode;
  }

  auto begin() { return ListIterator<ListNode>(mFirst); }
  auto end() { return ListIterator<ListNode>(nullptr); }

  auto begin() const { return ListIterator<const ListNode>(mFirst); }
  auto end() const { return ListIterator<const ListNode>(nullptr); }

  bool empty() const { return mFirst == nullptr; }

private:
  struct ListNode {

    template <typename... Args>
    ListNode(Args &&...args) : mElement(std::forward<Args>(args)...) {}

    T mElement;
    ListNode *mNext = nullptr;
  };

  ListNode *mFirst = nullptr;
  ListNode *mLast = nullptr;
};

} // namespace Cougar::Utils
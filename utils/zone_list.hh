#pragma once

#include "zone_allocator.hh"

#include <cassert>

namespace Cougar::Utils {

template <typename NodeT> class ZoneListIterator {
public:
  explicit ZoneListIterator(NodeT *n) : mNode(n) {}

  auto &operator*() {
    assert(mNode);
    return mNode->mElement;
  };

  ZoneListIterator &operator++() {
    assert(mNode);
    mNode = mNode->mNext;
    return *this;
  }

  bool operator!=(const ZoneListIterator &o) const { return mNode != o.mNode; }
  bool operator==(const ZoneListIterator &o) const { return mNode == o.mNode; }

private:
  NodeT *mNode = nullptr;
};

/// A linked list linving a zoned allocator
template <typename T> class ZoneList {
public:
  ZoneList(ZoneAllocator &zone) : mZone(zone) {}

  template <typename... Args> void emplace_back(Args &&...args) {
    ListNode *newNode = mZone.make<ListNode>(std::forward<Args>(args)...);
    if (!mFirst) {
      mFirst = newNode;
    } else {
      mLast->mNext = newNode;
    }
    mLast = newNode;
  }

  auto begin() { return ZoneListIterator<ListNode>(mFirst); }
  auto end() { return ZoneListIterator<ListNode>(nullptr); }

  auto begin() const { return ZoneListIterator<const ListNode>(mFirst); }
  auto end() const { return ZoneListIterator<const ListNode>(nullptr); }

private:
  struct ListNode {

    template <typename... Args>
    ListNode(Args &&...args) : mElement(std::forward<Args>(args)...) {}

    T mElement;
    ListNode *mNext = nullptr;
  };

  ListNode *mFirst = nullptr;
  ListNode *mLast = nullptr;
  ZoneAllocator &mZone;
};

} // namespace Cougar::Utils
#pragma once

#include "zone_allocator.hh"

#include <cassert>
#include <iterator>

namespace Cougar::Utils {

template <typename NodeT> class ListIterator {
public:
  explicit ListIterator(NodeT *n) : mNode(n) {}

  using iterator_category = std::forward_iterator_tag;
  using value_type = decltype(NodeT::mElement);
  using pointer = value_type *;
  using reference = value_type &;
  using difference_type = std::ptrdiff_t;

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

  ListIterator operator++(int) {
    assert(mNode);
    auto result = *this;
    mNode = mNode->mNext;
    return result;
  }

  bool operator!=(const ListIterator &o) const { return mNode != o.mNode; }
  bool operator==(const ListIterator &o) const { return mNode == o.mNode; }

private:
  NodeT *mNode = nullptr;
};

/// A linked list living a zoned allocator

template <typename T> class ListView {
protected:
  struct ListNode;

public:
  using iterator = ListIterator<ListNode>;
  using const_iterator = ListIterator<const ListNode>;

  auto begin() { return ListIterator<ListNode>(mFirst); }
  auto end() { return ListIterator<ListNode>(nullptr); }

  auto begin() const { return ListIterator<const ListNode>(mFirst); }
  auto end() const { return ListIterator<const ListNode>(nullptr); }

  bool empty() const { return mFirst == nullptr; }

protected:
  struct ListNode {

    template <typename... Args>
    ListNode(Args &&...args) : mElement(std::forward<Args>(args)...) {}

    T mElement;
    ListNode *mNext = nullptr;
  };

  ListNode *mFirst = nullptr;
  ListNode *mLast = nullptr;
};

template <typename T> class List final : public ListView<T> {
  using ListNode = typename ListView<T>::ListNode;

public:
  template <typename... Args> void emplace_back(Args &&...args) {
    ListNode *newNode = Zone::make<ListNode>(std::forward<Args>(args)...);
    if (!this->mFirst) {
      this->mFirst = newNode;
    } else {
      this->mLast->mNext = newNode;
    }
    this->mLast = newNode;
  }
};

} // namespace Cougar::Utils

// Token fomratter
template <typename T> struct fmt::formatter<Cougar::Utils::List<T>> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Utils::List<T> &list, FormatContext &ctx) {
    return format_to(ctx.out(), "{{{}}}",
                     fmt::join(list.begin(), list.end(), ", "));
  }
};

#include <gtest/gtest.h>

#include "utils/zone_allocator.hh"

namespace Cougar::Utils::Tests {

TEST(ZoneAllocatorTests, InitialValues) {
  ZoneAllocator a;

  EXPECT_EQ(0, a.getAllocatedBytes());
  EXPECT_EQ(0, a.getUsedBytes());
}

TEST(ZoneAllocatorTests, SimpleAllocation) {

  struct SomeStruct {
    double x = 44, y = 77;
  };

  ZoneAllocator a;

  SomeStruct *s = a.allocate<SomeStruct>();

  ASSERT_TRUE(s);
  EXPECT_EQ(44, s->x);
  EXPECT_EQ(77, s->y);

  EXPECT_EQ(sizeof(SomeStruct), a.getAllocatedBytes());
  EXPECT_GT(a.getUsedBytes(), 0);
}

TEST(ZoneAllocatorTests, AllocationWithParams) {

  class Iface {
  public:
    virtual int getNum() const = 0;
  };

  class Impl : public Iface {
  public:
    Impl(int a, int b) { mNum = a + b; }
    int getNum() const override { return mNum; }

  private:
    int mNum;
  };

  ZoneAllocator a;
  Iface *i = a.allocate<Impl>(7, 11);

  ASSERT_TRUE(i);
  EXPECT_EQ(7 + 11, i->getNum());
}

} // namespace Cougar::Utils::Tests
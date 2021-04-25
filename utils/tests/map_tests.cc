#include <gtest/gtest.h>

#include "utils/map.hh"

namespace Cougar::Utils::Tests {

TEST(MapTests, Emplace) {
  ZoneAllocator zone;

  struct PITA {

    int x;

    PITA(int a, int b) : x(a + b) {}
  };

  Map<std::string_view, PITA> map;

  PITA *ptr1 = map.emplace("one", 1, 2);
  PITA *ptr2 = map.emplace("two", 3, 4);

  ASSERT_TRUE(ptr1);
  EXPECT_EQ(1 + 2, ptr1->x);

  ASSERT_TRUE(ptr2);
  EXPECT_EQ(3 + 4, ptr2->x);

  EXPECT_THROW(map.emplace("one", 7, 7), std::exception);
  EXPECT_THROW(map.emplace("two", 7, 7), std::exception);
}

TEST(MapTests, Find) {
  ZoneAllocator zone;
  Map<std::string_view, int> map;

  map.emplace("one", 1);
  map.emplace("two", 2);
  map.emplace("three", 3);
  map.emplace("four", 4);

  int *p1 = map.find("one");
  ASSERT_TRUE(p1);
  EXPECT_EQ(1, *p1);

  int *p2 = map.find("two");
  ASSERT_TRUE(p2);
  EXPECT_EQ(2, *p2);

  int *p3 = map.find("three");
  ASSERT_TRUE(p3);
  EXPECT_EQ(3, *p3);

  int *p4 = map.find("four");
  ASSERT_TRUE(p4);
  EXPECT_EQ(4, *p4);

  int *p5 = map.find("five");
  EXPECT_FALSE(p5);
}

TEST(MapTests, HeterogenousTypes) {

  struct Base {
    Base(int a) : mA(a){};
    virtual int value() const { return mA; }
    int mA;
  };

  struct Derived : Base {
    Derived(int a, int b) : Base(a), mB(b) {}
    int value() const override { return mA + mB; }
    int mB;
  };

  ZoneAllocator zone;
  Map<std::string_view, Base> map;

  map.emplace("base", 42);
  map.emplace<Derived>("derived", 13, 7);

  EXPECT_EQ(42, map.find("base")->value());
  EXPECT_EQ(13 + 7, map.find("derived")->value());
}

} // namespace Cougar::Utils::Tests
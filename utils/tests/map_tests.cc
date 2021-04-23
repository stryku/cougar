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

} // namespace Cougar::Utils::Tests
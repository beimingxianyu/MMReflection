#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

TEST(reflection, meta) {
  // default types
  EXPECT_NE(GetMetaDatabase().find(typeid(void).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(char).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::uint16_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::uint32_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::uint64_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::int8_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::int16_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::int32_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::int64_t).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(float).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(double).hash_code()), GetMetaDatabase().end());
  EXPECT_NE(GetMetaDatabase().find(typeid(std::string).hash_code()), GetMetaDatabase().end());
}

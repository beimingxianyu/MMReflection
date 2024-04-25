#include <cstdlib>
#include <gtest/gtest.h>

#include "reflection.h"

using namespace MM::Reflection;

template<typename T>
T GetEmptyObject() {
  return T{};
}

struct TrivialSubStruct1 {
  std::uint8_t property1_;
  std::uint16_t property2_;
  std::uint32_t property3_;

  friend bool operator==(const TrivialSubStruct1& lhs,
                         const TrivialSubStruct1& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ && lhs.property4_ == rhs.property4_;
  }
  std::uint64_t property4_;
};
struct TrivialSubStruct2 {
  std::int8_t property1_;
  std::int16_t property2_;
  std::int32_t property3_;
  std::int64_t property4_;

  friend bool operator==(const TrivialSubStruct2& lhs,
                         const TrivialSubStruct2& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ && lhs.property4_ == rhs.property4_;
  }
};
struct TrivialSubStruct3 {
  float property1_;
  double property2_;

  friend bool operator==(const TrivialSubStruct3& lhs,
                         const TrivialSubStruct3& rhs) {
    return lhs.property1_ == rhs.property1_ && lhs.property2_ == rhs.property2_;
  }
};
struct TrivialSubStruct4 {
  std::uint8_t property1_;
  std::uint16_t property2_;
  std::int32_t property3_;
  std::int64_t property4_;

  friend bool operator==(const TrivialSubStruct4& lhs,
                         const TrivialSubStruct4& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ && lhs.property4_ == rhs.property4_;
  }
};
struct TrivialSubStruct5 {
  std::int8_t property1_;
  std::int16_t property2_;
  std::uint32_t property3_;
  std::uint64_t property4_;

  friend bool operator==(const TrivialSubStruct5& lhs,
                         const TrivialSubStruct5& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ && lhs.property4_ == rhs.property4_;
  }
};
struct TrivialStruct {
  TrivialSubStruct1 property1_;
  TrivialSubStruct2 property2_;
  TrivialSubStruct3 property3_;
  TrivialSubStruct4 property4_;
  TrivialSubStruct5 property5_;
  int property6_;
  float property7_;
  double property8_;

  friend bool operator==(const TrivialStruct& lhs, const TrivialStruct& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ &&
           lhs.property4_ == rhs.property4_ &&
           lhs.property5_ == rhs.property5_ &&
           lhs.property6_ == rhs.property6_ &&
           lhs.property7_ == rhs.property7_ && lhs.property8_ == rhs.property8_;
  }
};

MM_REGISTER {
  Class<TrivialSubStruct1>{"TrivialSubStruct1"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct1>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct2>{"TrivialSubStruct2"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct2>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct3>{"TrivialSubStruct3"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct3>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct4>{"TrivialSubStruct4"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct4>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct5>{"TrivialSubStruct5"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct5>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());

  Class<TrivialStruct>{"TrivialStruct"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialStruct>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
}

void AddRandomBit(char* address, size_t size) {
    srand(static_cast<unsigned int>(time(nullptr)));
    size_t randomIndex = rand() % (size * 8);

    size_t byteIndex = randomIndex / 8;
    size_t bitPosition = randomIndex % 8;

    address[byteIndex] ^= (1 << bitPosition);
}

TEST(reflection, serialize) {
  TrivialStruct test_trivial_struct{};
  AddRandomBit(reinterpret_cast<char*>(&test_trivial_struct), sizeof(TrivialStruct));

  Variable trivial_variable_refrence = Variable::CreateVariable(test_trivial_struct);
  DataBuffer data_buffer{};
  Serialize(data_buffer, trivial_variable_refrence);
  ASSERT_EQ(data_buffer.WriteToFile("./trivial_variable_refrence.bin"), true);
  data_buffer.Clear();
  ASSERT_EQ(data_buffer.LoadFromFile("./trivial_variable_refrence.bin"), true);
  Variable trivial_variable_refrence_deserialize = Deserialize(data_buffer);
  ASSERT_EQ(trivial_variable_refrence_deserialize.IsValid(), true);
  ASSERT_EQ(trivial_variable_refrence_deserialize.IsRefrenceVariable(), true);
  ASSERT_EQ(trivial_variable_refrence_deserialize.GetType()->GetOriginalTypeHashCode(), typeid(TrivialStruct).hash_code());
  ASSERT_EQ(*static_cast<TrivialStruct*>(trivial_variable_refrence.GetValue()), *static_cast<TrivialStruct*>(trivial_variable_refrence_deserialize.GetValue()));
}

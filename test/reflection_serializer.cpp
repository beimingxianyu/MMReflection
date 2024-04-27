#include <gtest/gtest.h>

#include <cstdlib>
#include <unordered_set>

#include "reflection.h"

#pragma pack(8)

using namespace MM::Reflection;

template<typename T>
T GetEmptyObject() {
  return T{};
}

void RandomBit(char* address, size_t size) {
  for (size_t i = 0; i != size; ++i) {
    srand(static_cast<unsigned int>(time(nullptr)));
    int random_value = rand();

    address[i] = static_cast<char>(random_value);
  }
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

float g_float = 0.0;
double g_double = 0.0;
std::unordered_set<void*> g_deleted_refrence_set{};

class RecursionSubClassBase {
public:
  virtual ~RecursionSubClassBase() = default;
  virtual void RandomData() = 0;
  virtual void DeleteRefrence() = 0;
  virtual std::uint32_t GetID() = 0;
};

class RecursionSubClass1 : public RecursionSubClassBase {
  MM_GENERATE_REFLECTION_BODY();

public:
  RecursionSubClass1()
    : property1_(),
      property2_(),
      property3_(),
      property4_(new double{0.0}),
      property5_(g_float) {}
  ~RecursionSubClass1() override { delete property4_; }
  RecursionSubClass1(RecursionSubClass1&& other) noexcept
      : RecursionSubClassBase(std::move(other)),
        property1_(other.property1_),
        property2_(other.property2_),
        property3_(other.property3_),
        property4_(other.property4_),
        property5_(other.property5_) {
    other.property4_ = nullptr;
  }

  void RandomData() override {
    RandomBit(reinterpret_cast<char*>(&property1_), sizeof(property1_));
    RandomBit(reinterpret_cast<char*>(&property2_), sizeof(property2_));
    RandomBit(reinterpret_cast<char*>(&property3_), sizeof(property3_));
    RandomBit(reinterpret_cast<char*>(property4_), sizeof(*property4_));
    RandomBit(reinterpret_cast<char*>(&property5_), sizeof(property5_));
  }

  void DeleteRefrence() override {
    if (g_deleted_refrence_set.count(&property5_) != 0) {
      g_deleted_refrence_set.emplace(&property5_);
      delete &property5_;
    }
  }

  std::uint32_t GetID() override {
    return 1;
  }

  friend bool operator==(const RecursionSubClass1& lhs,
                         const RecursionSubClass1& rhs) {
    assert(lhs.property4_ != nullptr && rhs.property4_ != nullptr);
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ &&
           *lhs.property4_ == *rhs.property4_ &&
           lhs.property5_ == rhs.property5_;
  }

 private:
  int property1_;
  double property2_;
  float property3_;
  double* property4_;
  float& property5_;
};

RecursionSubClass1 g_recursion_sub_class1{};

class RecursionSubClass2 : public RecursionSubClassBase{
  MM_GENERATE_REFLECTION_BODY();

public:
  RecursionSubClass2()
     : property1_(), property2_(g_double), property3_(new float{0.0}) {}
  ~RecursionSubClass2() override { delete property3_; }
  RecursionSubClass2(RecursionSubClass2&& other) noexcept
      : RecursionSubClassBase(std::move(other)),
        property1_(other.property1_),
        property2_(other.property2_),
        property3_(other.property3_) {
    other.property3_ = nullptr;
  }

  void RandomData() override {
    RandomBit(reinterpret_cast<char*>(&property1_), sizeof(property1_));
    RandomBit(reinterpret_cast<char*>(&property2_), sizeof(property2_));
    RandomBit(reinterpret_cast<char*>(property3_), sizeof(*property3_));
  }

  void DeleteRefrence() override {
    if (g_deleted_refrence_set.count(&property2_) != 0) {
      g_deleted_refrence_set.emplace(&property2_);
      delete &property2_;
    }
  }

  std::uint32_t GetID() override {
    return 2;
  }

   friend bool operator==(const RecursionSubClass2& lhs,
                          const RecursionSubClass2& rhs) {
    assert(lhs.property3_ != nullptr && rhs.property3_ != nullptr);
     return lhs.property1_ == rhs.property1_ &&
            lhs.property2_ == rhs.property2_ &&
            *lhs.property3_ == *rhs.property3_;
   }

private:
  double property1_;
  double& property2_;
  float* property3_;
};

RecursionSubClass2 g_recursion_sub_class2{};

class RecursionSubClass3 : public RecursionSubClassBase {
  MM_GENERATE_REFLECTION_BODY();

public:
  RecursionSubClass3()
      : property1_(),
        property2_(),
        property3_(),
        property4_(g_float),
        property5_(new RecursionSubClass1{}),
        property6_(g_recursion_sub_class2) {}
  ~RecursionSubClass3() override { delete property5_; }
  RecursionSubClass3(RecursionSubClass3&& other) noexcept
      : RecursionSubClassBase(std::move(other)),
        property1_(other.property1_),
        property2_(other.property2_),
        property3_(other.property3_),
        property4_(other.property4_),
        property5_(other.property5_),
        property6_(other.property6_) {
    other.property5_ = nullptr;
  }

  void RandomData() override {
    assert(property5_ != nullptr);
    RandomBit(reinterpret_cast<char*>(&property1_), sizeof(property1_));
    RandomBit(reinterpret_cast<char*>(&property2_), sizeof(property2_));
    RandomBit(reinterpret_cast<char*>(&property3_), sizeof(property3_));
    RandomBit(reinterpret_cast<char*>(&property4_), sizeof(property4_));
    property5_->RandomData();
    property6_.RandomData();
  }
  void DeleteRefrence() override {
    if (g_deleted_refrence_set.count(&property4_) != 0) {
      g_deleted_refrence_set.emplace(&property4_);
      delete &property4_;
    }
    if (g_deleted_refrence_set.count(&property6_) != 0) {
      property6_.DeleteRefrence();
      g_deleted_refrence_set.emplace(&property6_);
      delete &property6_;
    }
  }

  std::uint32_t GetID() override {
    return 3;
  }

  friend bool operator==(const RecursionSubClass3& lhs,
                         const RecursionSubClass3& rhs) {
    assert(lhs.property5_ != nullptr && rhs.property5_ != nullptr);
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ &&
           lhs.property4_ == rhs.property4_ &&
           *lhs.property5_ == *rhs.property5_ &&
           lhs.property6_ == rhs.property6_;
  }

 private:
  float property1_;
  double property2_;
  int property3_;
  float& property4_;
  RecursionSubClass1* property5_;
  RecursionSubClass2& property6_;
};

RecursionSubClass3 g_recursion_sub_class3{};

class RecursionSubClass4 : public RecursionSubClassBase {
  MM_GENERATE_REFLECTION_BODY();

public:
  RecursionSubClass4()
      : property1_(new RecursionSubClass1{}), property2_(new RecursionSubClass2{}), property3_(new RecursionSubClass3{}) {}
 ~RecursionSubClass4() override {
    delete property1_;
    delete property2_;
    delete property3_;
  }
  RecursionSubClass4(RecursionSubClass4&& other) noexcept
      : RecursionSubClassBase(std::move(other)),
        property1_(other.property1_),
        property2_(other.property2_),
        property3_(other.property3_) {
    other.property1_ = nullptr;
    other.property2_ = nullptr;
    other.property3_ = nullptr;
  }

  void RandomData() override {
    assert(property1_ != nullptr);
    assert(property2_ != nullptr);
    assert(property3_ != nullptr);
    property1_->RandomData();
    property2_->RandomData();
    property3_->RandomData();
  }
  void DeleteRefrence() override {}
  std::uint32_t GetID() override {return 4;}

  friend bool operator==(const RecursionSubClass4& lhs,
                         const RecursionSubClass4& rhs) {
    assert(lhs.property1_ != nullptr && rhs.property1_ != nullptr);
    assert(lhs.property2_ != nullptr && rhs.property2_ != nullptr);
    assert(lhs.property3_ != nullptr && rhs.property3_ != nullptr);
    return *lhs.property1_ == *rhs.property1_ &&
           *lhs.property2_ == *rhs.property2_ &&
           *lhs.property3_ == *rhs.property3_;
  }

 private:
  RecursionSubClass1* property1_{};
  RecursionSubClass2* property2_{};
  RecursionSubClass3* property3_{};
};

class RecursionSubClass5 : public RecursionSubClassBase {
public:
  RecursionSubClass5()
      : property1_(g_recursion_sub_class1), property2_(g_recursion_sub_class2), property3_(g_recursion_sub_class3) {}

public:
  void RandomData() override {
    property1_.RandomData();
    property2_.RandomData();
    property3_.RandomData();
  }
  void DeleteRefrence() override {
    if (g_deleted_refrence_set.count(&property1_) != 0) {
      g_deleted_refrence_set.emplace(&property1_);
      property1_.DeleteRefrence();
      delete &property1_;
    }
    if (g_deleted_refrence_set.count(&property2_) != 0) {
      g_deleted_refrence_set.emplace(&property2_);
      property2_.DeleteRefrence();
      delete &property2_;
    }
    if (g_deleted_refrence_set.count(&property3_) != 0) {
      g_deleted_refrence_set.emplace(&property3_);
      property3_.DeleteRefrence();
      delete &property3_;
    }
  }

  std::uint32_t GetID() override {
    return 5;
  }

  friend bool operator==(const RecursionSubClass5& lhs,
                         const RecursionSubClass5& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_;
  }

 private:
  RecursionSubClass1& property1_;
  RecursionSubClass2& property2_;
  RecursionSubClass3& property3_;
};

class RecursionClass {
  MM_GENERATE_REFLECTION_BODY();

public:
  RecursionClass() = default;

public:
  friend bool operator==(const RecursionClass& lhs, const RecursionClass& rhs) {
    return lhs.property1_ == rhs.property1_ &&
           lhs.property2_ == rhs.property2_ &&
           lhs.property3_ == rhs.property3_ &&
           lhs.property4_ == rhs.property4_ &&
           lhs.property5_ == rhs.property5_;
  }

  void RandomData () {
    property1_.RandomData();
    property2_.RandomData();
    property3_.RandomData();
    property4_.RandomData();
    property5_.RandomData();
  }

  void DeleteRefrence() {
    property1_.DeleteRefrence();
    property2_.DeleteRefrence();
    property3_.DeleteRefrence();
    property4_.DeleteRefrence();
    property5_.DeleteRefrence();
  }

 private:
  RecursionSubClass1 property1_{};
  RecursionSubClass2 property2_{};
  RecursionSubClass3 property3_{};
  RecursionSubClass4 property4_{};
  RecursionSubClass5 property5_{};
};

MM_REGISTER {
  Class<TrivialSubStruct1>{"TrivialSubStruct1"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct1>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct2>{"TrivialSubStruct2"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct2>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct3>{"TrivialSubStruct3"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct3>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct4>{"TrivialSubStruct4"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct4>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialSubStruct5>{"TrivialSubStruct5"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialSubStruct5>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());
  Class<TrivialStruct>{"TrivialStruct"}.Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<TrivialStruct>).SetSerializerName(TrivialSerializer::GetSerializerNameStatic());

  Class<RecursionSubClass1>{"RecursionSubClass1"}
      .Property("property1_", &RecursionSubClass1::property1_)
      .Property("property2_", &RecursionSubClass1::property2_)
      .Property("property3_", &RecursionSubClass1::property3_)
      .Property("property4_", &RecursionSubClass1::property4_)
      .Property("property5_", RefrencePropertyDescriptor<float>{40})
      .Method(Meta::GetEmptyObjectMethodName(),
              &GetEmptyObject<RecursionSubClass1>)
      .SetSerializerName(UnsefeRecursionSerializer::GetSerializerNameStatic());
  Class<RecursionSubClass2>{"RecursionSubClass2"}
      .Property("property1_", &RecursionSubClass2::property1_)
      .Property("property2_", RefrencePropertyDescriptor<double>{16})
      .Property("property3_", &RecursionSubClass2::property3_)
      .Method(Meta::GetEmptyObjectMethodName(),
              &GetEmptyObject<RecursionSubClass2>)
      .SetSerializerName(UnsefeRecursionSerializer::GetSerializerNameStatic());
  Class<RecursionSubClass3>{"RecursionSubClass3"}
      .Property("property1_", &RecursionSubClass3::property1_)
      .Property("property2_", &RecursionSubClass3::property2_)
      .Property("property3_", &RecursionSubClass3::property3_)
      .Property("property4_", RefrencePropertyDescriptor<float>{ 32})
      .Property("property5_", &RecursionSubClass3::property5_)
      .Property("property6_", RefrencePropertyDescriptor<RecursionSubClass2>{48})
      .Method(Meta::GetEmptyObjectMethodName(),
              &GetEmptyObject<RecursionSubClass3>)
      .SetSerializerName(UnsefeRecursionSerializer::GetSerializerNameStatic());
  Class<RecursionSubClass4>{"RecursionSubClass4"}
      .Property("property1_", &RecursionSubClass4::property1_)
      .Property("property2_", &RecursionSubClass4::property2_)
      .Property("property3_", &RecursionSubClass4::property3_)
      .Method(Meta::GetEmptyObjectMethodName(),
              &GetEmptyObject<RecursionSubClass4>)
      .SetSerializerName(UnsefeRecursionSerializer::GetSerializerNameStatic());
  Class<RecursionSubClass5>{"RecursionSubClass5"}
      .Property("property1_", RefrencePropertyDescriptor<RecursionSubClass1>{8})
      .Property("property2_", RefrencePropertyDescriptor<RecursionSubClass1>{16})
      .Property("property3_", RefrencePropertyDescriptor<RecursionSubClass1>{24})
      .Method(Meta::GetEmptyObjectMethodName(),
              &GetEmptyObject<RecursionSubClass5>)
      .SetSerializerName(UnsefeRecursionSerializer::GetSerializerNameStatic());
  Class<RecursionClass>{"RecursionClass"}
      .Property("property1_", &RecursionClass::property1_)
      .Property("property2_", &RecursionClass::property2_)
      .Property("property3_", &RecursionClass::property3_)
      .Property("property4_", &RecursionClass::property4_)
      .Property("property5_", &RecursionClass::property5_)
      .Method(Meta::GetEmptyObjectMethodName(), &GetEmptyObject<RecursionClass>)
      .SetSerializerName(UnsefeRecursionSerializer::GetSerializerNameStatic());
}


TEST(reflection, serialize) {
  TrivialStruct test_trivial_struct{};
  RandomBit(reinterpret_cast<char*>(&test_trivial_struct), sizeof(TrivialStruct));

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
  free(trivial_variable_refrence_deserialize.GetValue());
}

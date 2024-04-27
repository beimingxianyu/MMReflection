#pragma once

#include <data_buffer.h>
#include <meta.h>

namespace MM {
namespace Reflection {
struct alignas(4) SerializerDescriptor {
 std::uint32_t version_{0};
 // Add 1 to the actual type name size
 std::uint32_t c_style_type_name_size_{0};
  std::uint32_t custom_data_size_{0};
 bool is_refrence_{false};
};

struct DeserializerInfo {
  void* placement_address_{nullptr};
  bool is_refrence_{false};
  bool need_vptr_{true};
};

class SerializerBase {
  friend Variable Deserialize(const DataBuffer& data_buffer);

public:
  SerializerBase() = default;
  virtual ~SerializerBase() = default;
  SerializerBase(const SerializerBase& other) = delete;
  SerializerBase(SerializerBase&& other) = delete;
  SerializerBase& operator=(const SerializerBase& other) = delete;
  SerializerBase& operator=(SerializerBase&& other) = delete;

  virtual bool Check(const Meta& meta) const = 0;

  virtual DataBuffer& Serialize(DataBuffer& data_buffer,
                                Variable& variable) const = 0;

  virtual Variable Deserialize(const DataBuffer& data_buffer,
                               const Meta& meta,
                               const DeserializerInfo& deserializer_info) const = 0;

  virtual const std::string& GetSerializerName() const;

  virtual std::uint32_t GetVersion() const;

  static const std::string& GetSerializerNameStatic();

protected:
  void WriteDescriptor(DataBuffer& data_buffer, const Variable& variable,
                       const void* custom_data,
                       std::uint32_t custome_data_size) const ;

  static void* GetVariableValuePtr(Variable& variable);

  static SerializerDescriptor ReadDescriptor(const DataBuffer& data_buffer);

  static std::string ReadTypeName(const DataBuffer& data_buffer, std::uint64_t size);

  static void ReadCustomData(const DataBuffer& data_buffer, void* data_to,
                             std::uint64_t size);

  static void PreProcessDescriptor(const Meta& meta,
                                   Variable& invalid_variable_refrence,
                                   const DeserializerInfo& deserializer_info);
};

class TrivialSerializer final : public SerializerBase{
public:
 TrivialSerializer() = default;
 ~TrivialSerializer() override = default;

 bool Check(const Meta& meta) const override;

 DataBuffer& Serialize(DataBuffer& data_buffer,
                       Variable& variable) const override;

 Variable Deserialize(const DataBuffer& data_buffer, const Meta& meta, const DeserializerInfo& deserializer_info) const override;

 const std::string& GetSerializerName() const override;

 static const std::string& GetSerializerNameStatic();
};

class UnsefeRecursionSerializer: public SerializerBase {
public:
 UnsefeRecursionSerializer() = default;
 ~UnsefeRecursionSerializer() override = default;

 bool Check(const Meta& meta) const override;

 DataBuffer& Serialize(DataBuffer& data_buffer,
                       Variable& variable) const override;

 Variable Deserialize(const DataBuffer& data_buffer, const Meta& meta,
                      const DeserializerInfo& deserializer_info) const override;

 const std::string& GetSerializerName() const override;

 static const std::string& GetSerializerNameStatic();

private:
  static void ReadAllPropertyData(const DataBuffer& data_buffer,
                                 const Meta& meta, Variable& variable);
};

class RecursionSerializer final : public UnsefeRecursionSerializer {
public:
 RecursionSerializer() = default;
 ~RecursionSerializer() override = default;

 bool Check(const Meta& meta) const override;

 const std::string& GetSerializerName() const override;

 static const std::string& GetSerializerNameStatic();
};

DataBuffer& Serialize(DataBuffer& data_buffer, Variable& variable);

Variable Deserialize(const DataBuffer& data_buffer);
}  // namespace Reflection
}  // namespace MM

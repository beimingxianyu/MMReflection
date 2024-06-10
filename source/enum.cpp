#include "enum.h"

#include "meta.h"

MM::Reflection::EnumItem::EnumItem(const Meta* meta,
                                   const std::string& enum_name)
    : meta_(meta), pair_(nullptr) {
  assert(meta_ != nullptr && meta_->GetType().IsEnum());
  const EnumPair* target_enum = meta_->GetEnum(enum_name);
  if (target_enum != nullptr) {
    pair_ = target_enum;
  } else {
    meta_ = nullptr;
  }
}

bool MM::Reflection::EnumItem::IsValid() const {
  return meta_ != nullptr;
}

MM::Reflection::EnumValue MM::Reflection::EnumItem::GetValue() const {
  assert(IsValid());
  return pair_->value_;
}

std::uint8_t MM::Reflection::EnumItem::GetSize() const {
  assert(IsValid());
  return static_cast<std::uint8_t>(meta_->GetType().GetSize());
}

const std::string& MM::Reflection::EnumItem::GetEnumName() const {
  assert(meta_ != nullptr);
  return pair_->enum_name_;
}

const MM::Reflection::Type* MM::Reflection::EnumItem::GetType() const {
  assert(meta_ != nullptr);
  return &(GetMeta()->GetType());
}

const MM::Reflection::Meta* MM::Reflection::EnumItem::GetMeta() const {
  assert(meta_ != nullptr);
  return meta_;
}

MM::Reflection::EnumItem::EnumItem(const Meta* meta, const EnumPair* pair)
    : meta_(meta),
      pair_(pair) {
  assert(meta_ != nullptr && pair_ != nullptr);
}

std::uint64_t std::hash<MM::Reflection::EnumPair>::operator()(
    const MM::Reflection::EnumPair& value) const noexcept {
  return hash<std::string>()(value.enum_name_) ^
         hash<MM::Reflection::EnumValue>()(value.value_);
}
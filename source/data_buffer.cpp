#include "data_buffer.h"

MM::Reflection::DataBuffer::~DataBuffer() { Release(); }

MM::Reflection::DataBuffer::DataBuffer(const std::uint64_t init_size)
    : data_(nullptr), capacity_(0), read_data_offset(0), add_data_offset(0) {
  Reserver(init_size);
}

MM::Reflection::DataBuffer::DataBuffer(DataBuffer&& other) noexcept
    : data_(other.data_),
      capacity_(other.capacity_),
      read_data_offset(other.read_data_offset),
      add_data_offset(other.add_data_offset) {
  other.data_ = nullptr;
  other.capacity_ = 0;
  other.read_data_offset = 0;
  other.add_data_offset = 0;
}

MM::Reflection::DataBuffer& MM::Reflection::DataBuffer::operator=(
    DataBuffer&& other) noexcept {
  if (std::addressof(other) == this) {
    return *this;
  }

  Release();

  data_ = other.data_;
  capacity_ = other.capacity_;
  read_data_offset = other.read_data_offset;
  add_data_offset = other.add_data_offset;

  other.data_ = nullptr;
  other.capacity_ = 0;
  other.read_data_offset = 0;
  other.add_data_offset = 0;

  return *this;
}

void MM::Reflection::DataBuffer::AddData(const void* data_from,
                                         const std::uint64_t size) {
  assert(data_from != nullptr && size != 0);

  if (size + add_data_offset > capacity_) {
    Reserver(capacity_ * 2);
  }

  memcpy(data_ + add_data_offset, data_from, size);

  add_data_offset += size;
}

void MM::Reflection::DataBuffer::ReadData(void* data_to,
                                          const std::uint64_t size) const {
  assert(data_to != nullptr && size != 0);
  assert(read_data_offset + size <= add_data_offset);

  memcpy(data_to, data_ + read_data_offset, size);

  read_data_offset += size;
}

void MM::Reflection::DataBuffer::Reserver(std::uint64_t new_size) {
  if (new_size <= capacity_) {
    return;
  }

  RowDataType* new_data = new RowDataType[new_size];
  if (data_ != nullptr) {
    memcpy(new_data, data_, add_data_offset);
  }

  delete data_;

  data_ = new_data;
  capacity_ = new_size;
}

void MM::Reflection::DataBuffer::Release() {
  delete data_;

  data_ = nullptr;
  capacity_ = 0;
  read_data_offset = 0;
  add_data_offset = 0;
}

void* MM::Reflection::DataBuffer::GetData() { return data_; }

const void* MM::Reflection::DataBuffer::GetData() const { return data_; }

std::uint64_t MM::Reflection::DataBuffer::GetCapacity() const {
  return capacity_;
}

std::uint64_t MM::Reflection::DataBuffer::GetReadDataOffset() const {
  return read_data_offset;
}

std::uint64_t MM::Reflection::DataBuffer::GetAddDataOffset() const {
  return add_data_offset;
}
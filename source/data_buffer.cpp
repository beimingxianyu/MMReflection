#include "data_buffer.h"

#include <fstream>
#include <iostream>

MM::Reflection::DataBuffer::~DataBuffer() { Release(); }

MM::Reflection::DataBuffer::DataBuffer(const std::uint64_t init_size)
    : data_(nullptr), capacity_(0), read_data_offset_(0), add_data_offset_(0) {
  Reserver(init_size);
}

MM::Reflection::DataBuffer::DataBuffer(DataBuffer&& other) noexcept
    : data_(other.data_),
      capacity_(other.capacity_),
      read_data_offset_(other.read_data_offset_),
      add_data_offset_(other.add_data_offset_) {
  other.data_ = nullptr;
  other.capacity_ = 0;
  other.read_data_offset_ = 0;
  other.add_data_offset_ = 0;
}

MM::Reflection::DataBuffer& MM::Reflection::DataBuffer::operator=(
    DataBuffer&& other) noexcept {
  if (std::addressof(other) == this) {
    return *this;
  }

  Release();

  data_ = other.data_;
  capacity_ = other.capacity_;
  read_data_offset_ = other.read_data_offset_;
  add_data_offset_ = other.add_data_offset_;

  other.data_ = nullptr;
  other.capacity_ = 0;
  other.read_data_offset_ = 0;
  other.add_data_offset_ = 0;

  return *this;
}

void MM::Reflection::DataBuffer::AddData(const void* data_from,
                                         const std::uint64_t size) {
  assert(data_from != nullptr && size != 0);

  if (size + add_data_offset_ > capacity_) {
    if (capacity_ == 0) {
      capacity_ = 1024;
    }
    Reserver(capacity_ * 2);
  }

  memcpy(data_ + add_data_offset_, data_from, size);

  add_data_offset_ += size;
}

void MM::Reflection::DataBuffer::ReadData(void* data_to,
                                          const std::uint64_t size) const {
  assert(data_to != nullptr && size != 0);
  assert(read_data_offset_ + size <= add_data_offset_);

  memcpy(data_to, data_ + read_data_offset_, size);

  read_data_offset_ += size;
}

void MM::Reflection::DataBuffer::Reserver(std::uint64_t new_size) {
  if (new_size <= capacity_) {
    return;
  }

  RowDataType* new_data = new RowDataType[new_size];
  if (data_ != nullptr) {
    memcpy(new_data, data_, add_data_offset_);
  }

  delete data_;

  data_ = new_data;
  capacity_ = new_size;
}

void MM::Reflection::DataBuffer::Release() {
  delete data_;

  data_ = nullptr;
  capacity_ = 0;
  read_data_offset_ = 0;
  add_data_offset_ = 0;
}

void MM::Reflection::DataBuffer::Clear() {
  read_data_offset_ = 0;
  add_data_offset_ = 0;
}

void* MM::Reflection::DataBuffer::GetData() { return data_; }

const void* MM::Reflection::DataBuffer::GetData() const { return data_; }

std::uint64_t MM::Reflection::DataBuffer::GetCapacity() const {
  return capacity_;
}

std::uint64_t MM::Reflection::DataBuffer::GetReadDataOffset() const {
  return read_data_offset_;
}

std::uint64_t MM::Reflection::DataBuffer::GetAddDataOffset() const {
  return add_data_offset_;
}

bool MM::Reflection::DataBuffer::WriteToFile(
    const std::string& file_name) const {
  std::ofstream file{file_name, std::ios::trunc};
  if (!file.is_open()) {
    std::cerr << "[Error] [MMReflection] Falid to open file " << file_name
              << ".\n";
    return false;
  }

  file.write(reinterpret_cast<char*>(data_), add_data_offset_);

  file.close();

  return true;
}

bool MM::Reflection::DataBuffer::LoadFromFile(const std::string& file_name,
                                              std::uint64_t offset,
                                              std::uint64_t size) {
  std::ifstream file(file_name, std::ios::binary);

  if (!file) {
    std::cerr << "[Error] [MMReflection] Falid to open file " << file_name
              << ".\n";
    return false;
  }

  // Get file size.
  file.seekg(0, std::ios::end);
  std::uint64_t file_size = file.tellg();
  // No read size specified.
  if (size == ~static_cast<std::uint64_t>(0x0LL)) {
    if (offset >= file_size) {
      std::cerr
          << "[Error] [MMReflection] Failed to read data from file "
          << file_name << " offset " << offset << ".The size of file "
          << file_name << " is " << file_size
          << " bytes, and the specified read location exceeds the file size."
          << ".\n";
      return false;
    }

    file_size = file_size - offset;
  } else {
    if (offset + size > file_size) {
      std::cerr
          << "[Error] [MMReflection] Failed to read " << size
          << " bytes of data from file " << file_name << " offset " << offset
          << ".The size of file " << file_name << " is " << file_size
          << " bytes, and the specified read location exceeds the file size."
          << ".\n";
      return false;
    }

    file_size = size;
  }
  file.seekg(offset, std::ios::beg);

  if (file_size + add_data_offset_ > capacity_) {
    Reserver(file_size + add_data_offset_);
  }

  if (!file.read(reinterpret_cast<char*>(data_) + add_data_offset_,
                 file_size)) {
    std::cerr << "[Error] [MMReflection] Falid to read file " << file_name
              << ".\n";
    return false;
  }

  add_data_offset_ += file_size;

  file.close();

  return true;
}
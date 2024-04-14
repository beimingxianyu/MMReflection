#pragma once

#include <cstring>
#include <cstdint>


namespace MM {
namespace Reflection {
class DataBuffer {
public:
  using RowDataType = std::uint8_t;

public:
  DataBuffer() = default;
  ~DataBuffer();
  explicit DataBuffer(const std::uint64_t init_size);
  DataBuffer(const DataBuffer& other) = delete;
  DataBuffer(DataBuffer&& other) noexcept;
  DataBuffer& operator=(const DataBuffer& other) = delete;
  DataBuffer& operator=(DataBuffer&& other) noexcept;

 public:
  void AddData(const void* data_from, const std::uint64_t size);

  void ReadData(void* data_to, const std::uint64_t size) const;

  void Reserver(std::uint64_t new_size);

  void Release();

  void* GetData();

  const void* GetData() const;

  [[nodiscard]] std::uint64_t GetCapacity() const;

  [[nodiscard]] std::uint64_t GetReadDataOffset() const;

  [[nodiscard]] std::uint64_t GetAddDataOffset() const;

 private:
  RowDataType* data_{nullptr};
  std::uint64_t capacity_{0};

  mutable std::uint64_t read_data_offset{0};
  std::uint64_t add_data_offset{0};
};

}
}
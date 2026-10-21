#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <string_view>

namespace onix::core {
enum class BufferType {
    Original,
    Add,
};

struct Piece{
    BufferType buffer{BufferType::Original};
    std::size_t start{0};
    std::size_t length{0};
};

class PieceTable {
public:
    explicit PieceTable(std::string_view initial_text = "");
    
    void insert(std::size_t offset, std::string_view text);
    void remove(std::size_t offset, std::size_t length);
    
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    
    [[nodiscard]] std::string get_text() const;

private:
    std::string original_buffer_;
    std::string add_buffer_;
    std::vector<Piece> pieces_;
    std::size_t total_size_{0};
};

}
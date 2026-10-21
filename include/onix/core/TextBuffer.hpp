#pragma once

#include "onix/core/LineIndex.hpp"
#include "onix/core/PieceTable.hpp"
#include <cstddef>
#include <string>
#include <string_view>
namespace onix::core {

class TextBuffer {
public:
    TextBuffer();
    explicit TextBuffer(std::string_view initial_text);

    [[nodiscard]] std::size_t line_count() const noexcept;
    [[nodiscard]] std::string get_line(std::size_t line) const;
    [[nodiscard]] std::string get_text() const;

    [[nodiscard]] Position offset_to_position(std::size_t offset) const;
    [[nodiscard]] std::size_t position_to_offset(Position pos) const;

    void insert(std::size_t offset, std::string_view text);
    void insert_at(Position pos, std::string_view text);

    void remove(std::size_t offset, std::size_t length);

private:
    PieceTable piece_table_;
    LineIndex line_index_;
    
};
    
}
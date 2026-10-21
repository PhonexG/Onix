#include "onix/core/TextBuffer.hpp"
#include "onix/core/LineIndex.hpp"
#include <cstddef>
#include <string>
#include <string_view>

namespace onix::core {

TextBuffer::TextBuffer() = default;

TextBuffer::TextBuffer(std::string_view initial_text):
    piece_table_(initial_text),
    line_index_(initial_text) {}


std::size_t TextBuffer::line_count() const noexcept {
    return line_index_.line_count();
}

std::string TextBuffer::get_line(std::size_t line) const {
    std::size_t start = line_index_.line_to_offset(line);
    std::size_t end = (line + 1 < line_count()) ? line_index_.line_to_offset(line + 1) : piece_table_.size();
    return piece_table_.get_text().substr(start, end - start);
}


std::string TextBuffer::get_text() const {
    return piece_table_.get_text();
}

Position TextBuffer::offset_to_position(std::size_t offset) const {
    return line_index_.offset_to_position(offset);
}

std::size_t TextBuffer::position_to_offset(Position pos) const {
    return line_index_.line_to_offset(pos.line) + pos.column;
}

void TextBuffer::insert(std::size_t offset, std::string_view text) {
    piece_table_.insert(offset, text);
    line_index_.build_from(piece_table_.get_text()); // поки що базовий надійний варіант
}

void TextBuffer::insert_at(Position pos, std::string_view text) {
    insert(position_to_offset(pos), text);
}


void TextBuffer::remove(std::size_t offset, std::size_t length) {
    piece_table_.remove(offset, length);
    line_index_.build_from(piece_table_.get_text());
}

}
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <onix/core/LineIndex.hpp>
#include <stdexcept>
#include <string_view>

namespace onix::core {

LineIndex::LineIndex() = default;

LineIndex::LineIndex(std::string_view text) {
    build_from(text);
}

std::size_t LineIndex::line_count() const noexcept {
    return line_starts_.size();
}

std::size_t LineIndex::line_to_offset(std::size_t line) const {
    if (line >= line_starts_.size())
        throw std::out_of_range("Line out of range");

    return line_starts_[line];
}

void LineIndex::build_from(std::string_view text) {
    line_starts_.clear();
    line_starts_.push_back(0);

    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\n')
            line_starts_.push_back(i + 1);
    }
}

Position LineIndex::offset_to_position(std::size_t offset) const {
    auto it = std::upper_bound(line_starts_.begin(), line_starts_.end(), offset);
    --it;

    auto line = static_cast<std::size_t>(std::distance(line_starts_.begin(), it));
    std::size_t column = offset - *it;

    return Position{.line = line, .column = column};
}

}
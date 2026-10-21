#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace onix::core {

struct Position {
    std::size_t line{0};
    std::size_t column{0};

    auto operator<=>(const Position&) const = default;
};

class LineIndex {
public:
    LineIndex();
    explicit LineIndex(std::string_view text);

    [[nodiscard]] std::size_t line_count() const noexcept;
    [[nodiscard]] std::size_t line_to_offset(std::size_t line) const;
    [[nodiscard]] Position offset_to_position(std::size_t offset) const;

    void build_from(std::string_view text);

private:
    std::vector<std::size_t> line_starts_{0};
};

}

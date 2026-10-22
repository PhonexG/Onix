#pragma once

#include "onix/core/TextBuffer.hpp"
#include <cstddef>
#include <termios.h>

namespace onix::tui {

struct WindowSize {
    std::size_t rows{0};
    std::size_t cols{0};
};

class Terminal {
    public:
        Terminal();
        ~Terminal();

        Terminal(const Terminal&) = delete;
        Terminal& operator = (const Terminal&) = delete;

        Terminal(Terminal&&) noexcept;
        Terminal& operator = (Terminal&&) noexcept;

        void enable_raw_mode();
        void disable_raw_mode();

        [[nodiscard]] WindowSize get_window_size() const;

        void clear_screen();
        void set_cursor_position(std::size_t row, std::size_t col);

    private:
        termios original_termios_{};
        bool raw_mode_enabled_{false};
};
    
}
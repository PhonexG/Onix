#include <onix/tui/Terminal.hpp>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdexcept>
#include <string>

namespace onix::tui {
    Terminal::Terminal() {
        enable_raw_mode();
    }

    Terminal::~Terminal() {
        disable_raw_mode();
    }

    void Terminal::disable_raw_mode() {
        if(raw_mode_enabled_) {
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios_);
            raw_mode_enabled_ = false;
        }
    }

    void Terminal::enable_raw_mode() {
        if (raw_mode_enabled_) return;

        if (tcgetattr(STDIN_FILENO, &original_termios_) == -1) {
            throw std::runtime_error("Fail to get terminal attributes");
        }

        termios raw = original_termios_;

        raw.c_iflag &= ~static_cast<tcflag_t>(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~static_cast<tcflag_t>(OPOST);
        raw.c_lflag &= ~static_cast<tcflag_t>(ECHO | ICANON | IEXTEN | ISIG);

        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
            throw std::runtime_error("Faild to set termonl to raw mode");
        }

        raw_mode_enabled_ = true;
    }

    Terminal::Terminal(Terminal&& other) noexcept: 
        original_termios_(other.original_termios_),
        raw_mode_enabled_(other.raw_mode_enabled_) {
        other.raw_mode_enabled_ = false;
    }
    
    Terminal& Terminal::operator=(Terminal&& other) noexcept {
        if (this != &other) {
            disable_raw_mode();
            original_termios_ = other.original_termios_;
            raw_mode_enabled_ = other.raw_mode_enabled_;
            other.raw_mode_enabled_ = false;
        }
        return *this;
    }

    WindowSize Terminal::get_window_size() const {
        winsize ws{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
            return WindowSize{.rows = 24, .cols = 80};
        }
        return WindowSize{.rows = ws.ws_row, .cols = ws.ws_col};
    }

    void Terminal::clear_screen() {
        (void)write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
    }

    void Terminal::set_cursor_position(std::size_t row, std::size_t col) {
        std::string command = "\x1b[" + std::to_string(row + 1) + ";" + std::to_string(col + 1) + "H";
        (void)write(STDOUT_FILENO, command.data(), command.size());
    }

}
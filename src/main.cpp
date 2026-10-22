#include <onix/tui/Terminal.hpp>
#include <iostream>
#include <unistd.h>

int main() {
    try {
        onix::tui::Terminal terminal;

        terminal.clear_screen();
        auto size = terminal.get_window_size();

        std::cout << "Onix Editor (Raw Mode)\r\n";
        std::cout << "Window size: " << size.rows << " rows, " << size.cols << " cols\r\n";
        std::cout << "Press 'q' to exit...\r\n";

        char c = '\0';
        while (true) {
            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == 'q' || c == 17) {
                    break;
                }
                std::cout << "Key pressed: " << static_cast<int>(c) << " ('" << c << "')\r\n";
            }
        }

        terminal.clear_screen();
        std::cout << "Exited cleanly. Goodbye!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

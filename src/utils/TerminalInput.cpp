//
// Created by Ramiz Abdulla on 14/07/2025.
//

#include "TerminalInput.h"

#include "TerminalInput.h"
#include <termios.h>
#include <unistd.h>
#include <iostream>

static termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int readUserInputChar(std::string& currentInput) {
    char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) return -1;  // error or EOF

    if (c == 127 || c == 8) { // backspace/delete
        if (!currentInput.empty()) {
            currentInput.pop_back();
            std::cout << "\b \b" << std::flush;  // erase last char on terminal
        }
        return 0; // backspace handled, no char to add
    }

    std::cout << c << std::flush;
    return static_cast<unsigned char>(c);
}
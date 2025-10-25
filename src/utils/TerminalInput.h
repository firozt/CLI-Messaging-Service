#ifndef TERMINALINPUT_H
#define TERMINALINPUT_H

#include <string>

void enableRawMode();
void disableRawMode();
int readUserInputChar(std::string& currentInput);
void print_incoming(const std::string &msg, const std::string &cur_input);


#endif // TERMINALINPUT_H

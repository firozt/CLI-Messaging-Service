#ifndef TERMINALINPUT_H
#define TERMINALINPUT_H

#include <string>

void enableRawMode();
void disableRawMode();
int readUserInputChar(std::string& currentInput);


#endif // TERMINALINPUT_H

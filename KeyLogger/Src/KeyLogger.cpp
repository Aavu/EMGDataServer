//
// Created by Raghavasimhan Sankaranarayanan on 7/29/20.
//

#include "KeyLogger.h"

KeyLogger* KeyLogger::pInstance = nullptr;

KeyLogger *KeyLogger::getInstance() {
    pInstance = new KeyLogger();
    return pInstance;
}

KeyLogger::KeyLogger() {
    init();
}

KeyLogger::~KeyLogger() {
    delete pInstance;
}

void KeyLogger::init() {
    if (! m_bInitialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO); // Adding ECHO disables echoing key strokes in console
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        setbuf(stdin, nullptr);

        // set STDIN_FILENO as non-blocking
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

        m_bInitialized = true;
    }
}

char KeyLogger::getKeyPress() const {
    if (!m_bInitialized)
        return '0';
    char buf = '\0';
    auto ret = read(STDIN_FILENO, &buf, 1);
    if (ret == -1)
        return '0';
    return buf;
}

//int KeyLogger::_kbhit() {
//    int bytesAvailable = 0;
//    ioctl(STDIN_FILENO, FIONREAD, &bytesAvailable);
//    return bytesAvailable;
//}
